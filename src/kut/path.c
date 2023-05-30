// Copyright 21-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <stdlib.h>
#include "kut/path.h"
#include "kut/DEFS.h"
#include "kut/str.h"
#include "kut/buf.h"

static char *normalize(char *path) {
  char *r = str_trim(path);
  while (str_ends(r, "/")) r = str_left(r, -1);
  return r;
}

char *path_cat (char *name1, char *name2, ...) {
  va_list args;
  char *tmp;

  if (!name1 || !name2)
    EXC_GENERIC("'path_cat' requieres at least 2 arguments.");

  Buf *bf = buf_new();
  buf_add(bf, name1);
  if (*buf_str(bf)) buf_cadd(bf, '/');
  buf_add(bf, name2);

  va_start(args, name2);
  tmp = va_arg(args, char *);
  while (tmp) {
    if (*buf_str(bf)) buf_cadd(bf, '/');
    buf_add(bf, tmp);
    tmp = va_arg(args, char *);
  }
  va_end(args);

  return str_new(buf_str(bf));
}

char *path_base (char *path) {
  path = normalize(path);
  int ix = str_last_cindex(path, '/');
  if (ix != -1) return str_right(path, ix + 1);
  return path;
}

char *path_parent (char *path) {
  char *s = normalize(path);
  if (!*s) EXC_GENERIC(str_f("'%s' has not parent directory", path));
  int ix = str_last_cindex(s, '/');
  if (ix == -1) return "";
  if (ix == 0) return "/";
  return str_left(s, ix);
}

char *path_extension (char *path) {
  path = path_base(path);
  int ix = str_last_cindex(path, '.');
  if (ix == -1) {
    ix = str_len(path);
  }
  return str_right(path, ix);
}

char *path_clean (char *path) {
  char *s = str_trim(path);
  if (!*s) return "";
  Buf *bf = buf_new();
  int isSlash = 0;
  while (*s) {
    char ch = *s++;
    if (ch == '/') {
      if (isSlash) continue;
      isSlash = 1;
      buf_cadd(bf, ch);
      continue;
    }
    isSlash = 0;
    buf_cadd(bf, ch);
  }
  s = buf_str(bf);
  if (!s[1]) return s;
  if (str_ends(s, "/")) s = str_left(s, -1);
  //<char>
  Arr *new = arr_new();
  EACH(str_csplit(s, '/'), char, part) {
    if (!strcmp(part, ".")) continue;
    if (!strcmp(part, "..")) {
      if (arr_size(new)) arr_pop(new);
      else EXC_GENERIC(str_f("Bad path for cleaning '%s'", path));
      continue;
    }
    arr_push(new, part);
  }_EACH
  s = arr_cjoin(new, '/');
  if (!*s && *path == '/')
    EXC_GENERIC(str_f("Bad path for cleaning '%s'", path));

  return s;
}

Opt *path_canonical (char *s) {
  char *tmp = realpath(s, NULL);
  Opt *r = tmp ? opt_some(str_new(tmp)) : opt_none();
  free(tmp);
  return r;
}
