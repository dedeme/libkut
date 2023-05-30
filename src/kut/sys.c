// Copyright 21-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#define _GNU_SOURCE // Need for environ
#include <locale.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <errno.h>
#include "kut/sys.h"
#include "kut/DEFS.h"
#include "kut/str.h"
#include "kut/buf.h"
#include "kut/thread.h"
#include "kut/file.h"
#include "kut/time.h"

static int initialized = FALSE;

void sys_init(void) {
  if (initialized)
    EXC_ILLEGAL_STATE("module 'sys' already initialized");

  initialized = TRUE;
  setlocale(LC_ALL, "C");

  srand(time(0));
  exc_init();
}

// <char>
Map *sys_environ(void) {
  //<char>
  Map *r = map_new();
  char **penv = environ;
  while (*penv) {
    // <char>
    Arr *parts = str_csplit_trim(*penv++, '=');
    map_put(r, arr_get(parts, 0), arr_get(parts, 1));
  }
  return r;
}

void sys_sleep (int millis) {
  Time end = time_now() + millis;
  struct timespec t;
  struct timespec rem;

  for (;;) {
    t.tv_sec = millis / 1000;
    t.tv_nsec = (millis % 1000) * 1000000;
    if (nanosleep(&t, &rem)) {
      millis = end - time_now();
      if (millis <= 0) break;
    } else {
      break;
    }
  }
}

void sys_set_locale (char *language) {
  setlocale (LC_ALL, language);
}

char *sys_get_locale (void) {
  return setlocale (LC_ALL, NULL);
}

double sys_rnd_d (void) {
  return (double)rand() / (double)RAND_MAX;
}

int sys_rnd_i (int top) {
  return (int)(sys_rnd_d() * top);
}

int sys_user_id(void) {
  return getuid();
}

char *sys_user_name() {
  char *name = opt_get(map_get(sys_environ(), "USER"));
  if (name) return name;
  return "";
}

char *sys_user_home(void) {
  char *home = opt_get(map_get(sys_environ(), "HOME"));
  if (home) return home;
  return "";
}

// <char>
Rs *sys_cmd(char *command) {
  char *ferr = file_tmp("/tmp", "dmC");
  char *cmd = str_f("%s 2>%s", command, ferr);
  FILE *fp = popen(cmd, "r");

  if (!fp)
    return rs_fail(str_f("NOEXEC: '%s'", command));

  Buf *bf = buf_new();
  char *line = NULL;
  size_t len = 0;
  while (getline(&line, &len, fp) != -1) {
    buf_add(bf, line);
    free(line);
    line = NULL;
  }
  free(line);
  pclose(fp);

  char *err = "";
  if (file_exists(ferr)) {
    err = file_read(ferr);
    file_del(ferr);
  }

  char *out = str_new(buf_str(bf));
  return (*err) ? rs_fail(err) : rs_ok(out);
}

char *sys_read_line (void) {
  char *s = malloc(150);
  size_t n = 150;
  if (getline(&s, &n, stdin) == -1) {
    free(s); // correct
    EXC_IO("Fail reading on console");
  }
  char *r = str_left(s, -1);
  free(s);  // correct
  return r;
}

