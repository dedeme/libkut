// Copyright 23-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "kut/kv.h"
#include "kut/DEFS.h"
#include "kut/js.h"

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

char *kv_to_js (Kv *this, char *(*to)(void *e)) {
  return js_wa(arr_new_from(
    js_ws(this->key),
    to(this->value),
    NULL
  ));
}

Kv *kv_from_js (char *js, void *(*from)(char *ejs)) {
  // <char>
  Arr *a = js_ra(js);
  return kv_new(
    js_rs(arr_get(a, 0)),
    from(arr_get(a, 1))
  );
}
