// Copyright 21-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Option.

#ifndef LKUT_OPT_H
  #define LKUT_OPT_H

/// Option type.
typedef struct opt_Opt Opt;

/// Creates an option with value.
Opt *opt_some (void *value);

/// Creates an option without value.
Opt *opt_none (void);

/// Returns NULL if 'this' is empty.
void *opt_get (Opt *this);

/// Throws a illegal_state_exception_t if 'this' is empty.
void *opt_eget (Opt *this);

///
// Js *opt_to_js (Opt *this, Js *(*to)(void *e)); -------------------

///
// Opt *opt_from_js (Js *js, void *(*from)(Js *jse)); ------------------

#endif
