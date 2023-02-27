// Copyright 20-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "kut/kob.h"
#include "kut/DEFS.h"
#include "kut/str.h"

struct kob_Kob {
  enum KobType type;
  void *value;
};

enum KobType kob_get_type(Kob *this) {
  return this->type;
}

Kob *kob_bool(int value) {
  Kob *this = MALLOC(Kob);
  int *val = ATOMIC(sizeof(int));
  *val = value;
  this->type = kob_BOOL;
  this->value = val;
  return this;
}

int kob_get_bool(Kob *this) {
  if (this->type != kob_BOOL)
    EXC_ILLEGAL_ARGUMENT (
      "Bad type",
      kob_type_to_str(kob_BOOL),
      kob_type_to_str(this->type)
    );

  return *((int *)this->value);
}

int kob_is_bool(Kob *this) {
  return this->type == kob_BOOL;
}

Kob *kob_int(int value) {
  Kob *this = MALLOC(Kob);
  int *val = ATOMIC(sizeof(int));
  *val = value;
  this->type = kob_INT;
  this->value = val;
  return this;
}

int kob_get_int(Kob *this) {
  if (this->type != kob_INT)
    EXC_ILLEGAL_ARGUMENT (
      "Bad type",
      kob_type_to_str(kob_INT),
      kob_type_to_str(this->type)
    );

  return *((int *)this->value);
}

int kob_is_int(Kob *this) {
  return this->type == kob_INT;
}

Kob *kob_float(double value) {
  Kob *this = MALLOC(Kob);
  double *val = ATOMIC(sizeof(double));
  *val = value;
  this->type = kob_FLOAT;
  this->value = val;
  return this;
}

double kob_get_float(Kob *this) {
  if (this->type != kob_FLOAT)
    EXC_ILLEGAL_ARGUMENT (
      "Bad type",
      kob_type_to_str(kob_FLOAT),
      kob_type_to_str(this->type)
    );

  return *((double *)this->value);
}

int kob_is_float(Kob *this) {
  return this->type == kob_FLOAT;
}

Kob *kob_str(char *s) {
  Kob *this = MALLOC(Kob);
  this->type = kob_STRING;
  this->value = s;
  return this;
}

char *kob_get_str(Kob *this) {
  if (this->type != kob_STRING)
    EXC_ILLEGAL_ARGUMENT (
      "Bad type",
      kob_type_to_str(kob_STRING),
      kob_type_to_str(this->type)
    );

  return (char *)this->value;
}

int kob_is_str(Kob *this) {
  return this->type == kob_STRING;
}

Kob *kob_arr(Arr *a) {
  Kob *this = MALLOC(Kob);
  this->type = kob_ARRAY;
  this->value = a;
  return this;
}

Arr *kob_get_arr(Kob *this) {
  if (this->type != kob_ARRAY)
    EXC_ILLEGAL_ARGUMENT (
      "Bad type",
      kob_type_to_str(kob_ARRAY),
      kob_type_to_str(this->type)
    );

  return (Arr *)this->value;
}

int kob_is_arr(Kob *this) {
  return this->type == kob_ARRAY;
}

char *kob_type_to_str (enum KobType type) {
  switch (type) {
    case kob_STRING:
      return "String";
    case kob_INT:
      return "Int";
    case kob_BOOL:
      return "Bool";
    case kob_FLOAT:
      return "Float";
    case kob_ARRAY:
      return "Array";
    case kob_MAP:
      return "Map";
  }
  EXC_ILLEGAL_ARGUMENT("Bad type identifier", str_f("%d", type), "(0 to 5)");
  return NULL;
}
