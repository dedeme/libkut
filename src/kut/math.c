// Copyright 24-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "kut/math.h"
#include <math.h>
#include <locale.h>
#include "kut/DEFS.h"

char *math_itos (int64_t n) {
  return str_f("%ld", n);
}

char *math_ftos (double n, int scale) {
  scale = scale < 0 ? 0 : scale > 9 ? 9 : scale;
  char *tpl = str_f("%%.%df", scale);
  char *ns = str_f(tpl, n + (n >= 0 ? 0.000000000001 : -0.000000000001));
  if (scale > 0) {
    char *p = ns;
    while (*p) {
      if (*p == ',') *p++ = '.';
      else ++p;
    }
    --p;
    while (*p == '0') --p;
    if (*p != '.') ++p;
    ns = str_left(ns, p - ns);
  }
  if (!strcmp(ns, "-0")) ns = "0";
  return ns;
}

int math_stoi (char *s) {
  return math_stol(s);
}

long math_stol (char *s) {
  s = str_trim(s);
  if (!*s)
    EXC_ILLEGAL_ARGUMENT("Bad number", "An integer", "A empty string");

  char *tmp;
  int64_t r = strtol(s, &tmp, 0);
  if (*tmp)
    EXC_ILLEGAL_ARGUMENT("Bad number", "A valid number", s);

  return r;
}

float math_stof (char *s) {
  return math_stod(s);
}

double math_stod (char *s) {
  s = str_trim(s);
  if (!*s)
    EXC_ILLEGAL_ARGUMENT("Bad number", "A double", "A empty string");

  struct lconv *lc = localeconv();
  int ix = str_cindex(s, '.');
  if (ix != -1) s[ix] = *lc->decimal_point;

  char *tmp;
  double r = strtod(s, &tmp);
  if (*tmp)
    EXC_ILLEGAL_ARGUMENT("Bad number", "A valid number", s);

  return r;
}

char *math_to_locale (double n, int scale) {
  scale = scale < 0 ? 0 : scale > 9 ? 9 : scale;
  char *tpl = str_f("%%'.%df", scale);
  char *ns = str_f(tpl, n);
  if (str_starts(ns, "-0")) ns = str_right(ns, 1);
  return ns;
}

double math_from_iso (char *s) {
  return math_stod(str_replace(str_replace(s, ".", ""), ",", "."));
}

double math_from_en (char *s) {
  return math_stod(str_replace(s, ",", ""));
}

int math_digits (char *s) {
  if (!*s) return FALSE;
  char *ps = s;
  char ch;
  while ((ch = *ps++)) if (ch < '0' || ch > '9') return FALSE;
  return TRUE;
}

double math_round (double n, int scale) {
  n = n >= 0 ? n + 0.00000000001 : n - 0.00000000001;
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

int math_eq (double n1, double n2) {
  return math_eq_gap(n1, n2, 0.0000001);
}

int math_eq_gap (double n1, double n2, double gap) {
  gap = gap > 1 ? 1 : gap < 0.0000000001 ? 0.0000000001 : gap;
  return n1 < n2 + gap && n1 > n2 - gap;
}

double math_abs (double n) {
  return n >= 0 ? n : -n;
}

double math_pow (double base, double exp) {
  return pow(base, exp);
}

double math_sqrt (double n) {
  return sqrt(n);
}
