// Copyright 22-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// (Key - Value) pair.

#ifndef LKUT_KV_H
  #define LKUT_KV_H

/// Key value type.
typedef struct kv_Kv Kv;

/// Creates a new pair.
Kv *kv_new (char *key, void *value);

/// Returns the key of 'this'.
char *kv_key (Kv *this);

/// Returns the value of 'this'.
void *kv_value (Kv *this);

///
char *kv_to_js (Kv *this, char *(*to)(void *e));

///
Kv *kv_from_js (char *js, void *(*from)(char *jse));

#endif
