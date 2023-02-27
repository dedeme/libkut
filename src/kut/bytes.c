// Copyright 16-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "kut/bytes.h"
#include "kut/DEFS.h"
#include "kut/b64.h"

struct bytes_Bytes {
  unsigned char *bs;
  int length;
};

Bytes *bytes_new() {
  return bytes_new_bf(0);
}

Bytes *bytes_new_bf(int length) {
  Bytes *this = MALLOC(Bytes);
  this->bs = ATOMIC(length);
  this->length = length;
  return this;
}

Bytes *bytes_from_bytes(unsigned char *bs, int length) {
  Bytes *this = MALLOC(Bytes);
  this->bs = ATOMIC(length);
  memcpy(this->bs, bs, length);
  this->length = length;
  return this;
}

Bytes *bytes_from_str (char *s) {
  return bytes_from_bytes((unsigned char *)s, strlen(s));
}

unsigned char *bytes_bs(Bytes *this) {
  return this->bs;
}

int bytes_len(Bytes *this) {
  return this->length;
}

void bytes_add_bytes (Bytes *this, unsigned char *bs, int length) {
  int sum = this->length + length;
  if (sum) {
    this->bs = GC_REALLOC(this->bs, sum);
    memcpy(this->bs + this->length, bs, length);
    this->length = sum;
  }
}

void bytes_add (Bytes *this, Bytes *another) {
  bytes_add_bytes(this, another->bs, another->length);
}

void bytes_add_str (Bytes *this, char *s) {
  bytes_add_bytes(this, (unsigned char *)s, strlen(s));
}

char *bytes_to_str(Bytes *this) {
  int len = this->length;
  char *r = ATOMIC(len + 1);
  memcpy(r, this->bs, len);
  r[len] = 0;
  return r;
}

char *bytes_to_js(Bytes *this) {
  char *b64 = b64_encode_bytes(this);
  size_t len = strlen(b64);
  char *r = ATOMIC(len + 3);
  r[0] = '"';
  memcpy(r + 1, b64, len);
  r[len + 1] = '"';
  r[len + 2] = 0;
  return r;
}

Bytes *bytes_from_js(char *js) {
  size_t len = strlen(js) - 2;
  char *b64 = ATOMIC(len + 1);
  memcpy(b64, js + 1, len);
  b64[len] = 0;

  return b64_decode_bytes(b64);
}
