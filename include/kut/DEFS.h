// Copyright 20-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Definitions.

#ifndef LKUT_DEFS_H
  #define LKUT_DEFS_H

#include <gc.h>

#define __STDC_WANT_LIB_EXT2__ 1  //Define for asprintf
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "kut/exc.h"
#include "kut/str.h"

/// Definition for working with boolena balues.
#define TRUE 1

/// Definition for working with boolena balues.
#define FALSE 0

/// Returns a pointer to a new allocated memory which also includes pointers.
///   type <literal>: Identifier of type.
/// Example:
///   Foo *this = MALLOC(Foo);
#define MALLOC(type) (type *)GC_MALLOC(sizeof(type))

/// Returns a pointer to a new allocated momory which does not include pointers.
///   size <int>: Bytes to allocate.
/// Example:
///   Foo *this = ATOMIC(3200);
#define ATOMIC(size) GC_MALLOC_ATOMIC(size)

/// Repeats a blok 'n' times.
///   n <int>: Number of iterations.
/// Example:
///   REPEAT (3)
///     puts ("a");
///   _REPEAT
#define REPEAT(n) { \
  int __i = (n) + 1; \
  while (--__i) {

/// Ends a REPEAT block.
#define _REPEAT }}

/// Repeats a block giving to 'i' successively values from 0 (inclusive) to
/// 'end' (exclusive).
///   i <literal>: Iterator counter. It stars in 0.
///   end <int>: End of iterations (exclusive).
/// Example:
///   RANGE0(i, 10)
///     puts(str_f("%d", i));
///   _RANGE
///   // Show values from 0 to 9 (both inclusive).
#define RANGE0(i, end) { \
  int __end = end; \
  int i = -1; \
  while (++i < __end) {

/// Repeats a block giving to 'i' successively values from 'begin' (inclusive) to
/// 'end' (exclusive).
///   i <literal>: Iterator counter. It stars in 'begin'.
///   begin <int>: Begin of iterations (incluive)
///   end <int>: End of iterations (exclusive).
/// Example:
///   RANGE(i, 3, 10)
///     puts(str_f("%d", i));
///   _RANGE
///   // Show values from 3 to 9 (both inclusive).
#define RANGE(i, begin, end) { \
  int __end = end; \
  int i = (begin) - 1; \
  while (++i < __end) {

/// Ends a RANGE block.
#define _RANGE }}

/// Iterates over an 'Arr'. You can access to the 'element' index with _i.
///   a <Arr>: An Arr *.
///   type <literal>  : Element type without pointer sign (*).
///   e <literal>: Name of 'a' elments of type 'type'.
/// Example:
///   EACH(a, char, s) {
///     printf("[%d] -> %s\n", _i, s);
///   } _EACH
#define EACH(a, type, e) { \
  Arr *__a = (Arr *)a; \
  void **__p = arr_begin(__a); \
  void **__pend = arr_end(__a); \
  type *e; \
  int _i = -1; \
  while(__p < __pend) { \
    ++_i; \
    e = *__p++;

/// Iterates over an 'Arr' in reverse order. You can access to the 'element'
/// index with _i.
///   a <Arr>: An Arr *.
///   type <literal>  : Element type without pointer sign (*).
///   e <literal>: Name of 'a' elments of type 'type'.
/// Example:
///   EACHR(a, char, s) {
///     printf("[%d] -> %s\n", _i, s);
///   } _EACH
#define EACHR(a, type, e) { \
  Arr *__a = (Arr *)a; \
  void **__p = arr_end(__a); \
  void **__pstart = arr_begin(__a); \
  type *e; \
  int _i = arr_size(__a); \
  while(__p > __pstart) { \
    --_i; \
    e = *(--__p);

/// Finalizes EACH or EACHR.
#define _EACH }}

/// Macros to manage exceptions. Example:
///   TRY
///     ...
///   CATCH (e)
///     puts(exc_msg(e));
///   _TRY
/// NOTE: CATCH block must return 'void'
#define TRY { \
  jmp_buf *_TRY_buf = MALLOC(jmp_buf); \
  exc_add(_TRY_buf); \
  if (!setjmp(*_TRY_buf)) { \

/// See TRY.
///   e <literal>: Variable holding the exception.
#define CATCH(e) ;exc_remove();} else { exc_remove(); Exc *e = exc_get();

/// Finalizes bloks 'TRY ... CATCH(E) ... _TRY'.
#define _TRY }}

/// Throws an exception.
///   type <char *>: Exception type.
///   format <char *>: Format string (see FMT)
///   args... <variadic>: Arguments for 'format'.
/// Example:
///   THROW(exc_io_t, "Working directory not found: %s", strerror(errno));
#define THROW(type, format, args...) \
    exc_throw(type, str_f(format, ##args), __FILE__, (char *)__func__, __LINE__);

/// Throws a exception of type 'exc_generic_t'.
///   msg <char *>: Exception message.
/// Example:
///   EXC_GENERIC("Fail");
#define EXC_GENERIC(msg) \
  THROW(exc_generic_t, msg)

/// Throws a range exception if v < 'min' or v > 'max'.
///   value <int>: Value to compare.
///   min <int>: Minimum value of range (inclusive).
///   max <int>: Maximum value of range (inclusive).
/// Example:
///   EXC_RANGE(v, 0, 23); // -1 and 24 throw exeption.
#define EXC_RANGE(value, min, max) { \
    int __v = value; \
    int __min = min; \
    int __max = max; \
    if (__v < __min || __v > __max) \
      THROW(exc_range_t, exc_range(__min, __max, __v)) \
  }

/// Throw a expection if type 'exc_illegal_argument_t'.
///   var <char *>     : Argument name.
///   expected <char *>: Expected argument value.
///   actual <char *>  : Actual argument value.
/// Example:
///   EXC_ILLEGAL_ARGUMENT("result", "12", "abc");
#define EXC_ILLEGAL_ARGUMENT(msg, expected, actual) \
  THROW(exc_illegal_argument_t, exc_illegal_argument(msg, expected, actual))

/// Throws a 'exc_illegal_state_t'.
///   msg <char *>     : Exception message.
/// Example:
///   EXC_ILLEGAL_STATE("Fail");
#define EXC_ILLEGAL_STATE(msg) \
  THROW(exc_illegal_state_t, exc_illegal_state(msg))

/// Throws a 'exc_io_t'.
///   msg <char *>     : Exception message.
/// Example:
///   EXC_IO("Fail");
#define EXC_IO(msg) \
  THROW(exc_io_t, exc_io(msg))

#define TEST(actual, expected) \
  if (strcmp(actual, expected)) \
  EXC_ILLEGAL_ARGUMENT("Test failed", expected, actual)

#define TESTI(actual, expected) { \
    char *__sactual = str_f("%d", actual); \
    char *__sexpected = str_f("%d", expected); \
    if (strcmp(__sactual, __sexpected)) \
    EXC_ILLEGAL_ARGUMENT("Test failed", __sexpected, __sactual) \
  }

#define TESTF(actual, expected) { \
    char *__sactual = str_f("%.10f", actual); \
    char *__sexpected = str_f("%.10f", expected); \
    if (strcmp(__sactual, __sexpected)) \
    EXC_ILLEGAL_ARGUMENT("Test failed", __sexpected, __sactual) \
  }

///
typedef void *(*FMAP)(void *);

///
typedef int (*FEQ)(void *, void *);

///
typedef void (*FPROC)(void *);

///
typedef int (*FPRED)(void *);

#endif
