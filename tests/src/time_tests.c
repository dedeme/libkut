// Copyright 25-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "arr_tests.h"
#include "kut/DEFS.h"
#include "kut/time.h"
#include "kut/sys.h"
#include <locale.h>
void time_tests(void) {
  puts(">>> time:");

  Time d = time_now();
  char *s = time_to_str(d);
  TESTI(strlen(s), 8);
  d = time_new(2, 3, 2010);
  s = time_to_str(d);
  TEST(s, "20100402");
  s = time_to_iso(d);
  TEST(s, "02/04/2010");
  d = time_from_str("19881231");
  s = time_to_str(d);
  TEST(s, "19881231");
  s = time_to_us(d);
  TEST(s, "12-31-1988");
  d = time_from_iso("01/02/2020");
  s = time_to_str(d);
  TEST(s, "20200201");
  d = time_from_us("02/01/2020");
  s = time_to_str(d);
  TEST(s, "20200201");

  Time d1 = time_new(29, 1, 2013);
  Time d2 = time_new(6, 2, 2013);
  Time d3 = time_new(30, 3, 2013);

  TESTI(time_df(d1, d2), -5);
  assert(time_df(d1, d2) < 0);
  TESTI(time_df(d3, d2), 55);
  assert(time_eq(d1, time_add(d2, -5)));
  assert(!time_df(d1, time_add(d2, -5)));
  TESTI(time_df(d3, time_add(d2, 55)), 0);
  assert(time_eq(d3, time_add(d2, 55)));

  assert(time_day(d1) == 1 && time_day(d2) == 6 &&time_day(d3) == 30);
  assert(time_month(d1) == 2 && time_month(d2) == 2 &&time_month(d3) == 3);
  TESTI(time_year(d1), 2013);
  assert(time_week_day(d1) == 5);

  // time_from_iso_sep("", '/'); // fail
  // time_from_iso_sep("123/34/34", '/'); // fail
  // time_from_iso_sep("23/34", '/'); // fail
  // time_from_iso_sep("23/34/23/34", '/'); // fail
  // time_from_iso_sep("23/34/234b", '/'); // fail
  // time_from_iso_sep("23c/34/234b", '/'); // fail
  // time_from_iso_sep("23/34x/234", '/'); // fail
  // time_from_iso_sep("02/1/153", '/'); // fail

  // time_from_us_sep("", '/'); // fail
  // time_from_us_sep("123/34/34", '/'); // fail
  // time_from_us_sep("23/34", '/'); // fail
  // time_from_us_sep("23/34/23/34", '/'); // fail
  // time_from_us_sep("23/34/234b", '/'); // fail
  // time_from_us_sep("23c/34/234b", '/'); // fail
  // time_from_us_sep("23/34x/234", '/'); // fail

  assert(time_eq(time_from_iso("01/02/2015"),
    time_from_iso_sep("1/2/2015", '/')));

  assert(time_eq(time_from_iso("01/02/15"),
    time_from_iso_sep("1/02/2015", '/')));

  assert(time_eq(time_from_us("02/01/2015"),
    time_from_us_sep("2/1/2015", '/')));

  assert(time_eq(time_from_us("02/01/2015"),
    time_from_us_sep("02/1/15", '/')));

  /*
  Time t = time_now();
  puts(str_f("%s,%d", time_f(t, "%d/%m/%Y-%H:%M:%S"), (int)(t % 1000)));
  sys_sleep(50);
  t = time_now();
  puts(str_f("%s,%d", time_f(t, "%d/%m/%Y-%H:%M:%S"), (int)(t % 1000)));
  */

  puts("... Finished");
}
