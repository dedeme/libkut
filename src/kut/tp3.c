// Copyright 23-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "kut/tp3.h"
#include "kut/DEFS.h"

struct tp3_Tp3 {
  void *e1;
  void *e2;
  void *e3;
};

Tp3 *tp3_new (void *e1, void *e2, void *e3) {
  Tp3 *this = MALLOC(Tp3);
  this->e1 = e1;
  this->e2 = e2;
  this->e3 = e3;
  return this;
}

void *tp3_e1 (Tp3 *this) {
  return this->e1;
}

void *tp3_e2 (Tp3 *this) {
  return this->e2;
}

void *tp3_e3 (Tp3 *this) {
  return this->e3;
}
