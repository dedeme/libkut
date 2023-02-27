// Copyright 24-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "path_tests.h"
#include "kut/DEFS.h"
#include "kut/js.h"
#include "kut/sys.h"
#include "kut/opt.h"
#include "kut/rs.h"

void js_tests(void) {
  puts(">>> js:");

  char *s;
  char *str;

  assert(js_is_null("null"));
  assert(js_is_null("  null   "));
  assert(!js_is_null(""));
  assert(!js_is_null("nuln"));
  s = js_wn();
  assert(str_eq((char *)s, "null"));
  assert(js_rb(" true "));
  assert(js_rb(" false") == 0);

//  js_rb("true and"); // error
//  js_rb("xx"); // error

  s = js_wb(1);
  assert(js_rb(s));
  s = js_wb(0);
  assert(!js_rb(s));

  assert(js_ri(" 0 ") == 0);
  assert(js_rd(" 0.23 ") == 0.23);
  assert(js_ri(" -0 ") == 0);
  assert(js_rd(" -0.0 ") == 0);
  assert(js_rd(" -12.11 ") == -12.11);
  assert(js_rd(" -12.11e22 ") == -12.11e22);

//  js_ri(" 12abc "); // error
//  js_ri(" 12] "); // error
//  js_ri(" 12 }"); // error
//  js_rd(" .12"); // error
//  js_rd(" z.12"); // error

  s = js_wi(0);
  assert(js_ri(s) == 0);
  s = js_wi(254);
  assert(js_ri(s) == 254);
  s = js_wi(-1100);
  assert(js_ri(s) == -1100);
  s = js_wf(0.0, 0);
  assert(js_rd(s) == 0);
  s = js_wf(-0.0, 3);
  assert(js_rd(s) == 0);
  s = js_wf(-.0, 2);
  assert(js_rd(s) == 0);
  s = js_wf(1.045, 8);
  assert(js_rd(s) == 1.045);
  s = js_wf(-21.045, 3);
  assert(js_rd(s) == -21.045);
  s = js_wf(-21.04, 6);
  assert(js_rd(s) == -21.04);

  str = js_rs("  \"\" ");
  assert(str_eq("", str));
  str = js_rs("  \"a\\u0030\" ");
  assert(str_eq("a0", str));
  str = js_rs("  \"a\\t\\n\\\"\" ");
  assert(str_eq("a\t\n\"", str));

//  js_rs(" \""); // Error
//  js_rs(" \"a\" l"); // Error
//  js_rs(" \" \\ \" "); // Error
//  js_rs(" \" \\@ \" "); // Error
//  js_rs(" \" \\u30 \" "); // Error

  // <char>
  Arr *a, *a2;
  a = js_ra("[]");
  assert(arr_size(a) == 0);
  a = js_ra("[123]");
  assert(arr_size(a) == 1);
  double rsd = js_rd(arr_get(a, 0));
  assert(rsd == 123);
  a = js_ra("[-123.56, true]");
  assert(arr_size(a) == 2);
  rsd = js_rd(arr_get(a, 0));
  assert(rsd == -123.56);
  int rs = js_rb(arr_get(a, 1));
  assert(rs == 1);
  a = js_ra("[-123.56, true, \"a\"]");
  assert(arr_size(a) == 3);
  str = js_rs(arr_get(a, 2));
  assert(str_eq("a", str));

  a = js_ra("[-123.56, true, [], 56]");
  assert(arr_size(a) == 4);
  rsd = js_rd(arr_get(a, 3));
  assert(rsd == 56);
  a2 = js_ra(arr_get(a, 2));
  assert(arr_size(a2) == 0);

  a = js_ra(" [-123.56, true, [\"azf\", false], 56]  ");
  assert(arr_size(a) == 4);
  rsd = js_rd(arr_get(a, 3));
  assert(rsd == 56);
  a2 = js_ra(arr_get(a, 2));
  assert(arr_size(a2) == 2);
  rs = js_rb(arr_get(a2, 1));
  assert(rs == 0);
  str = js_rs(arr_get(a2, 0));
  assert(str_eq("azf", str));

//  js_ra("[-123.56, true, [], 56] h"); // Error
//  js_ra("[s123.56, true, [], 56] "); // Error
//  js_ra(" "); // Error
//  js_ra("[-123.56, true, [], true   "); // Error

  // Arr<char>, Arr<char>
  int arr_eq_str(Arr *a1, Arr *a2) {
    int size = arr_size(a1);
    if (size != arr_size(a2)) {
      return 0;
    }
    for (int i = 0; i < size; ++i) {
      if (!str_eq(arr_get(a1, i), arr_get(a2, i))) {
        return 0;
      }
    }
    return 1;
  }

  // <char>
  Arr *a3;
  a = arr_new();
  s = js_wa(a);
  a3 = js_ra(s);
  assert(arr_eq_str(a, a3));

  arr_push(a, js_wb(1));
  s = js_wa(a);
  a3 = js_ra(s);
  assert(arr_eq_str(a, a3));

  arr_push(a, js_wi(-16));
  s = js_wa(a);
  a3 = js_ra(s);
  assert(arr_eq_str(a, a3));

  arr_push(a, js_wf(1, 2));
  s = js_wa(a);
  a3 = js_ra(s);
  assert(arr_eq_str(a, a3));

  arr_push(a, js_ws("caf"));
  s = js_wa(a);
  a3 = js_ra(s);
  assert(arr_eq_str(a, a3));

  a2 = arr_new();
  arr_push(a, js_wa(a2));
  s = js_wa(a);
  a3 = js_ra(s);
  assert(arr_eq_str(a, a3));

  arr_push(a2, js_ws("a\n\tzzð"));
  arr_push(a, js_wa(a2));
  s = js_wa(a);
  a3 = js_ra(s);
  assert(arr_eq_str(a, a3));

  // <char>
  Map *m;
  m = js_ro("{}");
  assert(map_size(m) == 0);

  m = js_ro(" {\"a\":123 } ");
  assert(map_size(m) == 1);
  rsd = js_rd(opt_get(map_get(m, "a")));
  assert(rsd == 123);

  m = js_ro(" {\"a\":123, \"b\":true } ");
  assert(map_size(m) == 2);
  rs = js_rb(opt_get(map_get(m, "b")));
  assert(rs);

  m = js_ro(" {\"a\":123, \"a\":true } ");
  assert(map_size(m) == 1);
  rs =js_rb(opt_get(map_get(m, "a")));
  assert(rs);

  // Map<char>, Map<char>
  int map_eq_str(Map *m1, Map *m2) {
    int size = map_size(m1);
    if (size != map_size(m2)) {
      return 0;
    }

    for (int i = 0; i < size; ++i) {
      // <char>
      Kv *kv1 = arr_get(map_to_array(m1), i);
      // <char>
      Kv *kv2 = arr_get(map_to_array(m2), i);
      if (
        !str_eq(kv_key(kv1), kv_key(kv2)) ||
        !str_eq(kv_value(kv1), kv_value(kv2))
      ) {
        return 0;
      }
    }
    return 1;
  }

  // <char>
  Map *m2;
  m = map_new();
  s = js_wo(m);
  m2 = js_ro(s);
  assert(map_eq_str(m, m2));

  map_put(m, "A", js_wb(0));
  s = js_wo(m);
  m2 = js_ro(s);
  assert(map_eq_str(m, m2));

  map_put(m, "A", js_wb(0));
  s = js_wo(m);
  m2 = js_ro(s);
  assert(map_eq_str(m, m2));

  map_put(m, "B", js_wi(-34516));
  s = js_wo(m);
  m2 = js_ro(s);
  assert(map_eq_str(m, m2));

  map_put(m, "C", js_wf(321.19, 2));
  s = js_wo(m);
  m2 = js_ro(s);
  assert(map_eq_str(m, m2));

  map_put(m, "D", js_ws("caf"));
  s = js_wo(m);
  m2 = js_ro(s);
  assert(map_eq_str(m, m2));

  map_put(m, "F", js_wo(m));
  s = js_wo(m);
  m2 = js_ro(s);
  assert(map_eq_str(m, m2));

  sys_set_locale("es_ES.utf8");
  TEST(str_f("%.2f", 1234.34), "1234,34");
  TEST(js_wf(1234.34, 2), "1234.34");
  sys_set_locale("C");
  TEST(str_f("%.2f", 1234.34), "1234.34");

  // Opt and Rs

  assert(!opt_get(opt_from_js(opt_to_js(opt_none(), (FTO)js_ws), (FFROM)js_rs)));
  TEST(
    opt_get(opt_from_js(opt_to_js(opt_some("A"), (FTO)js_ws), (FFROM)js_rs)),
    "A"
  );

  TEST(
    rs_get(rs_from_js(rs_to_js(rs_ok("A"), (FTO)js_ws), (FFROM)js_rs)),
    "A"
  );
  TEST(
    rs_error(rs_from_js(rs_to_js(rs_ok("A"), (FTO)js_ws), (FFROM)js_rs)),
    ""
  );
  TEST(
    rs_error(rs_from_js(rs_to_js(rs_fail("A"), (FTO)js_ws), (FFROM)js_rs)),
    "A"
  );

  puts("... Finished");
}
