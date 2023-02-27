// Copyright 22-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "kut/cryp.h"
#include "kut/DEFS.h"
#include "kut/sys.h"
#include "kut/b64.h"

static char *b64_base =
  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char *cryp_genk (int lg) {
  if (lg <= 0)
    EXC_ILLEGAL_ARGUMENT("Value out of range", "Value < 0", str_f("%d", lg));

  char *r = ATOMIC(lg + 1);
  char *p = r + lg;
  *p-- = 0;
  while (lg--) {
    *p-- = *(b64_base + sys_rnd_i(64));
  }
  return r;
}

char *cryp_key (char *key, int lg) {
  if (!*key)
    EXC_ILLEGAL_ARGUMENT("Wrong key", "Value != \"\"", "\"\"");

  char *k0 = str_f(
    "%scodified in irreversibleDeme is good, very good!\n\r8@@", key);
  char *b64 = b64_encode(k0);
  Bytes *k = b64_decode_bytes(b64);

  unsigned char *ka = bytes_bs(k);
  size_t lenk = bytes_len(k);
  unsigned char sum = 0;
  unsigned char *p = ka;
  for (int i = 0; i < lenk; ++i) sum += *p++;

  size_t lg2 = lg + lenk;
  Bytes *rbs = bytes_new_bf(lg2);
  unsigned char *ra = bytes_bs(rbs);
  Bytes *rbs1 = bytes_new_bf(lg2);
  unsigned char *ra1 = bytes_bs(rbs1);
  Bytes *rbs2 = bytes_new_bf(lg2);
  unsigned char *ra2 = bytes_bs(rbs2);

  size_t ik = 0;
  size_t v1, v2, v3, v4;
  for (size_t i = 0; i < lg2; ++i) {
    v1 = ka[ik];
    v2 = v1 + ka[v1 % lenk];
    v3 = v2 + ka[v2 % lenk];
    v4 = v3 + ka[v3 % lenk];
    sum = sum + i + v4;
    ra1[i] = sum;
    ra2[i] = sum;
    ++ik;
    if (ik == lenk) {
      ik = 0;
    }
  }

  for (size_t i = 0; i < lg2; ++i) {
    v1 = ra2[i];
    v2 = v1 + ra2[v1 % lg2];
    v3 = v2 + ra2[v2 % lg2];
    v4 = v3 + ra2[v3 % lg2];
    sum = sum + v4;
    ra2[i] = sum;
    ra[i] = sum + ra1[i];
  }

  return str_left(b64_encode_bytes(rbs), lg);
}

char *cryp_encode (char *k, char *s) {
  if (!*k)
    EXC_ILLEGAL_ARGUMENT("Wrong key", "Value != \"\"", "\"\"");

  char *b64 = b64_encode(s);

  size_t lg = strlen(b64);
  char *k2 = cryp_key(k, lg);

  Bytes *rbs = bytes_new_bf(lg);

  unsigned char *prbs = bytes_bs(rbs);
  unsigned char *pk2 = (unsigned char *)k2;
  unsigned char *pb64 = (unsigned char *)b64;
  for(int i = 0; i < lg; ++i) *prbs++ = (*pk2++) + (*pb64++);

  return b64_encode_bytes(rbs);
}

char *cryp_decode (char *k, char *c) {
  if (!*k)
    EXC_ILLEGAL_ARGUMENT("Wrong key", "Value != \"\"", "\"\"");

  Bytes *bs = b64_decode_bytes(c);

  size_t lg = bytes_len(bs);
  char *k2 = cryp_key(k, lg);

  char *b64 = ATOMIC(lg + 1);

  unsigned char *pbs = bytes_bs(bs);
  unsigned char *pk2 = (unsigned char *)k2;
  char *pb64 = b64;
  for(int i = 0; i < lg; ++i) *pb64++ = *pbs++ - *pk2++;
  *pb64 = 0;

  return b64_decode(b64);
}
