// Copyright 22-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "b64_tests.h"
#include "kut/DEFS.h"
#include "kut/b64.h"

void b64_tests(void) {
  puts(">>> b64:");

  char *b64 = b64_encode("Cañónç䍆");
  assert(!strcmp(b64, "Q2HDscOzbsOn5I2G"));

  b64 = b64_encode("");
  assert(!strcmp(b64, ""));

  b64 = b64_encode("Cañónç䍆");
  char *s = b64_decode(b64);
  TEST(s, "Cañónç䍆");

  b64 = b64_encode("");
  s = b64_decode(b64);
  assert(!strcmp(s, ""));

  for (int len = 120; len < 130; ++len) {
    Bytes *bs = bytes_new_bf(len);
    unsigned char *bss = bytes_bs(bs);
    for (int i = 0; i < len; ++i) {
      bss[i] = (unsigned char)i +  10;
    }
    b64 = b64_encode_bytes(bs);
    Bytes *bs2 = b64_decode_bytes(b64);
    unsigned char *bss2 = bytes_bs(bs2);
    for (int i = 0; i < len; ++i) {
      assert(bss[i] == bss2[i]);
    }
  }

  puts("... Finished");
}
