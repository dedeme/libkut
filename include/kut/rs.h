// Copyright 23-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Result.
/// Example:
///     Rs *r = sys_cmd("wget");
///     if (*rs_error(r)) puts(str_f("ERR: %s", rs_error(r)));
///     else puts(str_f("Ok: %s", rs_get(r)));


#ifndef LKUT_RS_H
  #define LKUT_RS_H

/// Result type.
typedef struct rs_Rs Rs;

/// Creates an ok result.
Rs *rs_ok (void *value);

/// Creates a failure result.
Rs *rs_fail (char *msg);

/// Returns the 'ok' field or NULL if 'this' is a failure.
void *rs_get (Rs *this);

/// Returns the error field or "" if 'this' is ok.
char *rs_error (Rs *this);

///
char *rs_to_js (Rs *this, char *(*to)(void *e));

///
Rs *rs_from_js (char *js, void *(*from)(char *jse));

#endif
