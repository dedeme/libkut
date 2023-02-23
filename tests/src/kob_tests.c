// Copyright 20-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "kut/kob.h"
#include "kut/DEFS.h"
#include "kut/arr.h"
#include <string.h>
#include <locale.h>
#include "kut/str.h"

void kob_tests(void) {
  puts(">>> kob:");

  Kob *b = kob_bool(TRUE);
  TESTI(kob_get_bool(b), TRUE);
  TEST(kob_type_to_str(kob_BOOL), "Bool");

  Kob *i = kob_int(-123);
  TESTI(kob_get_int(i), -123);
  TEST(kob_type_to_str(kob_INT), "Int");

  Kob *f = kob_float(-123.12);
  TESTF(kob_get_float(f), -123.120000);
  TEST(kob_type_to_str(kob_FLOAT), "Float");

  Kob *s = kob_str("hello");
  TEST(kob_get_str(s), "hello");
  TEST(kob_type_to_str(kob_INT), "Int");

  Kob *a = kob_arr(arr_new());
  TESTI(arr_size(kob_get_arr(a)), 0);
  TEST(kob_type_to_str(kob_ARRAY), "Array");

  puts("... Finished");
}
