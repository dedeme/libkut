// Copyright 23-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Map object.

#ifndef LKUT_MAP_H
  #define LKUT_MAP_H

#include "kut/kv.h"
#include "kut/arr.h"

/// Map object.
typedef struct arr_Map Map;

/// Creates a new map.
Map *map_new(void);

/// Returns the number of elements.
int map_size(Map *this);

/// Puts 'value' with key 'key'. If key already exists its value
/// is changed.
///   this : The map.
///   key  : Entry key.
///   value: New value
void map_put(Map *this, char *key, void *value);

/// Returns TRUE if 'this' has the key 'key'.
int map_has_key(Map *this, char *key);

/// Returns the value pointed by key.
Opt *map_get(Map *this, char *key);

/// Removes value with key 'key' or does nothing if 'key' does not exist.
void map_remove(Map *this, char *key);

/// Returns an Arr<char> with keys.
Arr *map_keys(Map *this);

/// Returns the pairs 'Kv' (Arr<Kv>) of this.
Arr *map_to_array(Map *this);

/// Makes a map from an Arr<Kv>.
/// If there are duplicate keys, only the first of then is kept.
Map *map_from_array(Arr *this);

/// Sorts ascendently 'this' by keys.
void map_sort(Map *this);

/// Sorts ascendently 'this' by keys in locale.
void map_sort_locale(Map *this);

/// Returns this JSONized.
///   this: Container.
///   to  : Converter of container value to JSON.
//char *map_to_js (Map *this, char *(*to)(void *e));

/// Returns the container from its JSON representation.
///   js  : Container JSONized.
///   from: Converter from JSON to container value.
//Map *map_from_js (char *js, void *(*from)(char *ejs));

#endif
