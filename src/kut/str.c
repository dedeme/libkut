// Copyright 20-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <wctype.h>
#include "kut/str.h"
#include "kut/DEFS.h"
#include "kut/buf.h"

char *str_new (char *s) {
  char *r = ATOMIC(strlen(s) + 1);
  strcpy(r, s);
  return r;
}

char *str_new_c (char ch) {
  char *r = ATOMIC(2);
  *r = ch;
  r[1] = 0;
  return r;
}

int str_cmp(char *s1, char *s2) {
  return strcmp(s1, s2);
}

int str_cmp_locale(char *s1, char *s2) {
  return strcoll(s1, s2);
}

int str_greater(char *s1, char *s2) {
  return strcmp(s1, s2) > 0;
}

int str_greater_locale(char *s1, char *s2) {
  return strcoll(s1, s2) > 0;
}

int str_eq (char *str1, char *str2) {
  return !strcmp(str1, str2);
}

int str_starts (char *str, char  *substr) {
  int sublen = strlen(substr);
  return strlen(str) >= sublen && !memcmp(str, substr, sublen);
}

int str_ends (char *str, char  *substr) {
  int slen = strlen(str);
  int sublen = strlen(substr);
  return slen >= sublen && !memcmp(str + slen - sublen, substr, sublen);
}

int str_cindex (char *str, char ch) {
  int c = 0;
  char *p  = str;
  while (*p) {
    if (*p++ == ch) {
      return c;
    };
    ++c;
  }
  return -1;
}

int str_cindex_from (char *str, char ch, int from) {
  if (from >= strlen(str)) {
    return -1;
  }
  int c = from;
  char *p  = str + from;
  while (*p) {
    if (*p++ == ch) {
      return c;
    };
    ++c;
  }
  return -1;
}

int str_index(char *str, char *substr) {
  if (!*substr) {
    return 0;
  }
  int c = 0;
  int limit = strlen(str) - strlen(substr);
  char *p  = str;
  for (;;) {
    if (c > limit) {
      break;
    }
    if (str_starts(p++, substr)) {
      return c;
    };
    ++c;
  }
  return -1;
}

int str_index_from(char *str, char *substr, int from) {
  if (from >= strlen(str)) {
    return -1;
  }

  if (!*substr) {
    return from;
  }
  int c = from;
  int limit = strlen(str) - strlen(substr);
  char *p  = str + from;
  for (;;) {
    if (c > limit) {
      break;
    }
    if (str_starts(p++, substr)) {
      return c;
    };
    ++c;
  }
  return -1;
}

int str_last_cindex(char *str, char ch) {
  int r = -1;
  int c = 0;
  char *p  = str;
  while (*p) {
    if (*p++ == ch) {
      r = c;
    };
    ++c;
  }
  return r;
}

int str_last_index(char *str, char *substr) {
  int r = -1;
  if (!*substr) {
    return strlen(str);
  }
  int c = 0;
  int limit = strlen(str) - strlen(substr);
  char *p  = str;
  while (*p) {
    if (c > limit) {
      break;
    }
    if (str_starts(p++, substr)) {
      r = c;
    };
    ++c;
  }
  return r;
}

int str_len (char *str) {
  return strlen(str);
}

char *str_sub(char *str, int begin, int end) {
  int l = strlen(str);
  int df = -1;
  if (begin < 0) begin = l + begin;
  if (begin >= l) return "";
  if (begin < 0) begin = 0;

  if (end < 0) end = l + end;
  if (end > l) end = l;
  if (end <= 0) return "";

  df = end - begin;

  char *r;
  if (df < 0) return "";

  r = ATOMIC(df + 1);
  r[df] = 0;
  memcpy(r, str + begin, df);
  return r;
}

char *str_left(char *str, int end) {
  return str_sub(str, 0, end);
}

char *str_right(char *str, int begin) {
  return str_sub(str, begin, strlen(str));
}

char *str_ltrim(char *str) {
  unsigned char ch = *str;
  while (ch && ch <= ' ') ch = *++str;
  return str_new(str);
}

