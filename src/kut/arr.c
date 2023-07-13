// Copyright 20-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <stdarg.h>
#include "kut/arr.h"
#include "kut/DEFS.h"
#include "kut/buf.h"
#include "kut/js.h"
#include "kut/it.h"

struct arr_Arr {
  void **es; // Start elements.
  void **end; //End elements. (Elements are between 'es' (inclusive) and 'end'
             // exclusive.
  void **endbf; // End buffer.
};

Arr *arr_new (void) {
  return arr_new_bf(15);
}

Arr *arr_new_bf (int buffer) {
  Arr *this = MALLOC(Arr);
  void **es = GC_MALLOC(buffer * sizeof(void *));
  this->es = es;
  this->end = es;
  this->endbf = es + buffer;
  return this;
}

Arr *arr_new_from (void *e, ...) {
  va_list args;
  void *tmp;

  Arr *this = arr_new();
  arr_push(this, e);

  va_start(args, e);
  tmp = va_arg(args, void *);
  while (tmp) {
    arr_push(this, tmp);
    tmp = va_arg(args, void *);
  }
  va_end(args);

  return this;
}

Arr *arr_new_c (int size, void **es) {
  int bs_size= size * sizeof(void *);

  Arr *this = MALLOC(Arr);
  this->es = GC_MALLOC(bs_size + bs_size);
  this->end = this->es + size;
  this->endbf = this->es + size + size;
  memcpy(this->es, es, bs_size);
  return this;
}

Arr *arr_copy (Arr *this) {
  int buffer = this->endbf - this->es;
  int bf_size = buffer * sizeof(void *);
  Arr *r = MALLOC(Arr);
  void **es = GC_MALLOC(bf_size);
  r->es = es;
  r->end = es + (this->end - this->es);
  r->endbf = es + buffer;
  memcpy(es, this->es, bf_size);
  return r;
}

void **arr_begin (Arr *this) {
  return this->es;
}

void **arr_end (Arr *this) {
  return this->end;
}

int arr_size (Arr *this) {
  return this->end - this->es;
}

int arr_empty (Arr *this) {
  return this->es == this->end;
}

void *arr_get (Arr *this, int ix) {
  EXC_RANGE(ix, 0, arr_size(this) - 1);
  return *(this->es + ix);
}

void arr_push (Arr *this, void *e) {
  if (this->end == this->endbf) {
    int size = this->endbf - this->es;
    int new_size = size + size;
    this->es = GC_REALLOC(this->es, new_size * sizeof(void *));
    this->end = this->es + size;
    this->endbf = this->es + new_size;
  }
  *this->end++ = e;
}

void *arr_pop (Arr *this) {
  if (this->es >= this->end)
    EXC_ILLEGAL_STATE("Array is empty");

  --this->end;
  return *this->end;
}

void *arr_peek (Arr *this) {
  if (this->es >= this->end)
    EXC_ILLEGAL_STATE("Array is empty");

  return *(this->end - 1);
}

void arr_set (Arr *this, int ix, void *e) {
  EXC_RANGE(ix, 0, arr_size(this) - 1);

  void **p = this->es + ix;
  *p = e;
}

void arr_insert (Arr *this, int ix, void *e) {
  EXC_RANGE(ix, 0, arr_size(this));

  arr_push(this, e);
  void **p = this->end - 1;
  void **pix = this->es + ix;
  while (p > pix) {
    *p = *(p - 1);
    --p;
  }
  *p = e;
}

void arr_remove (Arr *this, int ix) {
  EXC_RANGE(ix, 0, arr_size(this) - 1);

  void **p = this->es + ix;
  void **p1 = p + 1;
  void **pend = this->end;
  while (p1 < pend) {
    *p++ = *p1++;
  }
  --this->end;
}

void arr_cat (Arr *this, Arr *other) {
  int other_len = other->end - other->es;
  if (other_len) {
    int this_len = this->end - this->es;
    int this_size = this->endbf - this->es;
    if (this_len + other_len >= this_size){
      int new_size = this_size + other_len;
      this->es = GC_REALLOC(this->es, new_size * sizeof(void *));
      this->end = this->es + this_len;
      this->endbf = this->es + new_size;
    }
    void **s = other->es;
    void **t = this->end;
    while (s < other->end) *t++ = *s++;
    this->end = t;
  }
}

