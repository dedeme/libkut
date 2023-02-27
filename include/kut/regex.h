// Copyright 26-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Regular expresion utilities.

#ifndef LKUT_REGEX_H
  #define LKUT_REGEX_H

#include "opt.h"

/// Type of regular expresion results.
/// Every offset is a pair of ints [begin-end] , where 'begin' is the start
/// of the string offset inclusive, and 'end' the end point exclusive.
typedef struct regex_RegexOffet RegexOffset;

/// Returns the offset begin.
int regexOffset_begin (RegexOffset *off);

/// Returns the offset end.
int regexOffset_end (RegexOffset *off);

/// Returns the offsets where 'rex' is found in s.
///   rex   : Regular expression to find. It does not admit grouping (that is,
///           parenthesis).
///   s     : String to search in.
///   return: Arr<RegexOffset>. Offsets of 's' with elements which match.
///           If any error happens, it returns 'opt_none'.
Opt *regex_matches (char *rex, char *s);

/// Equals to 'regex_matches' but "ignoring case".
Opt *regex_matches_ic (char *rex, char *s);

/// Replace ocurrences of 'rex' in 's' by 'replacement'. Ocurrences are
/// searched by regex_matches.
///   rex        : Regular expression to replace. It does not admit grouping
///                (that is, parenthesis).
///   s          : String to search in.
///   replacement: New string.
///   return     : Opt<char>. A new string with repacements done.
///                If any error happens, it returns ochar_none.
Opt *regex_replace (char *rex, char *s, char *replacement);

/// Equals to regex_replace, but "ignoring case".
Opt *regex_replace_ic (char *rex, char *s, char *replacement);

#endif