char *str_rtrim(char *str) {
  char *p = str + (strlen(str) - 1);
  while (p >= str && ((unsigned char)*p) <= ' ') {
    --p;
  }
  return str_left(str, (p - str) + 1);
}

char *str_trim(char *str) {
  unsigned char ch = *str;
  while (ch && ch <= ' ') ch = *++str;
  return str_rtrim(str);
}

// <char>
Arr *str_csplit(char *s, char sep) {
  // <char>
  Arr *r = arr_new();
  int i = str_cindex(s, sep);
  while (i != -1) {
    char *sub = ATOMIC(i + 1);
    sub[i] = 0;
    memcpy(sub, s, i);
    arr_push(r, sub);
    s = s + i + 1;
    i = str_cindex(s, sep);
  }
  arr_push(r, str_new(s));
  return r;
}

// <char>
Arr *str_csplit_trim(char *str, char sep) {
  // <char>
  Arr *r = str_csplit(str, sep);
  char **p = (char **)arr_begin(r);
  while (p < (char **)arr_end(r)) {
    *p = str_trim(*p);
    ++p;
  }
  return r;
}

// <char>
Arr *str_split(char *s, char *sep) {
  // <char>
  Arr *r = arr_new();
  int len = strlen(sep);
  if (!len) {
    char *rune;
    s = str_next_rune(&rune, s);
    while (*rune) {
      arr_push(r, rune);
      s = str_next_rune(&rune, s);
    }
    return r;
  }

  int i = str_index(s, sep);
  while (i != -1) {
    char *sub = ATOMIC(i + 1);
    sub[i] = 0;
    memcpy(sub, s, i);
    arr_push(r, sub);
    s = s + i + len;
    i = str_index(s, sep);
  }
  arr_push(r, str_new(s));
  return r;
}

// <char>
Arr *str_split_trim(char *str, char *sep) {
  // <char>
  Arr *r = str_split(str, sep);
  char **p = (char **)arr_begin(r);
  while (p < (char **)arr_end(r)) {
    *p = str_trim(*p);
    ++p;
  }
  return r;
}

char *str_creplace(char *s, char old, char new) {
  char *r = str_new(s);
  char *p = r;
  while (*p) {
    if (*p == old)
      *p = new;
    ++p;
  }
  return r;
}

char *str_replace(char *s, char *old, char *new) {
  if (!*old) return str_new(s);

  Buf *bf = buf_new();
  int len = strlen(old);
  int i = str_index(s, old);
  while (i != -1) {
    buf_add(bf, str_left(s, i));
    buf_add(bf, new);
    s = s + i + len;
    i = str_index(s, old);
  }
  buf_add(bf, s);
  return str_new(buf_str(bf));
}

char *str_vf(char *format, va_list args) {
  char *tmp;
  int len = vasprintf(&tmp, format, args);
  if (len < 0)
    exc_throw(
      exc_illegal_argument_t, "Wrong format",
      __FILE__, (char *)__func__, __LINE__);
  char *msg = ATOMIC(len + 1);
  strcpy(msg, tmp);
  free(tmp);
  return msg;
}

char *str_f(char *format, ...) {
  va_list args;
  va_start(args, format);
  char *r = str_vf(format, args);
  va_end(args);
  return r;
}

int str_runes_len(char *s) {
  unsigned char b1, b2, b3, b4;
  int r = 0;
  while (*s) {
    ++r;
    b1 = *s++;
    if (b1 < 0x80) continue;
    else if (b1 < 0xC2) return -1;
    else if (b1 < 0xE0) {
      b2 = *s++;
      if ((b2 & 0xC0) != 0x80) return -1;
    } else if (b1 < 0xF0) {
      b2 = *s++;
      if ((b2 & 0xC0) != 0x80) return -1;
      if (b1 == 0xE0 && b2 < 0xA0) return -1;
      b3 = *s++;
      if ((b3 & 0xC0) != 0x80) return -1;
    } else if (b1 < 0xF5) {
      b2 = *s++;
      if ((b2 & 0xC0) != 0x80) return -1;
      if (b1 == 0xF0 && b2 < 0x90) return -1;
      if (b1 == 0xF4 && b2 >= 0x90) return -1;
      b3 = *s++;
      if ((b3 & 0xC0) != 0x80) return -1;
      b4 = *s++;
      if ((b4 & 0xC0) != 0x80) return -1;
    } else return -1;
  }
  return r;
}

