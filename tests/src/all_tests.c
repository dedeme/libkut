// Copyright 20-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <stdio.h>
#include "kut/sys.h"
#include "kob_tests.h"
#include "exc_tests.h"
#include "path_tests.h"
#include "str_tests.h"
#include "arr_tests.h"
#include "bytes_tests.h"
#include "buf_tests.h"
#include "b64_tests.h"
#include "cryp_tests.h"
#include "map_tests.h"

int main(int argc, char *argv[]) {
  sys_init();

  kob_tests ();
  exc_tests();
  path_tests();
  str_tests();
  arr_tests();
  bytes_tests();
  buf_tests();
  b64_tests();
  cryp_tests();
  map_tests();
}
