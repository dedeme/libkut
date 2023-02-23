// Copyright 23-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "kut/rs.h"
#include "kut/DEFS.h"

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

/// Throws a illegal_state_exception_t if 'this' is a failure.
void *rs_eget (Rs *this) {
  if (!this->value)
    EXC_ILLEGAL_STATE("Result is a faliure");
  return this->value;
}

/// Returns the error field.
/// If 'this' is ok returns "".
char *rs_error (Rs *this) {
  return this->error;
}
