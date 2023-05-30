// Copyright 30-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "sys_tests.h"
#include "kut/sys.h"
#include "kut/DEFS.h"

void sys_tests(void) {
  puts(">>> sys:");
  assert(*sys_user_name());
  assert(*sys_user_home());
  //TESTI(str_len(sys_read_line()), 3);

  puts("... Finished");
}
