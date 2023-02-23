// Copyright 21-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "kut/thread.h"
#include "kut/DEFS.h"

static pthread_mutex_t thread_mutex;

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
  exc_thread_end();
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

void thread_run (void (*fn)()) {
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
  pthread_mutex_lock(&thread_mutex);
  fn();
  pthread_mutex_unlock(&thread_mutex);
}

void thread_sync2 (void (*fn)(void *), void *value) {
  pthread_mutex_lock(&thread_mutex);
  fn(value);
  pthread_mutex_unlock(&thread_mutex);
}
