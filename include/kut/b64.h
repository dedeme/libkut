// Copyright 22-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// B64 encoder - decoder.

#ifndef LKUT_B64_H
  #define LKUT_B64_H

#include "bytes.h"

/// b64 -> utf-8
/// Throws EXC_ILLEGAL_ARGUMENT if b64 has not a valid codification.
char *b64_decode(char *b64);

/// b64 -> bytes
/// Throws EXC_ILLEGAL_ARGUMENT if b64 has not a valid codification.
Bytes *b64_decode_bytes(char *b64);

/// utf8 -> b64
char *b64_encode(char *s);

/// bytes -> b64
char *b64_encode_bytes(Bytes *bs);

#endif
