// Copyright 23-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "kut/it.h"
#include "kut/DEFS.h"
#include "kut/tp3.h"

struct it_It {
  void *o;
  Opt *e;
  Opt *(*next)(void *o);
};

typedef Opt *(*it_Next)(void *);

It *it_new(
  void *o,
  Opt *(*next)(void *o)
) {
  It *this = MALLOC(It);
  this->o = o;
  this->next = next;
  this->e = next(o);
  return this;
}

static Opt *empty_next (void *o) { return opt_none(); }
It *it_empty (void) {
  return it_new(opt_none(), empty_next);
}

  //--
  typedef struct {
    void *e;
    int is_first;
  } it_unary_O;
  static Opt *unary_next(it_unary_O *o) {
    if (o->is_first) {
      o->is_first = 0;
      return opt_some(o->e);
    }
    return opt_none();
  }
It *it_unary (void *e) {
  it_unary_O *o = MALLOC(it_unary_O);
  o->e = e;
  o->is_first = 1;
  return it_new(o, (it_Next)unary_next);
}

  //--
  typedef struct {
    int i;
    int end;
  } it_range_O;
  static Opt *range_next (it_range_O *o) {
    int i = (o->i)++;
    if (i >= o->end) {
      return opt_none();
    }
    int *r = ATOMIC(sizeof(int));
    *r = i;
    return opt_some(r);
  }
It *it_range (int begin, int end) {
  it_range_O *o = MALLOC(it_range_O);
  o->i = begin;
  o->end = end;
  return it_new(o, (it_Next)range_next);
}

It *it_range0 (int end) {
  return it_range(0, end);
}

int it_has_next (It *this) {
  return opt_get(this->e) ? TRUE : FALSE;
}

static Opt *it_onext (It *this) {
  Opt *r = this->e;
  this->e = this->next(this->o);
  return r;
}

void *it_next (It *this) {
  return opt_get(it_onext(this));
}

void *it_peek (It *this) {
  return opt_get(this->e);
}

It *it_add(It *this, void *element) {
  return it_cat(this, it_unary(element));
}

It *it_add0(It *this, void *element) {
  return it_cat(it_unary(element), this);
}

  //--
  typedef struct {
    It *it1;
    It *it2;
  } it_cat_O;
  static Opt *cat_next (it_cat_O *o) {
    return (it_has_next(o->it1)) ? it_onext(o->it1)
      : (it_has_next(o->it2)) ? it_onext(o->it2) : opt_none();
  }
It *it_cat (It *this, It *another) {
  it_cat_O *o = MALLOC(it_cat_O);
  o->it1 = this;
  o->it2 = another;
  return it_new(o, (it_Next)cat_next);
}

  //--
  typedef struct {
    It *it;
    int i;
    int n;
  } it_take_O;
  static Opt *take_next (it_take_O *o) {
    if (o->i < o->n && it_has_next(o->it)) {
      o->i += 1;
      return it_onext(o->it);
    }
    else return opt_none();
  }
It *it_take (It *this, int n) {
  it_take_O *o = MALLOC(it_take_O);
  o->it = this;
  o->n = n;
  o->i = 0;
  return it_new(o, (it_Next)take_next);
}

  //--
  typedef struct {
    It *it;
    int (*f)(void *e);
  } it_takef_O;
  static Opt *takef_next (it_takef_O *o) {
    return it_has_next(o->it) && o->f(it_peek(o->it))
      ? it_onext(o->it) : opt_none();
  }
It *it_takef (It *this, int (*predicate)(void *e)) {
  it_takef_O *o = MALLOC(it_takef_O);
  o->it = this;
  o->f = predicate;
  return it_new(o, (it_Next)takef_next);
}

It *it_drop (It *this, int n) {
  int i = 0;
  while (it_has_next(this) && i++ < n)
    it_next(this);
  return this;
}

It *it_dropf (It *this, int (*predicate)(void *e)) {
  while (it_has_next(this) && predicate(it_peek(this)))
    it_next(this);
  return this;
}

  //--
  typedef struct {
    It *it;
    int (*f)(void *e);
  } it_filter_O;
  static Opt *filter_next (it_filter_O *o) {
    for(;;) {
      if (!it_has_next(o->it)) return opt_none();
      Opt *onext = it_onext(o->it);
      void *next = opt_get(onext);
      if (o->f(next)) return onext;
    }
  }
It *it_filter (It *this, int (*predicate)(void *e)) {
  it_filter_O *o = MALLOC(it_filter_O);
  o->it = this;
  o->f = predicate;
  return it_new(o, (it_Next)filter_next);
}

  //--
  typedef struct {
    It *it;
    void *(*f)(void *);
  } it_map_O;
  static Opt *map_next (it_map_O *o) {
    return it_has_next(o->it) ? opt_some(o->f(it_next(o->it))) : opt_none();
  }
