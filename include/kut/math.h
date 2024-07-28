// Copyright 24-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Utilitis to manage numbers.

#ifndef LKUT_MATH_H
  #define LKUT_MATH_H

#include <stdint.h>

/// Returns a standard string from 'n'.
char *math_itos (int64_t n);

/// Returns a standard string from 'n', with 'scale' decimal digits.
/// 'scale' is forzed to a value between '0' and '9' (both inclusive)
/// Example:
///   math_ftos(12.5, 2); \\ -> "12.50"
char *math_ftos (double n, int scale);

/// Returns an 'int' from its standard string.
/// 's' is 'trimmized' and then it can no be "".
/// Throws EXC_ILLEGAL_ARGUMENT if 's' is not a valid number.
int math_stoi (char *s);

/// Returns an 'long' from its standard string.
/// 's' is 'trimmized' and then it can no be "".
/// Throws EXC_ILLEGAL_ARGUMENT if 's' is not a valid number.
int64_t math_stol (char *s);

/// Returns an 'float' from its standard string.
/// 's' is 'trimmized' and then it can no be "".
/// Throws EXC_ILLEGAL_ARGUMENT if 's' is not a valid number.
float math_stof (char *s);

/// Returns an 'double' from its standard string.
/// 's' is 'trimmized' and then it can no be "".
/// Throws EXC_ILLEGAL_ARGUMENT if 's' is not a valid number.
double math_stod (char *s);

/// Returns a locale string from 'n', with 'scale' decimal digits.
/// 'scale' is forzed to a value between '0' and '9' (both inclusive)
/// Example:
///   sys_set_locale("es_ES.utf8");
///   math_to_locale(3512.5, 2); \\ -> "3.512,50"
///   sys_set_locale("C");
/// Throws EXC_ILLEGAL_ARGUMENT if 's' is not a valid number.
char *math_to_locale (double n, int scale);

/// Returns an 'double' from its ISO string (decimal separator: ',').
/// Throws EXC_ILLEGAL_ARGUMENT if 's' is not a valid number.
double math_from_iso (char *s);

/// Returns an 'double' from its 'EN' string (decimal separator: '.').
/// Throws EXC_ILLEGAL_ARGUMENT if 's' is not a valid number.
double math_from_en (char *s);

/// Returns TRUE if 's' is not "" and every character in it is a digit.
int math_digits (char *s);

/// Returns 'n' rounded with 'scale' decimal digits.
/// 'scale' is forzed to a value between '0' and '9' (both inclusive).
double math_round (double n, int scale);

/// Equals to 'eq_gap(n, 0.0000001)'.
int math_eq (double n1, double n2);

/// Returns TRUE if n1 < n2 + gap && n1 > n2 - gap
/// 'gap' is forzed to a value between '0.0000000001' and '1' (both inclusive).
int math_eq_gap (double n1, double n2, double gap);

/// Returns abs(n).
double math_abs (double n);

/// Returns base^exp.
double math_pow (double base, double exp);

/// Returns the square root of 'n'.
double math_sqrt (double n);

#endif
