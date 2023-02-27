// Copyright 21-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Array object.

#ifndef LKUT_ARR_H
  #define LKUT_ARR_H

#include "opt.h"

typedef struct it_It It;

/// Array object.
typedef struct arr_Arr Arr;

/// Creates an empty array.
Arr *arr_new ();

/// Creates an empty array with a defined initial buffer.
Arr *arr_new_bf (int buffer);

/// Creates an array from its elements.
/// 'e' is a 'NULL teminated' list of at leat one element.
Arr *arr_new_from (void *e, ...);

/// Creates an array from a C array.
/// Elements of 'es' are shallowing copied.
Arr *arr_new_c (int size, void **es);

/// Returns a shalow copy of 'this'.
Arr *arr_copy (Arr *this);

/// Restuns the pointer to the begin of elements.
///   arr_size(a) == arr_end(a) - arr_begin(a)
void **arr_begin (Arr *this);

/// Restuns the pointer to the end of elements.
///   arr_size(a) == arr_end(a) - arr_begin(a)
void **arr_end (Arr *this);

/// Returns 'this' size.
///   arr_size(a) == arr_end(a) - arr_begin(a)
int arr_size (Arr *this);

/// Returns TRUE if 'this' has no element.
int arr_empty (Arr *this);

/// Returns the element at position ix.
/// Throws EXC_RANGE if 'ix' is out of limits.
void *arr_get (Arr *this, int ix);

/// Adds in place 'e' at the end of 'this'.
void arr_push (Arr *this, void *e);

/// Returns and removes the last element.
/// Throws EXC_ILLEGAL_STATE if 'this' is empty.
void *arr_pop (Arr *this);

/// Returns the las element.
/// Throws EXC_ILLEGAL_STATE if 'this' is empty.
void *arr_peek (Arr *this);

/// Sets the element at position ix.
/// Throws EXC_RANGE if 'ix' is out of limits.
void arr_set (Arr *this, int ix, void *e);

/// Inserts an element at position ix.
/// Throws EXC_RANGE if 'ix' is out of limits.
void arr_insert (Arr *this, int ix, void *e);

/// Removes an element at position ix. Buffer size of 'this' does not change.
/// Throws EXC_RANGE if 'ix' is out of limits.
void arr_remove (Arr *this, int ix);

/// Adds in place elements (pointers) of 'other' to 'this'.
void arr_cat (Arr *this, Arr *other);

/// Inserts elements (pointers) of 'other' at 'ix'
/// Throws EXC_RANGE if 'ix' is out of limits.
void arr_insert_arr (Arr *this, int ix, Arr *other);

/// Removes elements between [begin-end). Buffer size of 'this' does not change.
/// Throws EXC_RANGE if begin or end are out of range.
void arr_remove_range (Arr *this, int begin, int end);

/// Removes every element of 'this'. Buffer size of 'this' does not change.
void arr_clear (Arr *this);

/// Reverses elements of 'this'.
void arr_reverse (Arr *this);

/// Sorts 'this' ascendantly using the function 'greater' that returns 'TRUE'
/// if 'e1' > 'e2'.
/// 'greater' can be cast to FEQ.
void arr_sort (Arr *this, int (*greater)(void *e1, void *e2));

/// 'arr_shuffle' remix 'this' elements. It should be used after calling
/// sys_init().
void arr_shuffle (Arr *this);

/// Returns 'TRUE' if every element of 'this' produces 'TRUE' with 'pred'.
/// 'pred' can be cast to FPRED.
int arr_all (Arr *this, int (*pred)(void *e));

/// Returns 'TRUE' if some element of 'this' produces 'TRUE' with 'pred'.
/// 'pred' can be cast to FPRED.
int arr_any (Arr *this, int (*pred)(void *e));

/// Executes 'fn' with each element of 'this'.
void arr_each(Arr *this, void (*fn)(void *e));

/// Executes 'fn' with each element of 'this' and its array index.
void arr_each_ix(Arr *this, void (*fn)(void *e, int ix));

/// Returns TRUE if 'a1' has the same elements number as 'a2' and f(e1, e1) for
/// each element returns TRUE.
int arr_eq(Arr *a1, Arr *a2, int (*feq)(void *e1, void *e2));

/// Returns the index of the first elements which returns 'TRUE'
/// with 'pred', or -1 if such element does not exist.
/// 'pred' can be cast to FPRED.
int arr_index (Arr *this, int (*pred)(void *e));

