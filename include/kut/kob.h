// Copyright 20-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Kut object

#ifndef LKUT_KOB_H
  #define LKUT_KOB_H

#include "kut/arr.h"

/// Types of kut objects
enum KobType {
  kob_BOOL, kob_INT, kob_FLOAT, kob_STRING, kob_ARRAY, kob_MAP
};

/// Kut object.
typedef struct kob_Kob Kob;

enum KobType kob_get_type(Kob *this);

/// Creates a Kob of type Bool.
Kob *kob_bool(int value);

/// Read a Kob of type Bool.
int kob_get_bool(Kob *this);

/// Returns 'true' if 'this' is a Bool.
int kob_is_bool(Kob *this);

/// Creates a Kob of type Int.
Kob *kob_int(int value);

/// Read a Kob of type Int.
int kob_get_int(Kob *this);

/// Returns 'true' if 'this' is an Int.
int kob_is_int(Kob *this);

/// Creates a Kob of type Float.
Kob *kob_float(double value);

/// Read a Kob of type Float.
double kob_get_float(Kob *this);

/// Returns 'true' if 'this' is a Float.
int kob_is_float(Kob *this);

/// Creates a Kob of type String.
Kob *kob_str(char *s);

/// Read a Kob of type String.
char *kob_get_str(Kob *this);

/// Returns 'true' if 'this' is a String.
int kob_is_str(Kob *this);

/// Creates a Kob of type Array.
Kob *kob_arr(Arr *a);

/// Read a Kob of type Array.
Arr *kob_get_arr(Kob *this);

/// Returns 'true' if 'this' is an Array.
int kob_is_arr(Kob *this);

/// Returns a string representation of 'type'.
char *kob_type_to_str (enum KobType type);

#endif
