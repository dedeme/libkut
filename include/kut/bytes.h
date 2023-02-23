// Copyright 22-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Bytes arrays.

#ifndef LKUT_BYTES_H
  #define LKUT_BYTES_H

/// Bytes structure
typedef struct bytes_Bytes Bytes;

/// Returns a new empty 'Bytes'.
Bytes *bytes_new(void);

/// Returns a new 'Bytes' of length 'length' of indetermined values.
Bytes *bytes_new_bf(int length);

/// Returns a new allocated 'Bytes' whitch is copy of 'bs'.
Bytes *bytes_from_bytes(unsigned char *bs, int length);

/// Returns a 'Bytes' whitch is copy of 's' without the ending zero.
Bytes *bytes_from_str(char *s);

/// Returns the pointer to the array enveloped.
unsigned char *bytes_bs (Bytes *this);

/// Returns the length if the array enveloped.
int bytes_len (Bytes *this);

/// Adds to 'this' a copy of 'bs'.
void bytes_add_bytes(Bytes *this, unsigned char *bs, int length);

/// Adds to 'this' a copy of 'another'.
void bytes_add(Bytes *this, Bytes *another);

/// Adds to 'this' a copy of 's' without the ending zero.
void bytes_add_str(Bytes *this, char *s);

#endif