/// Equals to (arr_index(a, fn) != -1)
int arr_contains (Arr *this, int (*pred)(void *e));

/// Returns the index of the last elements which returns 'TRUE'
/// with 'pred', or -1 if such element does not exist.
/// 'pred' can be cast to FPRED.
int arr_last_index (Arr *this, int (*pred)(void *e));

/// Returns the first element which pruduces 'TRUE' with 'pred' (opt_some(e))
/// or, otherwise, 'opt_none()'.
/// 'pred' can be cast to FPRED.
Opt *arr_find(Arr *this, int (*pred)(void *e));

/// Returns the last element which pruduces 'TRUE' with 'pred' (opt_some(e))
/// or, otherwise, 'opt_none()'.
/// 'pred' can be cast to FPRED.
Opt *arr_find_last(Arr *this, int (*pred)(void *e));

/// 'arr_filter_in' removes in place every element which returns 'FALSE' with
/// 'pred'.
/// 'pred' can be cast to FPRED.
void arr_filter_in (Arr *this, int (*pred)(void *e));

/// Returns a new Arr with the 'n' first elements of 'this'.
/// If 'this' has less elements than 'n', returs a copy of this.
/// If 'n <= 0) returns an empty array.
Arr *arr_take (Arr *this, int n);

/// Returns a new Arr with the first elements which return 'TRUE' with 'pred'.
/// 'pred' can be cast to FPRED.
Arr *arr_takef (Arr *this, int (*pred)(void *e));

/// Returns a new Arr with the elements left after 'arr_take()'.
Arr *arr_drop (Arr *this, int n);

/// Returns a new Arr with elements left after 'arr_takef()'.
/// 'pred' can be cast to FPRED.
Arr *arr_dropf (Arr *this, int (*pred)(void *e));

/// Returns a new Arr with every element which returns 'TRUE' with 'pred'.
/// 'pred' can be cast to FPRED.
Arr *arr_filter_to (Arr *this, int (*pred)(void *e));

/// Returns a new Arr with elements generated by apply converter to them.
/// 'converter' can be cast to FMAP
Arr *arr_map (Arr *this, void *(*converter)(void *e));

/// Returns a new Arr whit the first element generated by apply conv1 to
/// the old first one and the rest by apply conv2.
/// 'conv1' and 'conv2' can be cast to FMAP
Arr *arr_map2 (Arr *this, void *(*conv1)(void *e), void *(*conv2)(void *e));

/// Returns a new Arr mixing values of 'a1' and 'a2' produced by successively
/// apply 'converter' to their elements. The size of the resultant
/// array is the less of 'arr_size(a1)' and 'arr_size(a2)'.
/// 'converter' can be cast to FMAP
Arr *arr_zip (Arr *a1, Arr *a2, void *(*converter)(void *e1, void *e2));

/// Returns a new Arr mixing values of 'a1', 'a2' and 'a3'produced by successively
/// apply 'converter' to their elements. The size of the resultant array is the
/// less of 'arr_size(a1)', 'arr_size(a2)' and 'arr_size(a3)'.
Arr *arr_zip3 (
  Arr *a1, Arr *a2, Arr *a3, void*(*converter)(void*e1, void*e2, void*e3)
);

/// Removes in place duplicates with function 'feq(e)=TRUE' and returns them
/// in a new array.
/// It returns only the first duplicated element.
/// 'feq' can be cast to FEQ.
Arr *arr_duplicates (Arr *this, int (feq)(void *e1, void *e2));

/// Joins elements of 'a', separated by 'sep'.
/// 'a' is Arr<char>.
char *arr_cjoin(Arr *a, char sep);

/// Joins elements of 'a', separated by 'sep'.
/// 'a' is Arr<char>.
char *arr_join(Arr *a, char *sep);

/// Creates an iterator over 'this'.
It *arr_to_it (Arr *this);

/// Creates an Arr from 'it'.
Arr *arr_from_it (It *it);

/// Returns this JSONized.
///   this: Container.
///   to  : Converter of container element to JSON.
char *arr_to_js (Arr *this, char *(*to)(void *e));

/// Returns the container from its JSON representation.
///   js  : Container JSONized.
///   from: Converter from JSON to container element.
Arr *arr_from_js (char *js, void *(*from)(char *ejs));

#endif
