// Copyright 30-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "thread_tests.h"
#include "kut/thread.h"
#include "kut/DEFS.h"

static void blocked (void) {
    //--
    void fn (void) {
      return;
    }
  thread_sync(fn);
}

static void blocked2 (void) {
    //--
    void fn (void *value) {
      return;
    }
  thread_sync2(fn, NULL);
}

void thread_tests(void) {
  puts(">>> thread:");

    //--
    void fn (void) {
      blocked();
      blocked2();
    }
  thread_sync(fn);

  puts("... Finished");
}
