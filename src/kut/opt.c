// Copyright 21-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "kut/opt.h"
#include "kut/DEFS.h"
#include "kut/js.h"

struct opt_Opt {
  void *value;
};

Opt *opt_some (void *value) {
  Opt *this = MALLOC(Opt);
  this->value = value;
  return this;
}

Opt *opt_none (void) {
  Opt *this = MALLOC(Opt);
  this->value = NULL;
  return this;
}

void *opt_get (Opt *this) {
  return this->value;
}

void *opt_eget (Opt *this) {
  if (!this->value)
    EXC_ILLEGAL_STATE("Option is null");

  return this->value;
}

char *opt_to_js (Opt *this, char *(*to)(void *e)) {
  if (this->value) return to(this->value);
  return js_wn();
}


Opt *opt_from_js (char *js, void *(*from)(char *jse)) {
  if (js_is_null(js)) return opt_none();
  return opt_some(from(js));
}
