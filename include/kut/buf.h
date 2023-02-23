// Copyright 21-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Strings constructor.

#ifndef LKUT_BUF_H
  #define LKUT_BUF_H

/// Buf structure.
typedef struct buf_Buf Buf;

/// Creates a buffer with size 150.
Buf *buf_new (void);

/// Creates a buffer with size 'buffer_size'.
Buf *buf_new_bf (int buffer_size);

/// Returns the enveloped string of 'this'
char *buf_str (Buf *this);

/// Returns the length of the enveloped string of 'this. It is faster than
/// 'strlen(buf_str(this))'.
int buf_len (Buf *this);

/// Adds 'data' to 'this'.
void buf_add(Buf *this, char *data);

/// Adds a character to 'this'.
void buf_cadd(Buf *this, char data);

/// Resets buffer (but does not reduce its size).
void buf_reset(Buf *this);
#endif
