// Copyright 24-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "math_tests.h"
#include "kut/math.h"
#include "kut/DEFS.h"
#include "kut/sys.h"

void math_tests(void) {
  puts(">>> dec:");

  TEST(math_itos(-25), "-25");
  TEST(math_itos(0), "0");
  TEST(math_itos(32), "32");

  TEST(math_ftos(-3425.0, 0), "-3425");
  TEST(math_ftos(-3425.23, 0), "-3425");
  TEST(math_ftos(-3425.0, 2), "-3425");
  TEST(math_ftos(-3425.23, 2), "-3425.23");
  TEST(math_ftos(-3425.0, 5), "-3425");
  TEST(math_ftos(-3425.23, 5), "-3425.23");
  TEST(math_ftos(0.0, 0), "0");
  TEST(math_ftos(0.23, 0), "0");
  TEST(math_ftos(-0.0, 2), "0");
  TEST(math_ftos(-0.23, 2), "-0.23");
  TEST(math_ftos(0, 5), "0");
  TEST(math_ftos(0.23, 5), "0.23");
  TEST(math_ftos(3425.0, 0), "3425");
  TEST(math_ftos(3425.23, 0), "3425");
  TEST(math_ftos(3425.0, 2), "3425");
  TEST(math_ftos(3425.23, 2), "3425.23");
  TEST(math_ftos(3425.0, 5), "3425");
  TEST(math_ftos(3425.23, 5), "3425.23");
  TEST(math_ftos(3000.0, 0), "3000");
  TEST(math_ftos(1235.0e-2, 1), "12.4");

  sys_set_locale("es_ES.utf8");
  TEST(math_to_locale(-3425.0, 0), "-3.425");
  TEST(math_to_locale(-3425.23, 0), "-3.425");
  TEST(math_to_locale(-3425.0, 2), "-3.425,00");
  TEST(math_to_locale(-3425.23, 2), "-3.425,23");
  TEST(math_to_locale(-3425.0, 5), "-3.425,00000");
  TEST(math_to_locale(-3425.23, 5), "-3.425,23000");
  TEST(math_to_locale(0.0, 0), "0");
  TEST(math_to_locale(0.23, 0), "0");
  TEST(math_to_locale(-0.0, 2), "0,00");
  TEST(math_to_locale(-0.23, 2), "0,23");
  TEST(math_to_locale(0, 5), "0,00000");
  TEST(math_to_locale(0.23, 5), "0,23000");
  TEST(math_to_locale(3425.0, 0), "3.425");
  TEST(math_to_locale(3425.23, 0), "3.425");
  TEST(math_to_locale(3425.0, 2), "3.425,00");
  TEST(math_to_locale(3425.23, 2), "3.425,23");
  TEST(math_to_locale(3425.0, 5), "3.425,00000");
  TEST(math_to_locale(3425.23, 5), "3.425,23000");
  sys_set_locale("C");

  TESTI(math_stoi("-25"), -25);
  TESTI(math_stoi("0"), 0);
  TESTI(math_stoi("25"), 25);
  TESTI(math_stoi("+25"), 25);

  TESTI(math_stoi("0XF"), 15);
  TESTI(math_stoi("0Xff"), 255);
  TESTI(math_stoi("0XfFfF"), 65535);

  TESTF(math_stod("-3425"), -3425);
  TESTF(math_stod("-3425.23"), -3425.23);
  assert(math_eq(math_stod("-0"), 0));
  TESTF(math_stod("0.23"), 0.23);
  TESTF(math_stod("+3425"), 3425);
  TESTF(math_stod("3425.2300"), 3425.23);

  TESTF(math_from_en("-3,425"), -3425);
  TESTF(math_from_en("-3425.23"), -3425.23);
  assert(math_eq(math_from_en("-0"), 0));
  TESTF(math_from_en("0.23"), 0.23);
  TESTF(math_from_en("+3425"), 3425);
  TESTF(math_from_en("3,425.23"), 3425.23);

  TESTF(math_from_iso("-3.425"), -3425);
  TESTF(math_from_iso("-3425,23"), -3425.23);
  assert(math_eq(math_from_iso("-0"), 0));
  TESTF(math_from_iso("0,23"), 0.23);
  TESTF(math_from_iso("+3425"), 3425);
  TESTF(math_from_iso("3.425,23"), 3425.23);

  // TESTI(math_stoi("+2r5"), 0); // Fail
  // TESTI(math_stoi(""), 0); // Fail
  // TESTF(math_stod(""), 0.0); // Fail
  // TESTF(math_stod("gg"), 0.0); // Fail

  assert (math_digits("0123456789"));
  assert (!math_digits(""));
  assert (!math_digits("a0123456789"));
  assert (!math_digits("0123 456789"));
  assert (!math_digits("0123456789c"));

  TESTF (math_round(-3425, 0), -3425);
  TESTF (math_round(-3425.23, 0), -3425);
  TESTF (math_round(-3425, 2), -3425);
  TESTF (math_round(-3425.23, 2), -3425.23);
  TESTF (math_round(-3425.23, 3), -3425.23);
  TESTF (math_round(0, 0), 0);
  TESTF (math_round(0, 2), 0);
  TESTF (math_round(-3425.5, 0), -3426);
  TESTF (math_round(-3425.235, 0), -3425);
  TESTF (math_round(-3425.5, 2), -3425.5);
  TESTF (math_round(-3425.235, 2), -3425.24);
  TESTF (math_round(3425.5, 0), 3426);
  TESTF (math_round(3425.235, 0), 3425);
  TESTF (math_round(3425.5, 2), 3425.5);
  TESTF (math_round(3425.235, 2), 3425.24);
  TESTF (math_round(-145.575, 2), -145.58);

  assert (math_eq(-0, 0));
  assert (math_eq(234, 234));
  assert (math_eq(-234.876, -234.876));
  assert (!math_eq(-0, 1));
  assert (!math_eq(234, 235));
  assert (!math_eq(-234.876, -234.877));

  TESTF (math_abs(-0), 0);
  TESTF (math_abs(-133.12), 133.12);
  TESTF (math_abs(133.12), 133.12);

  TESTF (math_pow(5674, 0.5), math_sqrt(5674));
  TESTF (math_pow(-5674.23, 0.5), math_sqrt(-5674.23));

  puts("... Finished");
}
