// Copyright 21-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <stdlib.h>
#include "kut/path.h"
#include "kut/str.h"

static char *normalize(char *path) {
  char *r = str_trim(path);
  while (str_ends(r, "/")) r = str_left(r, -1);
  return r;
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
