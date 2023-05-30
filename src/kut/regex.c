// Copyright 26-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <regex.h>
#include "kut/regex.h"
#include "kut/DEFS.h"
#include "kut/buf.h"

struct regex_RegexOffset {
  int begin;
  int end;
};

static RegexOffset *regexOffset_new(int begin, int end) {
  RegexOffset *this = MALLOC(RegexOffset);
  this->begin = begin;
  this->end = end;
  return this;
}

int regexOffset_begin (RegexOffset *off) {
  return off->begin;
}

/// Returns the offset end.
int regexOffset_end (RegexOffset *off) {
  return off->end;
}

// <RegexOffset>
static Arr *matches (regex_t *rex, char *s) {
  Arr *r = arr_new();

  regmatch_t match;

  int ix = 0;
  for (;;) {
    char *sub = str_right(s, ix);
    RegexOffset *offset = NULL;
    int rs = regexec(rex, sub, 1, &match, 0);
    if (rs == REG_ESPACE)
      EXC_GENERIC("Regular expression out of memory");

    if (!rs) {
      int end = ix + match.rm_eo;
      offset = regexOffset_new(ix + match.rm_so, end);
      ix = end;
    }

    if (offset) arr_push(r, offset);
    else break;
  }

  regfree(rex);
  return r;
}

// <Arr<RegexOffset>>
Opt *regex_matches (char *rex, char *s) {
  if (!*rex)
    EXC_ILLEGAL_ARGUMENT("Bad regular expresion", "An expresion", "An empty string");

  regex_t exp;
  if (regcomp(&exp, rex, REG_EXTENDED)) return opt_none();
  return opt_some(matches(&exp, s));
}

// <Arr<RegexOffset>>
Opt *regex_matches_ic (char *rex, char *s) {
  if (!*rex)
    EXC_ILLEGAL_ARGUMENT("Bad regular expresion", "An expresion", "An empty string");

  regex_t exp;
  if (regcomp(&exp, rex, REG_EXTENDED | REG_ICASE)) return opt_none();
  return opt_some(matches(&exp, s));
}

// matches is Arr<RegexOffset>
static char *replace (Arr *matches, char *s, char *replacement) {
  Buf *bf = buf_new();
  int ix = 0;
  EACH(matches, RegexOffset, rg) {
    buf_add(bf, str_sub(s, ix, rg->begin));
    buf_add(bf, replacement);
    ix = rg->end;
  }_EACH
  buf_add(bf, str_right(s, ix));
  return str_new(buf_str(bf));
}

// <char>
Opt *regex_replace (char *rex, char *s, char *replacement) {
  // <RegexOffset>
  Arr *matches = opt_get(regex_matches(rex, s));
  if (matches) return opt_some(replace(matches, s, replacement));
  return opt_none();
}

// <char>
Opt *regex_replace_ic (char *rex, char *s, char *replacement) {
  // <RegexOffset>
  Arr *matches = opt_get(regex_matches_ic(rex, s));
  if (matches) return opt_some(replace(matches, s, replacement));
  return opt_none();
}

// matches is Arr<RegexOffset>
static char *replacef (Arr *matches, char *s, char *(*frepl)(char *match)) {
  Buf *bf = buf_new();
  int ix = 0;
  EACH(matches, RegexOffset, rg) {
    buf_add(bf, str_sub(s, ix, rg->begin));
    buf_add(bf, frepl(str_sub(s, rg->begin, rg->end)));
    ix = rg->end;
  }_EACH
  buf_add(bf, str_right(s, ix));
  return str_new(buf_str(bf));
}

Opt *regex_replacef (char *rex, char *s, char *(*freplacement)(char *match)) {
  // <RegexOffset>
  Arr *matches = opt_get(regex_matches(rex, s));
  if (matches) return opt_some(replacef(matches, s, freplacement));
  return opt_none();
}
