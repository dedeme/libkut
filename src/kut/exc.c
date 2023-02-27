// Copyright 21-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <stdlib.h>
#include <stdio.h>
#include <execinfo.h>
#include <setjmp.h>
#include "kut/DEFS.h"
#include "kut/exc.h"
#include "kut/str.h"
#include "kut/arr.h"
#include "kut/path.h"

#include <pthread.h>

// Arr[Exc]
static Arr *pool = NULL;
static pthread_mutex_t mutex;

struct exc_Exc {
  pthread_t thread;
  // [jmp_buf]
  Arr *buf;
  char *type;
  char *msg;
  // [char]
  Arr *stack;
};

static Exc *exc_new (pthread_t thread, jmp_buf *bf) {
  Exc *this = MALLOC(Exc);
  this->thread = thread;
  this->buf = arr_new_from(bf, NULL);
  this->type = NULL;
  this->msg = NULL;
  this->stack = arr_new();
  return this;
}

char *exc_type (Exc *this) {
  if (!this->type) {
    puts("exc_type: 'type' is NULL");
    exit(1);
  }
  return this->type;
}

char *exc_msg (Exc *this) {
  if (!this->msg) {
    printf("exc_msg: 'msg' is NULL");
    exit(1);
  }
  return this->msg;
}

// Arr[char]
Arr *exc_stack (Exc *this) {
  return this->stack;
}

static void exc_exit (Exc *this) {
  puts(exc_msg(this));

  // Arr[char]
  Arr *st = exc_stack(this);
  printf("\nObtained %d stack frames.\n", arr_size(st));

  for (int i = 0; i < arr_size(st); ++i) {
    puts(arr_get(st, i));
  }

  exit(1);
}

void exc_init () {
  if (pool) {
    return;
  }
  pool = arr_new();
  pthread_mutex_init(&mutex, NULL);

  jmp_buf *bf = MALLOC(jmp_buf);
  Exc *exc = exc_new(pthread_self(), bf);
  arr_push(pool, exc);

  int val = setjmp(*bf);
  if (val) {
    Exc *exc = exc_get();
    exc_exit(exc);
  }
}

void exc_thread_init (void) {
  if (pool) {
    pthread_mutex_lock(&mutex);

    pthread_t self = pthread_self();
    jmp_buf *bf = MALLOC(jmp_buf);
    Exc *exc = exc_new(self, bf);

    int i = -1;
    EACH(pool, Exc, exc)
      if (pthread_equal(exc->thread, self)) {
        i = _i;
        break;
      }
    _EACH

    if (i == -1) {
      arr_push(pool, exc);
    } else {
      arr_set(pool, i, exc);
    }

    pthread_mutex_unlock(&mutex);

    int val = setjmp(*bf);
    if (val) {
      Exc *exc = exc_get();
      exc_exit(exc);
    }
  } else {
    puts("'exc_init()' has not been called");
    exit(1);
  }
}

void exc_thread_end (void) {
  if (pool) {
    pthread_mutex_lock(&mutex);

    pthread_t th = pthread_self();
    int i = -1;
    EACH(pool, Exc, exc)
      if (pthread_equal(exc->thread, th)) {
        i = _i;
        break;
      }
    _EACH
    if (i != -1) {
      arr_remove(pool, i);
    }

    pthread_mutex_unlock(&mutex);
  } else {
    puts("'exc_init()' has not been called");
    exit(1);
  }
}

void exc_add (jmp_buf *jump) {
  Exc *exc = exc_get();
  arr_push(exc->buf, jump);
}

void exc_remove () {
  Exc *exc = exc_get();
  arr_pop(exc->buf);
}

Exc *exc_get (void) {
  Exc *r = NULL;
  if (pool) {
    pthread_mutex_lock(&mutex);

    pthread_t th = pthread_self();
    EACH(pool, Exc, exc)
      if (pthread_equal(exc->thread, th)) {
        r = exc;
        break;
      }
    _EACH

    pthread_mutex_unlock(&mutex);
  } else {
    puts("'exc_init()' has not been called");
    exit(1);
  }

  if (!r) {
    puts("exc_get: thread not found");
    exit(1);
  }
  return r;
}

void exc_throw (char *type, char *message, char *file, char *func, int line) {
  Exc *exc = exc_get();

  void *array[25];
  size_t size;
  char **strings;
  size = backtrace(array, 25);
  strings = backtrace_symbols(array, size);
  // Arr[char]
  Arr *stack = arr_new();
  RANGE0(i, size) {
    arr_push(stack, str_new(strings[i]));
  }_RANGE
  free(strings);

  exc->stack = stack;
  exc->msg = str_f(
    "%s:%d:[%s]: %s", path_base(file), line, func, message
  );
  exc->type = type;

  longjmp(*((jmp_buf *)arr_peek(exc->buf)), 1);
}

char *exc_range(int begin, int end, int index) {
  return str_f(
    "Index out of range: %d out of [%d - %d]", index, begin, end
  );
}

char *exc_illegal_argument(char *msg, char *expected, char *actual) {
  return str_f(
    "%s\nExpected: %s\n  Actual: %s",
    msg, expected, actual
  );
}
