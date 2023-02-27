// Copyright 23-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "kut/rs.h"
#include "kut/DEFS.h"
#include "kut/js.h"

struct rs_Rs {
  char *error;
  void *value;
};

Rs *rs_ok (void *value) {
  Rs *this = MALLOC(Rs);
  this->error = "";
  this->value = value;
  return this;
}

Rs *rs_fail (char *msg) {
  Rs *this = MALLOC(Rs);
  this->error = msg;
  this->value = NULL;
  return this;
}

void *rs_get (Rs *this) {
  return this->value;
}

char *rs_error (Rs *this) {
  return this->error;
}

char *rs_to_js (Rs *this, char *(*to)(void *e)) {
  return (*this->error)
    ? js_wa(arr_new_from(
        js_ws(this->error),
        js_wn(),
        NULL
      ))
    : js_wa(arr_new_from(
        js_ws(""),
        to(this->value),
        NULL
      ));
  ;
}

Rs *rs_from_js (char *js, void *(*from)(char *jse)) {
  // <char>
  Arr *a = js_ra(js);
  char *error = js_rs(arr_get(a, 0));
  return (*error) ? rs_fail(error) : rs_ok(from(arr_get(a, 1)));
}