static char *error = "??";
static char *replace_error = "�";
static char *_str_next_rune(char *s) {
  unsigned char b1, b2, b3, b4;
  b1 = *s++;
  if (b1) {
    if (b1 < 0xC2) {
      return str_f("%c", b1);
    } else if (b1 < 0xE0) {
      b2 = *s++;
      if ((b2 & 0xC0) != 0x80) {
        return error;
      }
      return str_f("%c%c", b1, b2);
    } else if (b1 < 0xF0) {
      b2 = *s++;
      if ((b2 & 0xC0) != 0x80 ||
          (b1 == 0xE0 && b2 < 0xA0)
      ) {
        return error;
      }
      b3 = *s++;
      if ((b3 & 0xC0) != 0x80) {
        return error;
      }
      return str_f("%c%c%c", b1, b2, b3);
    } else if (b1 < 0xF5) {
      b2 = *s++;
      if (((b2 & 0xC0) != 0x80) ||
          (b1 == 0xF0 && b2 < 0x90) ||
          (b1 == 0xF4 && b2 >= 0x90)
      ) {
        return error;
      }
      b3 = *s++;
      if ((b3 & 0xC0) != 0x80) {
        return error;
      }
      b4 = *s++;
      if ((b4 & 0xC0) != 0x80) {
        return error;
      }
      return str_f("%c%c%c%c", b1, b2, b3, b4);
    } else {
      return error;
    }
  }
  return "";
}

char *str_next_rune(char **rune, char *s) {
  char *r = _str_next_rune(s);
  if (r[0] == '?' && r[1] == '?') {
    *rune = str_new(replace_error);
    return s + 1;
  }
  *rune = r;
  return s + strlen(r);
}

Arr *str_runes(char *s) {
  //<char>
  Arr *r = arr_new();
  char *rune;
  while (*s) {
    s = str_next_rune(&rune, s);
    arr_push(r, rune);
  }
  return r;
}

// Opt<unsigned>
Opt *str_to_unicode(char *s0) {
  unsigned char *s = (unsigned char *)s0;
  unsigned b1, b2, b3, b4;
  int lg = str_runes_len(s0) + 1;
  unsigned *r = ATOMIC(lg * sizeof(unsigned));
  unsigned *pr = r;
  while (*s) {
    b1 = *s++;
    if (b1 < 0x80) *pr++ = b1;
    else if (b1 < 0xC2) return opt_none();
    else if (b1 < 0xE0) {
      b2 = *s++;
      if ((b2 & 0xC0) != 0x80) return opt_none();
      *pr++ = (b1 << 6) + b2 - 0x3080;
    } else if (b1 < 0xF0) {
      b2 = *s++;
      if ((b2 & 0xC0) != 0x80) return opt_none();
      if (b1 == 0xE0 && b2 < 0xA0) return opt_none();
      b3 = *s++;
      if ((b3 & 0xC0) != 0x80) return opt_none();
      *pr++ = (b1 << 12) + (b2 << 6) + b3 - 0xE2080;
    } else if (b1 < 0xF5) {
      b2 = *s++;
      if ((b2 & 0xC0) != 0x80) return opt_none();
      if (b1 == 0xF0 && b2 < 0x90) return opt_none();
      if (b1 == 0xF4 && b2 >= 0x90) return opt_none();
      b3 = *s++;
      if ((b3 & 0xC0) != 0x80) return opt_none();
      b4 = *s++;
      if ((b4 & 0xC0) != 0x80) return opt_none();
      *pr++ = (b1 << 18) + (b2 << 12) + (b3 << 6) + b4 - 0x3C82080;
    } else {
      return opt_none();
    }
  }
  *pr = 0;
  return opt_some(r);
}

