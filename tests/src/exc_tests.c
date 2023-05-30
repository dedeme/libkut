// Copyright 20-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "kut/thread.h"
#include "exc_tests.h"
#include "kut/DEFS.h"

void fn(void) {
  int v = -1;
  TRY
    v = 0;
    THROW(exc_generic_t, str_f("Value is %d", v));
    v = 1;
  CATCH(e)
    TESTI(v, 0);
    TEST(exc_type(e), exc_generic_t);
    assert(str_ends(exc_msg(e), "Value is 0"));
    v = 2;
  _TRY
  TESTI(v, 2);

  // THROW(exc_generic_t, "FAIL fn0");
}

char *fn2(void) {
  int v = -1;
  TRY
    v = 0;
    THROW(exc_generic_t, str_f("Value is %d", v));
    v = 1;
  CATCH(e)
    TESTI(v, 0);
    TEST(exc_type(e), exc_generic_t);
    assert(str_ends(exc_msg(e), "Value is 0"));
    v = 2;
  _TRY
  TESTI(v, 2);

  // THROW(exc_generic_t, "FAIL fn");

  return "ok2";
}

void fn_b0(void) {
  THROW(exc_io_t, "FAIL fn0");
}

void fn_b1(void) {
  TRY {
    fn_b0();
  } CATCH (e) {
    THROW(exc_generic_t, str_right(exc_msg(e), -3));
  }_TRY
}

void exc_tests(void) {
  puts(">>> exc:");

  TRY
    pthread_t *thr = thread_start(fn);
    //sys_sleep(50);
    TEST(fn2(), "ok2");
    thread_join(thr);
  CATCH (e)
    TEST(exc_msg(e), "ok2");
  _TRY

  TRY {
    fn_b1();
  } CATCH (e) {
    TEST(exc_type(e), exc_generic_t);
  } _TRY

  puts("... Finished");
}