void arr_insert_arr (Arr *this, int ix, Arr *other) {
  int this_len = this->end - this->es;
  EXC_RANGE(ix, 0, this_len);

  int other_len = other->end - other->es;
  if (other_len) {
    int this_size = this->endbf - this->es;
    int new_size = this_size;
    if (this_len + other_len >= this_size){
      new_size = this_size + other_len;

      void **es = GC_MALLOC(new_size * sizeof(void *));
      void **end = es;

      void **p = this->es;
      void **pend = p + ix;
      while (p < pend) {
        *end++ = *p++;
      }
      void **p2 = other->es;
      void **p2end = other->end;
      while (p2 < p2end) {
        *end++ = *p2++;
      }
      pend = this->end;
      while (p < pend) {
        *end++ = *p++;
      }

      this->es = es;
      this->end = end;
      this->endbf = es + new_size;
    } else {
      void **s = this->end - 1;
      void **t = s + other_len;
      this->end = t + 1;
      void **limit = this->es + ix;
      while (s >= limit) {
        *t-- = *s--;
      }
      s = other->es;
      t = this->es + ix;
      limit = other->end;
      while (s < limit) {
        *t++ = *s++;
      }
    }
  }
}

void arr_remove_range (Arr *this, int begin, int end) {
  int sz = arr_size(this);
  EXC_RANGE(end, 0, sz);
  EXC_RANGE(begin, 0, end);

  int df = end - begin;
  if (df == 0) {
    return;
  }

  void **pb = this->es + begin;
  void **pe = this->es + end;
  void **pend = this->end;
  while (pe < pend) {
    *pb++ = *pe++;
  }
  this->end -= df;
}

void arr_clear (Arr *this) {
  this->end = this->es;
}

void arr_reverse (Arr *this) {
  void **p = this->es;
  void **end = this->end - 1;
  void *tmp;
  for (int i = 0; i < (this->end - this->es) / 2; ++i) {
    tmp = *p;
    *p++ = *end;
    *end-- = tmp;
  }
}

void arr_sort (Arr *this, int (*greater)(void *, void *)) {
  void sort(void **a, int size) {
    if (size < 2) {
      return;
    }
    if (size == 2) {
      if (greater(a[0], a[1])) { void *tmp = a[0]; a[0] = a[1]; a[1] = tmp; }
      return;
    }
    if (size == 3) {
      if (greater(a[0], a[1])) { void *tmp = a[0]; a[0] = a[1]; a[1] = tmp; }
      if (greater(a[0], a[2])) { void *tmp = a[0]; a[0] = a[2]; a[2] = tmp; }
      if (greater(a[1], a[2])) { void *tmp = a[1]; a[1] = a[2]; a[2] = tmp; }
    }
    int mid1 = size >> 1;
    int mid2 = size - mid1;
    void *a1[mid1];
    void *a2[mid2];
    void **pa = a;
    void **pa1 = a1;
    void **pa2 = a2;
    for (int i = 0; i < mid1; ++i) *pa1++ = *pa++;
    for (int i = 0; i < mid2; ++i) *pa2++ = *pa++;
    sort(a1, mid1);
    sort(a2, mid2);

    pa = a;
    pa1 = a1;
    void **pa1_end = a1 + mid1;
    pa2 = a2;
    void **pa2_end = a2 + mid2;

    for(;;) {
      if (pa1 == pa1_end) {
        while (pa2 < pa2_end) *pa++ = *pa2++;
        break;
      }
      if (pa2 == pa2_end) {
        while (pa1 < pa1_end) *pa++ = *pa1++;
        break;
      }
      if (greater(*pa1, *pa2)) *pa++ = *pa2++;
      else *pa++ = *pa1++;
    }
  }

  sort(this->es, this->end - this->es);
}

void arr_shuffle (Arr *this) {
  int rnd_i (int max) {
    return (int)(((double)rand() / (double)RAND_MAX) * max);
  }
  void **es = this->es;
  int size = this->end - es;
  void **p = this->end - 1;
  void **pix, *tmp;
  while (p > es) {
    pix = es + rnd_i(size--);
    tmp = *p;
    *p-- = *pix;
    *pix = tmp;
  }
}

