// Copyright 23-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Result.

#ifndef LKUT_RS_H
  #define LKUT_RS_H

/// Result type.
typedef struct rs_Rs Rs;

/// Creates an ok result.
Rs *rs_ok (void *value);

/// Creates a failure result.
Rs *rs_fail (char *msg);

/// Returns NULL if 'this' is a failure.
void *rs_get (Rs *this);

/// Throws a illegal_state_exception_t if 'this' is a failure.
void *rs_eget (Rs *this);

/// Returns the error field.
/// If 'this' is ok returns "".
char *rs_error (Rs *this);

///
// Js *rs_to_js (Rs *this, Js *(*to)(void *e)); -------------------

///
// Rs *rs_from_js (Js *js, void *(*from)(Js *jse)); ------------------


#endif
