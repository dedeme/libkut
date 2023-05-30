// Copyright 23-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "path_tests.h"
#include "kut/DEFS.h"
#include "kut/map.h"
#include "kut/sys.h"
#include "kut/js.h"

void map_tests(void) {
  puts(">>> map:");

  // <char>
  Map *m = map_new();
  TESTI(map_size(m), 0);

  map_put(m, "uno", "2");
  TESTI(map_size(m), 1);
  TEST(opt_get(map_get(m, "uno")), "2");
  map_add(m, "dos", "2");
  TEST(opt_get(map_get(m, "dos")), "2");
  map_set(m, "uno", str_new("1"));
  TEST(opt_get(map_get(m, "uno")), "1");
  TESTI(map_size(m), 2);
  assert(!opt_get(map_get(m, "tres")));
  map_put(m, "tres", "3");

  map_remove(m, "dos");
  TEST(opt_get(map_get(m, "uno")), "1");
  TEST(opt_get(map_get(m, "tres")), "3");
  assert(!opt_get(map_get(m, "dos")));
  TESTI(map_size(m), 2);

  map_remove(m, "cero");
  TEST(opt_get(map_get(m, "uno")), "1");
  TEST(opt_get(map_get(m, "tres")), "3");
  assert(!opt_get(map_get(m, "dos")));
  TESTI(map_size(m), 2);

  // <char>
  Arr *keys = map_keys(m);
  TEST(arr_join(keys, "-"), "uno-tres");

  arr_sort(keys, (FEQ)str_greater);
  TEST(arr_join(keys, "-"), "tres-uno");

  // <Kv>
  Arr *am = map_to_array(m);
  arr_reverse(am);
  m = map_from_array(am);
  keys = map_keys(m);
  TEST(arr_join(keys, "-"), "tres-uno");
  arr_reverse(am);
  m = map_from_array(am);
  keys = map_keys(m);
  TEST(arr_join(keys, "-"), "uno-tres");

  map_put(m, "él", "2");

  map_sort(m);
  keys = map_keys(m);
  TEST(arr_join(keys, "-"), "tres-uno-él");

  sys_set_locale("es_ES.utf8");
  map_sort_locale(m);
  sys_set_locale("C");
  keys = map_keys(m);
  TEST(arr_join(keys, "-"), "él-tres-uno");

  TESTI(map_size(
    map_from_js(map_to_js(map_new(), (FTO)js_ws), (FFROM)js_rs)), 0);

  Map *m2 = map_from_js(map_to_js(m, (FTO)js_ws), (FFROM)js_rs);
  TEST(opt_get(map_get(m2, "uno")), "1");
  TEST(opt_get(map_get(m2, "él")), "2");
  TEST(opt_get(map_get(m2, "tres")), "3");

  puts("... Finished");
}
