// Copyright 24-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <ctype.h>
#include <errno.h>
#include <locale.h>
#include "kut/js.h"
#include "kut/DEFS.h"
#include "kut/buf.h"
#include "kut/dec.h"

static void json_unicode(Buf *bf, char *hexdigits) {
  char hexvalue (char ch) {
    return (ch <= '9') ? ch - '0' : toupper(ch) - 'A' + 10;
  }
  unsigned int codepoint =
    hexvalue(*hexdigits) * 4096 +
    hexvalue(*(hexdigits + 1)) * 256 +
    hexvalue(*(hexdigits + 2)) * 16 +
    hexvalue(*(hexdigits + 3));
  if (codepoint < 0x80) {
    buf_cadd(bf, (codepoint>>0  & 0x7F) | 0x00);
    return;
  }
  if (codepoint < 0x0800) {
    buf_cadd(bf, (codepoint>>6  & 0x1F) | 0xC0);
    buf_cadd(bf, (codepoint>>0  & 0x3F) | 0x80);
    return;
  }
  buf_cadd(bf, (codepoint>>12 & 0x0F) | 0xE0);
  buf_cadd(bf, (codepoint>>6  & 0x3F) | 0x80);
  buf_cadd(bf, (codepoint>>0  & 0x3F) | 0x80);
}

static char *json_blanks(char *json) {
  while (*json && ((unsigned char)*json) <= ' ') {
    ++json;
  }
  return json;
}

inline
static char *json_snull(char *json) {
  return json_blanks(json + 4);
}

static char *json_sbool(char *json) {
  if (*json == 't') {
    return json_blanks(json + 4);
  }
  return json_blanks(json + 5);
}

static char *json_snumber(char *j) {
  while (
    *j && *j != '}' && *j != ']' && *j != ',' && ((unsigned char) *j) > ' '
  ) {
    ++j;
  }
  return json_blanks(j);
}

static char *json_sstring(char *j) {
  ++j;
  while (*j && *j != '"') {
    if (*j++ == '\\') {
      if (*j) {
        ++j;
      }
    }
  }
  if (*j) {
    ++j;
  }
  return json_blanks(j);
}

static char *json_selement(char *j);

static char *json_sarray(char *j) {
  ++j;
  j = json_blanks(j);
  while (*j && *j != ']') {
    j = json_selement(j);
    if (*j == ',') {
      ++j;
      j = json_blanks(j);
    }
  }
  if (*j) {
    ++j;
  }
  return json_blanks(j);
}

static char *json_sobject(char *j) {
  ++j;
  j = json_blanks(j);
  while (*j && *j != '}') {
    j = json_sstring(j);
    ++j;
    j = json_selement(j);
    if (*j == ',') {
      ++j;
      j = json_blanks(j);
    }
  }
  if (*j) {
    ++j;
  }
  return json_blanks(j);
}

static char *json_selement(char *j) {
  j = json_blanks(j);
  switch (*j) {
  case 'n': return json_snull(j);
  case 't':
  case 'f': return json_sbool(j);
  case '"': return json_sstring(j);
  case '[': return json_sarray(j);
  case '{': return json_sobject(j);
  case '-': return json_snumber(j);
  default :
    if (*j >= '0' && *j <= '9') {
      return json_snumber(j);
    } else {
      if (*j) {
        ++j;
      }
      return j;
    }
  }
}

static int json_rend(char *json) {
  while (*json && ((unsigned char)*json) <= ' ') {
    ++json;
  }
  return *json ? 0 : 1;
}

static char *errorf(char *json) {
  return strlen(json) > 70 ? str_f("%s...", str_left(json, 67)) : json;
}

int js_is_null (char *json) {
  char *j = json_blanks(json);
  if (memcmp(j, "null", 4)) {
    return 0;
  }
  return json_rend(j + 4);
}

int js_rb (char *json) {
  char *jsons = json_blanks(json);
  int r;

  char *j = jsons;
  if (memcmp(j, "true", 4)) {
    if (memcmp(j, "false", 5))
      EXC_ILLEGAL_ARGUMENT("Bad JSON string", "Boolean value", errorf(json));

    r = 0;
    j += 5;
  } else {
    r = 1;
    j += 4;
  }
  if (!json_rend(j))
    EXC_ILLEGAL_ARGUMENT("Bad JSON string", "Boolean value (spare characters)", errorf(json));

  return r;
}

int js_ri (char *json) {
  return js_rl(json);
}

