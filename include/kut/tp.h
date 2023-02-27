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

#endif