It *it_map (It *this, void *(*converter)(void *e)) {
  it_map_O *o = MALLOC(it_map_O);
  o->it = this;
  o->f = converter;
  return it_new(o, (it_Next)map_next);
}

  //--
  typedef struct {
    It *it;
    int is_first;
    void *(*f1)(void *);
    void *(*f)(void *);
  } it_map2_O;
  static Opt *map2_next(it_map2_O *o) {
    if (!it_has_next(o->it)) return opt_none();
    if (o->is_first) {
      o->is_first = 0;
      return opt_some(o->f1(it_next(o->it)));
    }
    return opt_some(o->f(it_next(o->it)));
  }
It *it_map2 (It *this, void *(*conv1)(void *e), void *(*conv2)(void *e)) {
  it_map2_O *o = MALLOC(it_map2_O);
  o->it = this;
  o->is_first = 1;
  o->f1 = conv1;
  o->f = conv2;
  return it_new(o, (it_Next)map2_next);
}

  //--
  typedef struct {
    It *it1;
    It *it2;
  } it_zip_O;
  static Opt *zip_next(it_zip_O *o) {
    return it_has_next(o->it1) && it_has_next(o->it2)
      ? opt_some(tp_new(it_next(o->it1), it_next(o->it2)))
      : opt_none();
  }
It *it_zip (It *it1, It *it2) {
  it_zip_O *o = MALLOC(it_zip_O);
  o->it1 = it1;
  o->it2 = it2;
  return it_new(o, (it_Next)zip_next);
}

  //--
  typedef struct {
    It *it1;
    It *it2;
    It *it3;
  } it_zip3_O;
  static Opt *zip3_next(it_zip3_O *o) {
    return it_has_next(o->it1) && it_has_next(o->it2) && it_has_next(o->it3)
      ? opt_some(tp3_new(
          it_next(o->it1),
          it_next(o->it2),
          it_next(o->it3)
        ))
      : opt_none();
  }
It *it_zip3 (It *it1, It *it2, It *it3) {
  it_zip3_O *o = MALLOC(it_zip3_O);
  o->it1 = it1;
  o->it2 = it2;
  o->it3 = it3;

  return it_new(o, (it_Next)zip3_next);
}

It *it_reverse (It *this) {
  Arr *a = arr_from_it(this);
  arr_reverse(a);
  return arr_to_it(a);
}

It *it_sort (It *this, int (*comparator)(void *, void *)) {
  Arr *a = arr_from_it(this);
  arr_sort(a, comparator);
  return arr_to_it(a);
}

void it_each (It *this, void (*f)(void *e)) {
  while (it_has_next(this)) f(it_next(this));
}

void it_each_ix (It *this, void (*f)(void *e, int ix)) {
  int c = 0;
  while (it_has_next(this))
    f(it_next(this), c++);
}

int it_count (It *this) {
  int c = 0;
  while (it_has_next(this)) {
    it_next(this);
    ++c;
  }
  return c;
}

int it_eq (It *it1, It *it2, int (*feq)(void *e1, void *e2)) {
  while (it_has_next(it1) && it_has_next(it2)) {
    void *e1 = it_next(it1);
    void *e2 = it_next(it2);
    if (!feq(e1, e2))
      return 0;
  }
  if (it_has_next(it1) || it_has_next(it2))
    return 0;
  return 1;
}

int it_index (It *this, int (*predicate)(void *e)) {
  int ix = 0;
  while(it_has_next(this)) {
    if (predicate(it_next(this)))
      return ix;
    ++ix;
  }
  return -1;
}

int it_contains (It *this, int (*predicate)(void *e)) {
  while(it_has_next(this))
    if (predicate(it_next(this)))
      return 1;
  return 0;
}

int it_last_index (It *this, int (*predicate)(void *e)) {
  int r = -1;
  int ix = 0;
  while(it_has_next(this)) {
    if (predicate(it_next(this)))
      r = ix;
    ++ix;
  }
  return r;
}

Opt *it_find (It *this, int (*predicate)(void *e)) {
  while(it_has_next(this)) {
    void *next = it_next(this);
    if (predicate(next)) {
      return opt_some(next);
    }
  }
  return opt_none();
}

Arr *it_to (It *this) {
  return arr_from_it (this);
}

It *it_from (Arr *a) {
  return arr_to_it(a);
}

// Tp[Arr, Arr]
Tp *it_duplicates (It *this, int (feq)(void *e1, void *e2)) {
  Arr *d = arr_new();
  Arr *r = arr_new();
  void fn (void *e1) {
    int fcontains (void *e2) { return feq(e1, e2); }
    if (it_contains(it_from(r), fcontains)) {
      if (!it_contains(it_from(d), fcontains)) {
        arr_push(d, e1);
      }
    } else {
      arr_push(r, e1);
    }
  }
  it_each(this, fn);

  return tp_new(d, r);
}
