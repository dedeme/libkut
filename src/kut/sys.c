// Copyright 21-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <locale.h>
#include <time.h>
#include <stdlib.h>
#include "kut/exc.h"

void sys_init(void) {
  setlocale(LC_ALL, "C");
  srand(time(0));
  exc_init();
}

void sys_sleep (int millis) {
  struct timespec t;
  struct timespec rem;

  t.tv_sec = millis / 1000;
  t.tv_nsec = (millis % 1000) * 1000000;
  nanosleep(&t, &rem);
}

void sys_set_locale (char *language) {
  setlocale (LC_ALL, language);
}

double sys_rnd_d (void) {
  return (double)rand() / (double)RAND_MAX;
}

int sys_rnd_i (int top) {
  return (int)(sys_rnd_d() * top);
}

char *sys_locale (void) {
  return setlocale (LC_ALL, NULL);
}
