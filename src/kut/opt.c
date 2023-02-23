// Copyright 21-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "kut/opt.h"
#include "kut/DEFS.h"

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
    EXC_ILLEGAL_STATE("Option is null")

  return this->value;
}
