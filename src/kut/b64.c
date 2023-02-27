// Copyright 22-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "kut/b64.h"
#include <stdint.h>
#include "kut/DEFS.h"

static char encoding_table[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const unsigned char decoding_table[256] = {
  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
  52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
  64,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
  15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
  64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
  41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64,
  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
};
static int mod_table[] = {0, 2, 1};

static char *base64_encode(
  unsigned char *data,
  size_t input_length,
  size_t *output_length
) {
  *output_length = 4 * ((input_length + 2) / 3);
  char *encoded_data = ATOMIC(*output_length + 1);

  for (int i = 0, j = 0; i < input_length;) {
    uint32_t octet_a = i < input_length ? (unsigned char)data[i++] : 0;
    uint32_t octet_b = i < input_length ? (unsigned char)data[i++] : 0;
    uint32_t octet_c = i < input_length ? (unsigned char)data[i++] : 0;

    uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

    encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
    encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
    encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
    encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
  }

  for (int i = 0; i < mod_table[input_length % 3]; i++) {
    encoded_data[*output_length - 1 - i] = '=';
  }

  *(encoded_data + *output_length) = 0;
  return encoded_data;
}

// Only for positive input_length
static Bytes *base64_decode(
  const char *data,
  size_t input_length,
  size_t *output_length
) {
  if (input_length % 4 != 0)
    EXC_ILLEGAL_ARGUMENT(
      "Bad B64 string", "Length %% 4 != 0", str_f("Length %ld", input_length)
    );

  *output_length = input_length / 4 * 3;
  if (data[input_length - 1] == '=') (*output_length)--;
  if (data[input_length - 2] == '=') (*output_length)--;

  Bytes *bs = bytes_new_bf(*output_length);
  unsigned char *decoded_data = bytes_bs(bs);

  for (int i = 0, j = 0; i < input_length;) {
    uint32_t sextet_a = data[i] == '='
      ? 0 & i++
      : decoding_table[(int)data[i++]];
    uint32_t sextet_b = data[i] == '='
      ? 0 & i++
      : decoding_table[(int)data[i++]];
    uint32_t sextet_c = data[i] == '='
      ? 0 & i++
      : decoding_table[(int)data[i++]];
    uint32_t sextet_d = data[i] == '='
      ? 0 & i++
      : decoding_table[(int)data[i++]];

    uint32_t triple = (sextet_a << 3 * 6) +
      (sextet_b << 2 * 6) +
      (sextet_c << 1 * 6) +
      (sextet_d << 0 * 6);

    if (j < *output_length) {
      decoded_data[j++] = (triple >> 2 * 8) & 0xFF;
    }
    if (j < *output_length) {
      decoded_data[j++] = (triple >> 1 * 8) & 0xFF;
    }
    if (j < *output_length) {
      decoded_data[j++] = (triple >> 0 * 8) & 0xFF;
    }
  }
  return bs;
}

char *b64_decode(char *b64) {
  Bytes *bs = b64_decode_bytes(b64);
  int len = bytes_len(bs);
  char *s = ATOMIC(len + 1);
  memcpy(s, bytes_bs(bs), len);
  *(s + len) = 0;
  if (str_runes_len(s) == -1)
    return arr_join(str_runes(s), "");

  return s;
}

Bytes *b64_decode_bytes(char *b64) {
  if (*b64) {
    size_t len;
    return base64_decode(b64, strlen(b64), &len);
  } else {
    return bytes_new_bf(0);
  }
}

char *b64_encode(char *s) {
  size_t len;
  return base64_encode((unsigned char *)s, strlen(s), &len);
}

char *b64_encode_bytes(Bytes *bs) {
  size_t len;
  return base64_encode(bytes_bs(bs), bytes_len(bs), &len);
}
