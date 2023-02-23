// Copyright 23-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Tuple of two elements.

#ifndef LKUT_TP_H
  #define LKUT_TP_H

/// Type of Tuple of two elements.
typedef struct tp_Tp Tp;

/// Creates a tuple of two elements.
Tp *tp_new (void *e1, void *e2);

/// Returns the first element.
void *tp_e1 (Tp *this);

/// Returns the second element.
void *tp_e2 (Tp *this);

// Js *tp_to_js (Tp *this, Js *(*to1)(void *e), Js *(*to2)(void *e));

// Tp *tp_from_js (Js *js, void *(*from1)(Js *jse), void *(*from2)(Js *jse));


#endif
