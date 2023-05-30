// Copyright 21-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "kut/thread.h"
#include "kut/DEFS.h"
#include <unistd.h>

static pthread_mutex_t thread_mutex;
static pthread_t *thread_in = NULL;

struct thread_Thread {
  void (*fn) (void *);
  void *value;
};

static struct thread_Thread *thread_thread_new (
  void (*fn) (void *), void *value
) {
  struct thread_Thread *this = MALLOC(struct thread_Thread);
  this->fn = fn;
  this->value = value;
  return this;
}

static void *thread_thread_run (struct thread_Thread *data) {
  exc_thread_init();
  data->fn(data->value);
  exc_thread_end();
  return NULL;
}

static void *thread_thread_run0 (void (*fn)(void)) {
  exc_thread_init();
  fn();
  exc_thread_end();
  return NULL;
}

pthread_t *thread_start (void (*fn)(void)) {
  pthread_t *thr = MALLOC(pthread_t);
  pthread_create(thr, NULL, (void *(*)(void *))thread_thread_run0, fn);
  return thr;
}

pthread_t *thread_start2 (void (*fn)(void *), void *value) {
  pthread_t *thr = MALLOC(pthread_t);
  struct thread_Thread *data = thread_thread_new(fn, value);
  pthread_create(thr, NULL, (void *(*)(void *))thread_thread_run, data);
  return thr;
}

void thread_run (void (*fn)(void)) {
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  pthread_t *thr = MALLOC(pthread_t);
  pthread_create(thr, &attr, (void *(*)(void *))thread_thread_run0, fn);
}

void thread_run2 (void (*fn)(void *), void *value) {
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  pthread_t *thr = MALLOC(pthread_t);
  struct thread_Thread *data = thread_thread_new(fn, value);
  pthread_create(thr, &attr, (void *(*)(void *))thread_thread_run, data);
}

void thread_join (pthread_t *thr) {
  pthread_join(*thr, NULL);
}

void thread_sync (void (*fn)(void)) {
  pthread_t self = pthread_self();
  if (thread_in && pthread_equal(self, *thread_in)) {
    fn();
    return;
  }

  pthread_mutex_lock(&thread_mutex);
  thread_in = MALLOC(pthread_t);
  *thread_in = self;
  fn();
  thread_in = NULL;
  pthread_mutex_unlock(&thread_mutex);
}

void thread_sync2 (void (*fn)(void *), void *value) {
  pthread_t self = pthread_self();
  if (thread_in && pthread_equal(self, *thread_in)) {
    fn(value);
    return;
  }

  pthread_mutex_lock(&thread_mutex);
  thread_in = MALLOC(pthread_t);
  *thread_in = self;
  fn(value);
  thread_in = NULL;
  pthread_mutex_unlock(&thread_mutex);
}
