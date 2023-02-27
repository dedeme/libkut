// Copyright 23-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "kut/map.h"
#include "kut/DEFS.h"
#include "kut/js.h"

Map *map_new(void) {
  return (Map *)arr_new();
}

int map_size(Map *this) {
  return arr_size((Arr *)this);
}

void map_put(Map *this, char *key, void *value) {
  int todo = TRUE;
  EACH((Arr *)this, Kv, kv) {
    if (!strcmp(kv_key(kv), key)) {
      arr_set((Arr *)this, _i, kv_new(key, value));
      todo = FALSE;
      break;
    }
  }_EACH

  if (todo) arr_push((Arr *)this, kv_new(key, value));
}

int map_has_key(Map *this, char *key) {
  EACH((Arr *)this, Kv, kv) {
    if (!strcmp(kv_key(kv), key)) return TRUE;
  }_EACH
  return FALSE;
}

Opt *map_get(Map *this, char *key) {
  EACH((Arr *)this, Kv, kv) {
    if (!strcmp(kv_key(kv), key)) return opt_some(kv_value(kv));
  }_EACH
  return opt_none();
}

void map_remove(Map *this, char *key) {
  int ix = -1;
  EACH((Arr *)this, Kv, kv) {
    if (!strcmp(kv_key(kv), key)) {
      ix = _i;
      break;
    }
  }_EACH

  if (ix != -1) arr_remove((Arr *)this, ix);
}

// <char>
Arr *map_keys(Map *this) {
  //(
    char *fmap (Kv *kv) {
      return kv_key(kv);
    }
  //)
  return arr_map((Arr *)this, (FMAP)fmap);
}

//<kv>
Arr *map_to_array(Map *this) {
  return arr_copy((Arr *)this);
}

// Arr<kv>
Map *map_from_array(Arr *this) {
  // <Kv>
  Arr *r = arr_copy(this);
  //(
    int feq (Kv *e1, Kv *e2) {
      return !str_cmp(kv_key(e1), kv_key(e2));
    }
  //)
  arr_duplicates(r, (FEQ)feq);
  return (Map *)r;
}

void map_sort(Map *this) {
  //(
    int greater(Kv *e1, Kv *e2) {
      return str_greater(kv_key(e1), kv_key(e2));
    }
  //)
  arr_sort((Arr *)this, (FEQ)greater);
}

void map_sort_locale(Map *this) {
  //(
    int greater(Kv *e1, Kv *e2) {
      return str_greater_locale(kv_key(e1), kv_key(e2));
    }
  //)
  arr_sort((Arr *)this, (FEQ)greater);
}

// this is Map<char>
char *map_to_js(Map *this, char *(*to)(void *e)) {
    //--
    char *fto (Kv *kv) { return kv_to_js(kv, to); }
  return js_wa(arr_map((Arr *)this, (FMAP)fto));
}

// <char>
Map *map_from_js(char *js, void *(*from)(char *jse)) {
    //-- Kv<char>
    Kv *ffrom (char *kv_js) { return kv_from_js(kv_js, from); }
  return (Map *)arr_map(js_ra(js), (FMAP)ffrom);
}
