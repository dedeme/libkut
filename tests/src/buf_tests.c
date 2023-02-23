// Copyright 22-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "bytes_tests.h"
#include "kut/DEFS.h"
#include "kut/buf.h"

void buf_tests(void) {
  puts(">>> buf:");

  char *s0 = "";
  char *s1 = "1";
  char *s2 = "abc";
  Buf *bf = buf_new();

  buf_add(bf, s0);
  buf_cadd(bf, '+');
  buf_add(bf, "0");
  buf_add(bf, s1);
  buf_add(bf, s2);
  buf_add(bf, s0);
  buf_add(bf, s2);
  buf_add(bf, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
  char *s = buf_str(bf);
  assert(str_eq(s, "+01abcabc"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));

  buf_reset(bf);
  s = buf_str(bf);
  assert(str_eq(s, ""));
  assert(!buf_len(bf));

  puts("... Finished");
}
