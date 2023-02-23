// Copyright 22-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Cryptographic utilities.
/// Functions of this file must be called after calling 'sys_init()'.


#ifndef LKUT_CRYP_H
  #define LKUT_CRYP_H

/// Generates a b64 random key of a length 'lg'.
/// Throws EXC_ILLEGAL_ARGUMENT if lg <= 0.
char *cryp_genk (int lg);

/// Codified 'k' in irreversible way, using 'lg' b64 digits..
///   k : String to codify.
///   lg: Length of result.
/// Throws EXC_ILLEGAL_ARGUMENT if key == "".
char *cryp_key (char *k, int lg);

/// Encodes 's' with key 'k'.
///   k: Key for encoding.
///   s: Message to encode.
/// Throws EXC_ILLEGAL_ARGUMENT if k == "".
char *cryp_encode (char *k, char *s);

/// Decodes 'b64' using key 'k'. 'b64' was codified with cryp_encode().
///   k  : Key for decoding.
///   b64: Text codified with cryp().
/// Throws EXC_ILLEGAL_ARGUMENT if k == "".
char *cryp_decode (char *k, char *b64);

#endif