long js_rl (char *json) {
  char *jsons = json_blanks(json);
  char *j = jsons;
  if (*j != '-' && (*j < '0' || *j > '9'))
    EXC_ILLEGAL_ARGUMENT("Bad JSON string", "Long value", errorf(json));

  Buf *bf = buf_new();
  while (
    *j && *j != '}' && *j != ']' && *j != ',' && ((unsigned char)*j) > ' '
  ) {
    ++j;
  }
  buf_add_buf(bf, jsons, j - jsons);
  char *n = buf_str(bf);
  if (!json_rend(j))
    EXC_ILLEGAL_ARGUMENT("Bad JSON string", "Long value (spare characters)", errorf(json));

  char *tail;
  errno = 0;
  long r = strtol(n, &tail, 10);
  if (errno)
    EXC_GENERIC(str_f("Bad JSON string: Long overflow in %s", errorf(json)));

  if (*tail)
    EXC_ILLEGAL_ARGUMENT("Bad JSON string", "Long value", errorf(json));

  return r;
}

double js_rd (char *json) {
  struct lconv *lc = localeconv();
  char *jsons = json_blanks(json);
  char *j = jsons;
  if (*j != '-' && (*j < '0' || *j > '9'))
    EXC_ILLEGAL_ARGUMENT("Bad JSON string", "Float value", errorf(json));

  Buf *bf = buf_new();
  while (
    *j && *j != '}' && *j != ']' && *j != ',' && ((unsigned char)*j) > ' '
  ) {
    ++j;
  }
  buf_add_buf(bf, jsons, j - jsons);
  char *n = buf_str(bf);
  int ix = str_cindex(n, '.');
  if (ix != -1) {
    n[ix] = *lc->decimal_point;
  }
  if (!json_rend(j))
    EXC_ILLEGAL_ARGUMENT("Bad JSON string", "Float value (spare characters)", errorf(json));

  errno = 0;
  char *tail;
  double r = strtod(n, &tail);
  if (errno)
    EXC_GENERIC(str_f("Bad JSON string: Float overflow in %s", errorf(json)));

  if (*tail)
    EXC_ILLEGAL_ARGUMENT("Bad JSON string", "Float value", errorf(json));

  return r;
}

char *js_rs (char *j) {
  int is_hex (char ch) {
    return (ch >= '0' && ch <= '9') ||
      (ch >= 'a' && ch <= 'f') ||
      (ch >= 'A' && ch <= 'F');
  }

  char *json = json_blanks(j);
  if (*json != '"')
    EXC_ILLEGAL_ARGUMENT("Bad JSON string", "String value (not begin with '\"')", errorf(j));

  ++json;
  Buf *bf = buf_new();
  while (*json && *json != '"') {
    if (*json == '\\') {
      ++json;
      switch (*json) {
        case '"' :
        case '\\' :
        case '/' :
          buf_cadd(bf, *json);
          break;
        case 'b' :
          buf_cadd(bf, '\b');
          break;
        case 'f' :
          buf_cadd(bf, '\f');
          break;
        case 'n' :
          buf_cadd(bf, '\n');
          break;
        case 'r' :
          buf_cadd(bf, '\r');
          break;
        case 't' :
          buf_cadd(bf, '\t');
          break;
        case 'u' : {
          ++json;
          int c = 5;
          while (--c) {
            if (!is_hex(*json++))
              EXC_ILLEGAL_ARGUMENT("Bad JSON string", "String value (Bad unicode)", errorf(j));

          }
          json_unicode(bf, json - 4);
          continue;
        }
        default :
          EXC_ILLEGAL_ARGUMENT("Bad JSON string", "String value (Bad escape sequence)", errorf(j));
      }
      ++json;
    } else {
      buf_cadd(bf, *json++);
    }
  }
  if (!*json)
    EXC_ILLEGAL_ARGUMENT("Bad JSON string", "String value (not end with '\"')", errorf(j));

  if (!json_rend(json + 1))
    EXC_ILLEGAL_ARGUMENT("Bad JSON string", "String value (spare characters)", errorf(j));

  return str_new(buf_str(bf));
}

