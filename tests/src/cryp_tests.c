// Copyright 22-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "cryp_tests.h"
#include "kut/DEFS.h"
#include "kut/cryp.h"

void cryp_tests(void) {
  puts(">>> cryp:");

  char *b64;

  b64 = cryp_genk(6);
  TESTI(strlen(b64), 6);

  b64 = cryp_key("deme", 6);
  TEST(b64, "wiWTB9");
  b64 = cryp_key("Generaro", 5);
  TEST(b64, "Ixy8I");
  b64 = cryp_key("Generara", 5);
  TEST(b64, "0DIih");

  b64 = cryp_encode("abc", "01");
  assert(!str_eq(b64, "01"));
  b64 = cryp_decode("abc", b64);
  TEST(b64, "01");
  b64 = cryp_encode("abcd", "11");
  assert(!str_eq(b64, "11"));
  b64 = cryp_decode("abcd", b64);
  TEST(b64, "11");
  b64 = cryp_encode("abc", "");
  TEST(b64, "");
  b64 = cryp_decode("abc", b64);
  TEST(b64, "");
  b64 = cryp_encode("c", "a");
  assert(!str_eq(b64, "a"));
  b64 = cryp_decode("c", b64);
  TEST(b64, "a");
  b64 = cryp_encode("xxx", "ab c");
  assert(!str_eq(b64, "ab c"));
  b64 = cryp_decode("xxx", b64);
  TEST(b64, "ab c");
  b64 = cryp_encode("abc", "\n\ta€b c");
  assert(!str_eq(b64, "\n\ta€b c"));
  b64 = cryp_decode("abc", b64);
  TEST(b64, "\n\ta€b c");

  puts("... Finished");
}
