// Copyright 21-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Management of multithread programs.
/// This file must be called the first when using threads and compiled with
/// '-lpthread'


#ifndef LKUT_THREAD_H
  #define LKUT_THREAD_H

#include <pthread.h>
/// Necesary definition for multithreading garbage collector.
#define GC_THREADS
#include "gc.h"

/// Launchs 'fn' in a new joinable thread.
/// Example:
///   /**/void fn(void) { puts("Here"); }
///   pthread_t *thr = thread_start(fn);
///   ...
///   thread_join(thr); // Waits for thr.
/// NOTE: After calling 'async_thread' is mandatory to call 'thread_join' to
///         free resources.
pthread_t *thread_start (void (*fn)(void));

/// Launchs 'fn' in a new joinable thread.
/// Example:
///   /**/void fn(char *tx) { puts(tx); }
///   pthread_t *thr = thread_start2((void(*)(void *))fn, "Hello");
///   ...
///   thread_join(thr); // Waits for thr.
/// NOTE: After calling 'thread_start2' is mandatory to call 'async_join' to
///         free resources.
pthread_t *thread_start2 (void (*fn)(void *), void *value);

/// Launch 'fn' in a new thread.
/// Example:
///   /**/void fn(void) { puts("Here"); }
///   thread_run((fn);
void thread_run (void (*fn)(void));

/// Launch 'fn' in a new thread.
/// Example:
///   /**/void fn(char *tx) { puts(tx); }
///   thread_run2((void(*)(void *))fn, "Hello");
void thread_run2 (void (*fn)(void *), void *value);

/// Wait until thr finishes.
void thread_join (pthread_t *thr);

/// Executes 'fn' synchronically.
/// Example:
///   /**/void fn() { puts("Here"); }
///   thread_sync(fn);
/// NOTE: if 'fn' call another function which also call 'thread_sync' or
///       'thread_sync2', the program will be blocked.
void thread_sync (void (*fn)(void));

/// Executes 'fn' synchronically.
/// Example:
///   /**/void fn(char *tx) { puts(tx); }
///   thread_sync2((void(*)(void *))fn, "Hello");
/// NOTE: if 'fn' call another function which also call 'thread_sync' or
///       'thread_sync2', the program will be blocked.
void thread_sync2 (void (*fn)(void *), void *value);

#endif
