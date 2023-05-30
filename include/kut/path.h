// Copyright 21-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Paths management.

#ifndef LKUT_PATH_H
  #define LKUT_PATH_H

#include "opt.h"

/// Concatenates several file names (at least 2).
/// Variable argumens must finish with NULL.
char *path_cat (char *name1, char *name2, ...);

/// Returns base name of 'path' after removing all trailing '/'.
/// If path is "" or is root, it returns "".
char *path_base (char *path);

/// Returns the parent path of 'path' after removing all trailing '/'.
/// If 'path' is a string without '/', it returns an empty string.
/// If path is root or an empty string, it throws an exception.
char *path_parent (char *path);

/// Returns only extension of 'path_base(path)'. Extension is returned with point,
/// (e.g., ".", ".txt").
/// If path does not have extension it returns "".
char *path_extension (char *path);

/// Removes duplicates '/' and redundant '..'.
/// The returns ends in a slash only if it is the root "/".
char *path_clean (char *s);

/// Returns Opt<char> Cannonical representation of 'path'.
///   - If some component of 'path' is not in file system, returns 'opt_none'.
///   - Directories do not finish in '/'.
Opt *path_canonical (char *s);

#endif