// Opt<char>
Opt *str_from_unicode(unsigned *u) {
  Buf *bf = buf_new();

  while (*u) {
    unsigned code_point = *u++;
    if (code_point < 0x80) {
      buf_cadd(bf, (unsigned char)code_point);
    } else if (code_point <= 0x7FF) {
      buf_cadd(bf, (unsigned char)(code_point >> 6) + 0xC0);
      buf_cadd(bf, (unsigned char)(code_point & 0x3F) + 0x80);
    } else if (code_point <= 0xFFFF) {
      buf_cadd(bf, (unsigned char)(code_point >> 12) + 0xE0);
      buf_cadd(bf, (unsigned char)((code_point >> 6) & 0x3F) + 0x80);
      buf_cadd(bf, (unsigned char)(code_point & 0x3F) + 0x80);
    } else if (code_point <= 0x10FFFF) {
      buf_cadd(bf, (unsigned char)(code_point >> 18) + 0xF0);
      buf_cadd(bf, (unsigned char)((code_point >> 12) & 0x3F) + 0x80);
      buf_cadd(bf, (unsigned char)((code_point >> 6) & 0x3F) + 0x80);
      buf_cadd(bf, (unsigned char)(code_point & 0x3F) + 0x80);
    } else {
      return opt_none();
    }
  }

  return opt_some(str_new(buf_str(bf)));
}

char *str_from_iso(char *s) {
  Buf *bf = buf_new();
  unsigned char ch = *s++;
  while (ch) {
    if(ch < 0x80) {
      buf_cadd(bf, ch);
    } else {
      buf_cadd(bf, 0xc0 | (ch & 0xc0) >> 6);
      buf_cadd(bf, 0x80 | (ch & 0x3f));
    }
    ch = *s++;
  }
  return str_new(buf_str(bf));
}

char *str_to_upper (char *s) {
  unsigned *ws = (unsigned *)opt_get(str_to_unicode(s));
  if (!ws) EXC_ILLEGAL_ARGUMENT("Bad string", "Valid utf8 string", "Wrong value");

  unsigned *p = ws;
  while (*p) {
    *p = (unsigned)towupper(*p);
    ++p;
  }

  char *r = (char *)opt_get(str_from_unicode(ws));
  if (!r) EXC_ILLEGAL_ARGUMENT("Interal error", "Valid Unicode wchar", "Wrong value");
  return r;
}

char *str_to_lower (char *s) {
  unsigned *ws = (unsigned *)opt_get(str_to_unicode(s));
  if (!ws) EXC_ILLEGAL_ARGUMENT("Bad string", "Valid utf8 string", "Wrong value");

  unsigned *p = ws;
  while (*p) {
    *p = (unsigned)towlower(*p);
    ++p;
  }

  char *r = (char *)opt_get(str_from_unicode(ws));
  if (!r) EXC_ILLEGAL_ARGUMENT("Internal error", "Valid Unicode wchar", "Wrong value");
  return r;
}

char *str_to_escape (char *s) {
  Buf *bf = buf_new();
  buf_cadd(bf, '"');
  while (*s) {
    char ch = *s++;
    if (ch == '"' || ch == '\\') {
      buf_cadd(bf, '\\');
    }
    buf_cadd(bf, ch);
  }
  buf_cadd(bf, '"');

  return str_new(buf_str(bf));
}

char *str_from_escape (char *s) {
  int len = strlen(s);
  if (len < 2) {
    return str_new(s);
  }

  ++s;
  Buf *bf = buf_new();
  while (*s) {
    char ch = *s++;
    if (ch != '\\') {
      buf_cadd(bf, ch);
    } else {
      buf_cadd(bf, *s++);
    }
  }

  return str_left(buf_str(bf), buf_len(bf) - 1);
}