// <char>
Arr *js_ra (char *j) {
  char *json = json_blanks(j);
  if (*json != '[')
    EXC_ILLEGAL_ARGUMENT("Bad JSON string", "Array value (not begin with '[')", errorf(j));

  ++json;
  // <char>
  Arr *a = arr_new();
  while (*json && *json != ']') {
    char *tmp = json;
    json = json_selement(json);
    Buf *bf = buf_new();
    buf_add_buf(bf, tmp, json - tmp);
    arr_push(a, str_new(buf_str(bf)));
    if (*json == ',') {
      ++json;
    } else if (*json && *json != ']')
      EXC_ILLEGAL_ARGUMENT("Bad JSON string", "Array value (comma missing)", errorf(j));

  }
  if (!*json)
    EXC_ILLEGAL_ARGUMENT("Bad JSON string", "Array value (not end with ']')", errorf(j));

  if (!json_rend(json + 1))
    EXC_ILLEGAL_ARGUMENT("Bad JSON string", "Array value (spare characters)", errorf(j));

  return a;
}

// <char>
Map *js_ro (char *j) {
  char *json = json_blanks(j);
  if (*json != '{')
    EXC_ILLEGAL_ARGUMENT("Bad JSON string", "Object value (not begin with '{')", errorf(j));

  json = json_blanks(json + 1);

  Map *m = map_new();
  while (*json && *json != '}') {
    if (*json != '"')
      EXC_ILLEGAL_ARGUMENT("Bad JSON string", "Object value (bad key)", errorf(j));

    char *tmp = json;
    json = json_sstring(json);
    Buf *kbf = buf_new();
    buf_add_buf(kbf, tmp, json - tmp);

    if (*json != ':')
      EXC_ILLEGAL_ARGUMENT("Bad JSON string", "Object value (':' is missing)", errorf(j));

    ++json;
    tmp = json;
    json = json_selement(json);
    Buf *vbf = buf_new();
    buf_add_buf(vbf, tmp, json - tmp);

    map_put(m, js_rs(buf_str(kbf)), str_new(buf_str(vbf)));

    if (*json == ',') {
      json = json_blanks(json + 1);
    } else if (*json && *json != '}')
      EXC_ILLEGAL_ARGUMENT("Bad JSON string", "Object value (comma missing)", errorf(j));

  }
  if (!*json)
    EXC_ILLEGAL_ARGUMENT("Bad JSON string", "Object value (not end with '}')", errorf(j));

  if (!json_rend(json + 1))
    EXC_ILLEGAL_ARGUMENT("Bad JSON string", "Object value (spare characters)", errorf(j));

  return m;
}

char *js_wn() {
  return str_new("null");
}

char *js_wb(int value) {
  return str_new(value ? "true" : "false");
}

char *js_wi(int n) {
  return str_f("%d", n);
}

char *js_wl(long n) {
  return str_f("%ld", n);
}

char *js_wf(double n, int scale) {
  return dec_ftos(n, scale);
}

char *js_ws(char *s) {
  Buf *bf = buf_new();

  char tmp;
  buf_cadd(bf, '"');
  while (*s) {
    tmp = *s++;
    switch (tmp) {
    case '"':
      buf_add(bf, "\\\"");
      break;
    case '\\':
      buf_add(bf, "\\\\");
      break;
    case '\b':
      buf_add(bf, "\\b");
      break;
    case '\f':
      buf_add(bf, "\\f");
      break;
    case '\n':
      buf_add(bf, "\\n");
      break;
    case '\r':
      buf_add(bf, "\\r");
      break;
    case '\t':
      buf_add(bf, "\\t");
      break;
    default:
      buf_cadd(bf, tmp);
    }
  }
  buf_cadd(bf, '"');
  return str_new(buf_str(bf));
}

// Arr<char>
char *js_wa(Arr *a) {
  int size = arr_size(a);
  Buf *bf = buf_new();
  buf_cadd(bf, '[');
  if (size > 0) {
    buf_add(bf, arr_get(a, 0));
    int i;
    for (i = 1; i < size; ++i) {
      buf_cadd(bf, ',');
      buf_add(bf, arr_get(a, i));
    }
  }
  buf_cadd(bf, ']');
  return str_new(buf_str(bf));
}

// Map<char>
char *js_wo(Map *m) {
  Buf *bf = buf_new();
  buf_cadd(bf, '{');
  // <char>
  Arr *kvs = map_to_array(m);
  for (int i = 0; i < arr_size(kvs); i++) {
    //<char>
    Kv *kv = arr_get(kvs, i);
    if (i) {
      buf_cadd(bf, ',');
    }
    buf_add(bf, js_ws(kv_key(kv)));
    buf_cadd(bf, ':');
    buf_add(bf, kv_value(kv));
  }
  buf_cadd(bf, '}');
  return str_new(buf_str(bf));
}
