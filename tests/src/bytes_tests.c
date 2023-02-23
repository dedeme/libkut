// Copyright 20-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "bytes_tests.h"
#include "kut/DEFS.h"
#include "kut/bytes.h"

void bytes_tests(void) {
  puts(">>> bytes:");

  char *s1 = "ab";
  char *s2 = "c";
  char *s3 = "";

  Bytes *b1;
  Bytes *b2;

  b1 = bytes_new();
  assert(bytes_len(b1) == 0);

  b2 = bytes_from_str(s2);
  assert(bytes_len(b2) == 1);

  bytes_add_str(b1, s1);
  assert(bytes_len(b1) == 2);
  assert(*bytes_bs(b1) == 'a');
  assert(bytes_bs(b1)[1] == 'b');

  bytes_add_str(b1, s3);
  assert(bytes_len(b1) == 2);
  assert(*bytes_bs(b1) == 'a');
  assert(bytes_bs(b1)[1] == 'b');

  bytes_add(b1, b2);
  assert(bytes_len(b1) == 3);
  assert(*bytes_bs(b1) == 'a');
  assert(bytes_bs(b1)[1] == 'b');
  assert(bytes_bs(b1)[2] == 'c');

  b1 = bytes_new();
  bytes_add(b1, b2);
  assert(bytes_len(b1) == 1);
  assert(*bytes_bs(b1) == 'c');

  puts("... Finished");
}
