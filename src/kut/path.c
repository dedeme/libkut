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

  if (!*name1 || !*name2)
    EXC_GENERIC("'path_cat' requieres not empty strings.");

  Buf *bf = buf_new();
  buf_add(bf, name1);
  buf_cadd(bf, '/');
  buf_add(bf, name2);

  va_start(args, name2);
  tmp = va_arg(args, char *);
  while (tmp) {
    if (*tmp) {
      buf_cadd(bf, '/');
      buf_add(bf, tmp);
    } else {
      EXC_GENERIC("'path_cat' requieres not empty strings.");
    }
    tmp = va_arg(args, char *);
  }
  va_end(args);

  return str_new(buf_str(bf));
}

char *path_base (char *path) {
  path = normalize(path);
  int ix = str_last_cindex(path, '/');
  if (ix != -1) {
    return str_right(path, ix + 1);
  }
  return path;
}

char *path_parent (char *path) {
  path = normalize(path);
  int ix = str_last_cindex(path, '/');
  if (ix == -1) {
    ix = 0;
  }
  return str_left(path, ix);
}

char *path_extension (char *path) {
  path = path_base(path);
  int ix = str_last_cindex(path, '.');
  if (ix == -1) {
    ix = str_len(path);
  }
  return str_right(path, ix);
}

Opt *path_canonical (char *s) {
  char *tmp = realpath(s, NULL);
  Opt *r = tmp ? opt_some(str_new(tmp)) : opt_none();
  free(tmp);
  return r;
}
