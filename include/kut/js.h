// Copyright 24-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// JSON encoder - decoder

#ifndef LKUT_JS_H
  #define LKUT_JS_H

#include "arr.h"
#include "map.h"

/// Returns '1' if json is "null" or '0' in another case.
int js_is_null (char *json);

/// Read boolean.
/// Throws EXC_ILLEGAL_ARGUMENT if 'json' is not a valid JSON string.
int js_rb (char *json);

/// Read int.
/// Throws EXC_ILLEGAL_ARGUMENT if 'json' is not a valid JSON string.
int js_ri (char *json);

/// Read long.
/// Throws EXC_ILLEGAL_ARGUMENT if 'json' is not a valid JSON string.
long js_rl (char *json);

/// Read double.
/// Throws EXC_ILLEGAL_ARGUMENT if 'json' is not a valid JSON string.
double js_rd (char *json);

/// Read string.
/// Throws EXC_ILLEGAL_ARGUMENT if 'json' is not a valid JSON string.
char *js_rs (char *json);

/// Read array in Arr<char>.
/// Throws EXC_ILLEGAL_ARGUMENT if 'json' is not a valid JSON string.
Arr *js_ra (char *json);

/// Read object in Map<char>.
/// Throws EXC_ILLEGAL_ARGUMENT if 'json' is not a valid JSON string.
Map *js_ro (char *json);

/// Write a null value.
char *js_wn(void);

/// Write a boolean value.
char *js_wb(int value);

/// Write an int value.
char *js_wi(int n);

/// Write an long value.
char *js_wl(long n);

/// Write a double value.
char *js_wf(double n, int scale);

/// Write a string.
char *js_ws(char *s);

/// Write an Arr<char>.
char *js_wa(Arr *a);

/// Write a Map<char>.
char *js_wo(Map *m);

#endif