int arr_all (Arr *this, int (*pred)(void *e)) {
  void **p = this->es;
  while (p < this->end) if (!pred(*p++)) return 0;
  return 1;
}

int arr_any (Arr *this, int (*pred)(void *e)) {
  void **p = this->es;
  while (p < this->end) if (pred(*p++)) return 1;
  return 0;
}

void arr_each(Arr *this, void (*fn)(void *e)) {
  void **p = this->es;
  while (p < this->end) fn(*p++);
}

void arr_each_ix(Arr *this, void (*fn)(void *e, int ix)) {
  void **p = this->es;
  int ix = 0;
  while (p < this->end) fn(*p++, ix++);
}

int arr_eq(Arr *a1, Arr *a2, int (*feq)(void *e1, void *e2)) {
  if (arr_size(a1) != arr_size(a2)) return FALSE;
  void **p1 = a1->es;
  void **p2 = a2->es;
  while (p1 < a1->end) if (!feq(*p1++, *p2++)) return FALSE;
  return TRUE;
}

int arr_index (Arr *this, int (*pred)(void *e)) {
  void **es = this->es;
  void **p = es;
  while (p < this->end) if (pred(*p++)) return p - es - 1;
  return -1;
}

Opt *arr_find(Arr *this, int (*pred)(void *e)) {
  void **p = this->es;
  while (p < this->end) {
    void *e = *p++;
    if (pred(e)) return opt_some(e);
  }
  return opt_none();
}

Opt *arr_find_last(Arr *this, int (*pred)(void *e)) {
  Opt *r = opt_none();
  void **p = this->es;
  while (p < this->end) {
    void *e = *p++;
    if (pred(e)) r = opt_some(e);
  }
  return r;
}

int arr_last_index (Arr *this, int (*pred)(void *e)) {
  void **es = this->es;
  void **p = this->end;
  if (p > es) {
    while (--p > es) if (pred(*p)) return p - es;
    if (pred(*es)) return 0;
  }
  return -1;
}

void arr_filter_in (Arr *this, int (*pred)(void *e)) {
  void **p = this->es;
  void **end = this->end;
  void **new_end = p;
  while (p < end) {
    if (pred(*p)) {
      *new_end++ = *p;
    }
    ++p;
  }
  this->end = new_end;
}

Arr *arr_take (Arr *this, int n) {
  if (n > arr_size(this)) return arr_copy(this);
  Arr *r = arr_new();
  void **p = this->es;
  for (int i = 0; i < n; ++i) arr_push(r, *p++);
  return r;
}

Arr *arr_takef (Arr *this, int (*predicate)(void *e)) {
  Arr *r = arr_new();
  void **p = this->es;
  while (p < this->end) {
    if (predicate(*p)) arr_push(r, *p);
    else break;
    ++p;
  }
  return r;
}

Arr *arr_drop (Arr *this, int n) {
  if (n < 0) return arr_copy(this);
  if (n >= arr_size(this)) return arr_new();
  Arr *r = arr_new();
  void **p = this->es + n;
  while (p < this->end) arr_push(r, *p++);
  return r;
}

Arr *arr_dropf (Arr *this, int (*predicate)(void *e)) {
  Arr *r = arr_new();
  void **p = this->es;
  while (p < this->end) {
    if (!predicate(*p)) break;
    ++p;
  }
  while (p < this->end) arr_push(r, *p++);
  return r;
}

Arr *arr_filter_to (Arr *this, int (*predicate)(void *e)) {
  Arr *r = arr_new();
  void **p = this->es;
  while (p < this->end) {
    void *e = *p++;
    if (predicate(e)) arr_push(r, e);
  }
  return r;
}

Arr *arr_map (Arr *this, void *(*converter)(void *e)) {
  Arr *r = arr_new();
  void **p = this->es;
  while (p < this->end) arr_push(r, converter(*p++));
  return r;
}

