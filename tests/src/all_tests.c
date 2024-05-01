// Copyright 20-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>
#include <locale.h>
#include <stdio.h>
#include "kut/sys.h"
#include "exc_tests.h"
#include "path_tests.h"
#include "str_tests.h"
#include "arr_tests.h"
#include "bytes_tests.h"
#include "buf_tests.h"
#include "b64_tests.h"
#include "cryp_tests.h"
#include "map_tests.h"
#include "js_tests.h"
#include "dec_tests.h"
#include "time_tests.h"
#include "file_tests.h"
#include "regex_tests.h"
#include "it_tests.h"
#include "thread_tests.h"
#include "sys_tests.h"

int main(int argc, char *argv[]) {
  sys_init();

  exc_tests();
  path_tests();
  str_tests();
  arr_tests();
  bytes_tests();
  buf_tests();
  b64_tests();
  cryp_tests();
  map_tests();
  js_tests();
  dec_tests();
  time_tests();
  file_tests();
  regex_tests();
  it_tests();
  thread_tests();
  sys_tests();
}
