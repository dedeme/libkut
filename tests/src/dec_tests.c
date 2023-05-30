// Copyright 24-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "bytes_tests.h"
#include "kut/dec.h"
#include "kut/DEFS.h"
#include "kut/sys.h"

void dec_tests(void) {
  puts(">>> dec:");

  TEST(dec_itos(-25), "-25");
  TEST(dec_itos(0), "0");
  TEST(dec_itos(32), "32");

  TEST(dec_ftos(-3425.0, 0), "-3425");
  TEST(dec_ftos(-3425.23, 0), "-3425");
  TEST(dec_ftos(-3425.0, 2), "-3425");
  TEST(dec_ftos(-3425.23, 2), "-3425.23");
  TEST(dec_ftos(-3425.0, 5), "-3425");
  TEST(dec_ftos(-3425.23, 5), "-3425.23");
  TEST(dec_ftos(0.0, 0), "0");
  TEST(dec_ftos(0.23, 0), "0");
  TEST(dec_ftos(-0.0, 2), "0");
  TEST(dec_ftos(-0.23, 2), "-0.23");
  TEST(dec_ftos(0, 5), "0");
  TEST(dec_ftos(0.23, 5), "0.23");
  TEST(dec_ftos(3425.0, 0), "3425");
  TEST(dec_ftos(3425.23, 0), "3425");
  TEST(dec_ftos(3425.0, 2), "3425");
  TEST(dec_ftos(3425.23, 2), "3425.23");
  TEST(dec_ftos(3425.0, 5), "3425");
  TEST(dec_ftos(3425.23, 5), "3425.23");
  TEST(dec_ftos(3000.0, 0), "3000");
  TEST(dec_ftos(1235.0e-2, 1), "12.4");

  sys_set_locale("es_ES.utf8");
  TEST(dec_to_locale(-3425.0, 0), "-3.425");
  TEST(dec_to_locale(-3425.23, 0), "-3.425");
  TEST(dec_to_locale(-3425.0, 2), "-3.425,00");
  TEST(dec_to_locale(-3425.23, 2), "-3.425,23");
  TEST(dec_to_locale(-3425.0, 5), "-3.425,00000");
  TEST(dec_to_locale(-3425.23, 5), "-3.425,23000");
  TEST(dec_to_locale(0.0, 0), "0");
  TEST(dec_to_locale(0.23, 0), "0");
  TEST(dec_to_locale(-0.0, 2), "0,00");
  TEST(dec_to_locale(-0.23, 2), "0,23");
  TEST(dec_to_locale(0, 5), "0,00000");
  TEST(dec_to_locale(0.23, 5), "0,23000");
  TEST(dec_to_locale(3425.0, 0), "3.425");
  TEST(dec_to_locale(3425.23, 0), "3.425");
  TEST(dec_to_locale(3425.0, 2), "3.425,00");
  TEST(dec_to_locale(3425.23, 2), "3.425,23");
  TEST(dec_to_locale(3425.0, 5), "3.425,00000");
  TEST(dec_to_locale(3425.23, 5), "3.425,23000");
  sys_set_locale("C");

  TESTI(dec_stoi("-25"), -25);
  TESTI(dec_stoi("0"), 0);
  TESTI(dec_stoi("25"), 25);
  TESTI(dec_stoi("+25"), 25);

  TESTI(dec_stoi("0XF"), 15);
  TESTI(dec_stoi("0Xff"), 255);
  TESTI(dec_stoi("0XfFfF"), 65535);

  TESTF(dec_stod("-3425"), -3425);
  TESTF(dec_stod("-3425.23"), -3425.23);
  assert(dec_eq(dec_stod("-0"), 0));
  TESTF(dec_stod("0.23"), 0.23);
  TESTF(dec_stod("+3425"), 3425);
  TESTF(dec_stod("3425.2300"), 3425.23);

  TESTF(dec_from_en("-3,425"), -3425);
  TESTF(dec_from_en("-3425.23"), -3425.23);
  assert(dec_eq(dec_from_en("-0"), 0));
  TESTF(dec_from_en("0.23"), 0.23);
  TESTF(dec_from_en("+3425"), 3425);
  TESTF(dec_from_en("3,425.23"), 3425.23);

  TESTF(dec_from_iso("-3.425"), -3425);
  TESTF(dec_from_iso("-3425,23"), -3425.23);
  assert(dec_eq(dec_from_iso("-0"), 0));
  TESTF(dec_from_iso("0,23"), 0.23);
  TESTF(dec_from_iso("+3425"), 3425);
  TESTF(dec_from_iso("3.425,23"), 3425.23);

  // TESTI(dec_stoi("+2r5"), 0); // Fail
  // TESTI(dec_stoi(""), 0); // Fail
  // TESTF(dec_stod(""), 0.0); // Fail
  // TESTF(dec_stod("gg"), 0.0); // Fail

  assert (dec_digits("0123456789"));
  assert (!dec_digits(""));
  assert (!dec_digits("a0123456789"));
  assert (!dec_digits("0123 456789"));
  assert (!dec_digits("0123456789c"));

  TESTF (dec_round(-3425, 0), -3425);
  TESTF (dec_round(-3425.23, 0), -3425);
  TESTF (dec_round(-3425, 2), -3425);
  TESTF (dec_round(-3425.23, 2), -3425.23);
  TESTF (dec_round(-3425.23, 3), -3425.23);
  TESTF (dec_round(0, 0), 0);
  TESTF (dec_round(0, 2), 0);
  TESTF (dec_round(-3425.5, 0), -3426);
  TESTF (dec_round(-3425.235, 0), -3425);
  TESTF (dec_round(-3425.5, 2), -3425.5);
  TESTF (dec_round(-3425.235, 2), -3425.24);
  TESTF (dec_round(3425.5, 0), 3426);
  TESTF (dec_round(3425.235, 0), 3425);
  TESTF (dec_round(3425.5, 2), 3425.5);
  TESTF (dec_round(3425.235, 2), 3425.24);

  assert (dec_eq(-0, 0));
  assert (dec_eq(234, 234));
  assert (dec_eq(-234.876, -234.876));
  assert (!dec_eq(-0, 1));
  assert (!dec_eq(234, 235));
  assert (!dec_eq(-234.876, -234.877));

  TESTF (dec_abs(-0), 0);
  TESTF (dec_abs(-133.12), 133.12);
  TESTF (dec_abs(133.12), 133.12);

  TESTF (dec_pow(5674, 0.5), dec_sqrt(5674));
  TESTF (dec_pow(-5674.23, 0.5), dec_sqrt(-5674.23));

  puts("... Finished");
}