Arr *arr_map2 (Arr *this, void *(*conv1)(void *e), void *(*conv2)(void *e)) {
  Arr *r = arr_new();
  void **p = this->es;
  void **end = this->end;
  if (p < end) arr_push(r, conv1(*p++));
  while (p < end) arr_push(r, conv2(*p++));
  return r;
}

void *arr_reduce(Arr *this, void *seed, void *(*fn)(void *seed, void *elem)) {
  void **p = this->es;
  while (p < this->end) seed = fn(seed, *p++);
  return seed;
}

Arr *arr_zip (Arr *a1, Arr *a2, void *(*converter)(void *e1, void *e2)) {
  Arr *r = arr_new();
  void **p1 = a1->es;
  void **p2 = a2->es;
  while (p1 < a1->end && p2 < a2->end) arr_push(r, converter(*p1++, *p2++));
  return r;
}

Arr *arr_zip3 (
  Arr *a1, Arr *a2, Arr *a3, void*(*converter)(void*e1, void*e2, void*e3)
) {
  Arr *r = arr_new();
  void **p1 = a1->es;
  void **p2 = a2->es;
  void **p3 = a3->es;
  while (p1 < a1->end && p2 < a2->end && p3 < a3->end)
    arr_push(r, converter(*p1++, *p2++, *p3++));
  return r;
}

Arr *arr_duplicates (Arr *this, int (feq)(void *e1, void *e2)) {
  Arr *r = arr_new();
  Arr *new = arr_new();
  void **p = this->es;
  while (p < this->end) {
    void *e = *p++;
    int unique = 1;
    void **newp = new->es;
    while (newp < new->end) {
      void *newe = *newp++;
      if (feq(e, newe)) {
        int rnew = 1;
        void **rp = r->es;
        while (rp < r->end) {
          void *re = *rp++;
          if (feq(e, re)) {
            rnew = 0;
            break;
          }
        }
        if (rnew) {
          arr_push(r, e);
        }
        unique = 0;
        break;
      }
    }
    if (unique) {
      arr_push(new, e);
    }
  }
  this->es = new->es;
  this->end = new->end;
  this->endbf = new->endbf;

  return r;
}

// a is Arr<char>
char *arr_cjoin(Arr *a, char sep) {
  Buf *bf = buf_new();
  int first = 1;
  char **p = (char **)a->es;
  while (p < (char **)a->end) {
    if (first) {
      first = 0;
    } else {
      buf_cadd(bf, sep);
    }
    buf_add(bf, *p++);
  }
  return str_new(buf_str(bf));
}

// a is Arr<char>
char *arr_join(Arr *a, char *sep) {
  Buf *bf = buf_new();
  int first = 1;
  char **p = (char **)a->es;
  while (p < (char **)a->end) {
    if (first) {
      first = 0;
    } else {
      buf_add(bf, sep);
    }
    buf_add(bf, *p++);
  }
  return str_new(buf_str(bf));
}

// -------------------------------------------------------------------------- //
typedef struct {                                                              //
  void **es;                                                                  //
  size_t n;                                                                   //
  size_t i;                                                                   //
} arr_to_it_O;                                                                //
static Opt *to_it_next(arr_to_it_O *o) {                                      //
  size_t i = o->i;                                                            //
  if (i < o->n) {                                                             //
    o->i += 1;                                                                //
    return opt_some(o->es[i]);                                                //
  } else {                                                                    //
    return opt_none();                                                        //
  }                                                                           //
}                                                                             //
// -------------------------------------------------------------------------- //
It *arr_to_it (Arr *this) {
  arr_to_it_O *o = MALLOC(arr_to_it_O);
  o->es = this->es;
  o->n = arr_size(this);
  o->i = 0;
  return it_new(o, (Opt *(*)(void*))to_it_next);
}

Arr *arr_from_it (It *it) {
  Arr *r = arr_new();
  while (it_has_next(it)) {
    arr_push(r, it_next(it));
  }
  return r;
}

char *arr_to_js (Arr *this, char *(*to)(void *e)) {
  return js_wa((Arr *)arr_map(this, (FMAP)to));
}

Arr *arr_from_js (char *js, void *(*from)(char *ejs)) {
  return arr_map(js_ra(js), (FMAP)from);
}
