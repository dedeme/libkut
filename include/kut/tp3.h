// Copyright 23-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Tuple of three elements.

#ifndef LKUT_TP3_H
  #define LKUT_TP3_H

/// Type of Tuple of three elements.
typedef struct tp3_Tp3 Tp3;

/// Creates a tuple of three elements.
Tp3 *tp3_new (void *e1, void *e2, void *e3);

/// Returns the first element.
void *tp3_e1 (Tp3 *this);

/// Returns the second element.
void *tp3_e2 (Tp3 *this);

/// Returns the therd element.
void *tp3_e3 (Tp3 *this);


#endif
