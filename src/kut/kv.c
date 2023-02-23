// Copyright 23-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "kut/kv.h"
#include "kut/DEFS.h"

struct kv_Kv {
  char *key;
  void *value;
};

Kv *kv_new (char *key, void *value) {
  Kv *this = MALLOC(Kv);
  this->key = key;
  this->value = value;
  return this;
}

char *kv_key (Kv *this) {
  return this->key;
}

void *kv_value (Kv *this) {
  return this->value;
}
