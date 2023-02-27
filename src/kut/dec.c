// Copyright 24-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "kut/dec.h"
#include <math.h>
#include <locale.h>
#include "kut/DEFS.h"

char *dec_itos (long n) {
  return str_f("%ld", n);
}

char *dec_ftos (double n, int scale) {
  scale = scale < 0 ? 0 : scale > 9 ? 9 : scale;
  char *tpl = str_f("%%.%df", scale);
  char *loc = setlocale(LC_ALL, NULL);
  setlocale(LC_ALL, "C");
  char *ns = str_f(tpl, n);
  setlocale(LC_ALL, loc);
  if (str_starts(ns, "-0")) ns = str_right(ns, 1);
  return ns;
}

int dec_stoi (char *s) {
  return dec_stol(s);
}

long dec_stol (char *s) {
  s = str_trim(s);
  if (!*s)
    EXC_ILLEGAL_ARGUMENT("Bad number", "An integer", "A empty string")

  char *tmp;
  long r = strtol(s, &tmp, 0);
  if (*tmp)
    EXC_ILLEGAL_ARGUMENT("Bad number", "A valid number", s)

  return r;
}

float dec_stof (char *s) {
  return dec_stod(s);
}

double dec_stod (char *s) {
  s = str_trim(s);
  if (!*s)
    EXC_ILLEGAL_ARGUMENT("Bad number", "A double", "A empty string")

  if (str_starts(s, "-0")) s = str_right(s, 1);
  char *tmp;
  double r = strtod(s, &tmp);
  if (*tmp)
    EXC_ILLEGAL_ARGUMENT("Bad number", "A valid number", s)

  return r;
}

char *dec_to_locale (double n, int scale) {
  scale = scale < 0 ? 0 : scale > 9 ? 9 : scale;
  char *tpl = str_f("%%'.%df", scale);
  char *ns = str_f(tpl, n);
  if (str_starts(ns, "-0")) ns = str_right(ns, 1);
  return ns;
}

double dec_from_iso (char *s) {
  return dec_stod(str_replace(str_replace(s, ".", ""), ",", "."));
}

double dec_from_en (char *s) {
  return dec_stod(str_replace(s, ",", ""));
}

int dec_digits (char *s) {
  if (!*s) return FALSE;
  char *ps = s;
  char ch;
  while ((ch = *ps++)) if (ch < '0' || ch > '9') return FALSE;
  return TRUE;
}

double dec_round (double n, int scale) {
  switch (scale) {
    case 0: return round(n);
    case 1: return round (n * 10.0) / 10.0;
    case 2: return round (n * 100.0) / 100.0;
    case 3: return round (n * 1000.0) / 1000.0;
    case 4: return round (n * 10000.0) / 10000.0;
    case 5: return round (n * 100000.0) / 100000.0;
    case 6: return round (n * 1000000.0) / 1000000.0;
    case 7: return round (n * 10000000.0) / 10000000.0;
    case 8: return round (n * 100000000.0) / 100000000.0;
    case 9: return round (n * 1000000000.0) / 1000000000.0;
    default: if (scale < 0) return round(n);
             else return round (n * 1000000000.0) / 1000000000.0;
  }
}

int dec_eq (double n1, double n2) {
  return dec_eq_gap(n1, n2, 0.000001);
}

int dec_eq_gap (double n1, double n2, double gap) {
  gap = gap > 1 ? 1 : gap < 0.0000000001 ? 0.0000000001 : gap;
  return n1 < n2 + gap && n1 > n2 - gap;
}

double dec_abs (double n) {
  return n >= 0 ? n : -n;
}

double dec_pow (double base, double exp) {
  return pow(base, exp);
}

double dec_sqrt (double n) {
  return sqrt(n);
}
