// Copyright 20-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "path_tests.h"
#include "kut/DEFS.h"
#include "kut/str.h"
#include "kut/arr.h"
#include "kut/buf.h"
#include "kut/opt.h"
#include "kut/sys.h"

void str_tests(void) {
  puts(">>> str:");

  // starts - ends

  assert(str_starts("ab", ""));
  assert(str_starts("", ""));
  assert(str_starts("ab", "a"));
  assert(str_starts("ab", "ab"));
  assert(!str_starts("", "b"));
  assert(!str_starts("ab", "b"));
  assert(!str_starts("ab", "abc"));

  assert(str_ends("ab", ""));
  assert(str_ends("", ""));
  assert(str_ends("ab", "b"));
  assert(str_ends("ab", "ab"));
  assert(!str_ends("", "b"));
  assert(!str_ends("ab", "a"));
  assert(!str_ends("ab", "cab"));

  // index

  TESTI(str_cindex("ab", 'a'), 0);
  TESTI(str_cindex_from("ab", 'a', 1), -1);
  TESTI(str_cindex("", 'a'), -1);
  TESTI(str_cindex("ab", 'b'), 1);
  TESTI(str_cindex_from("ab", 'b', 1), 1);
  TESTI(str_cindex("ab", 'c'), -1);

  TESTI(str_index("ab", "a"), 0);
  TESTI(str_index("", "a"), -1);
  TESTI(str_index("ab", "b"), 1);
  TESTI(str_index("ab", "c"), -1);
  TESTI(str_index("ab", ""), 0);
  TESTI(str_index("", ""), 0);
  TESTI(str_index("ab", "abc"), -1);
  TESTI(str_index("abcd", "bc"), 1);
  TESTI(str_index("abcd", "bcd"), 1);
  TESTI(str_index("abcd", "d"), 3);
  TESTI(str_index_from("abcd", "d", 0), 3);
  TESTI(str_index_from("abcd", "d", 2), 3);
  TESTI(str_index_from("abcde", "d", 4), -1);
  TESTI(str_index("abcd", "ad"), -1);

  TESTI(str_last_cindex("ab", 'a'), 0);
  TESTI(str_last_cindex("aba", 'a'), 2);
  TESTI(str_last_cindex("", 'a'), -1);
  TESTI(str_last_cindex("ab", 'b'), 1);
  TESTI(str_last_cindex("ab", 'c'), -1);

  TESTI(str_last_index("ab", "a"), 0);
  TESTI(str_last_index("abac", "a"), 2);
  TESTI(str_last_index("", "a"), -1);
  TESTI(str_last_index("ab", "b"), 1);
  TESTI(str_last_index("abcb", "b"), 3);
  TESTI(str_last_index("ab", "c"), -1);
  TESTI(str_last_index("ab", ""), 2);
  TESTI(str_last_index("", ""), 0);
  TESTI(str_last_index("ab", "abc"), -1);
  TESTI(str_last_index("abcd", "bc"), 1);
  TESTI(str_last_index("abcdbc", "bc"), 4);
  TESTI(str_last_index("abcd", "bcd"), 1);
  TESTI(str_last_index("abcd", "d"), 3);
  TESTI(str_last_index("abcddd", "d"), 5);
  TESTI(str_last_index("abcd", "ad"), -1);

  // copy / new / cmp

  TEST(str_new_c('n'), "n");

  char *s0 = "";
  char *s1 = "1";

  char *r;

  r = str_new(s0);
  TEST(r, "");
  TESTI(str_len(r), 0);
  assert (!str_cmp(r, ""));
  assert (str_eq(r, ""));
  assert (str_cmp(r, "1"));
  assert (!str_eq(r, "1"));

  r = str_new(s1);
  TEST(r, "1");
  TESTI(str_len(r), 1);
  assert (str_cmp(r, ""));
  assert (!str_eq(r, ""));
  assert (!str_cmp(r, "1"));
  assert (str_eq(r, "1"));

  TESTI(str_len("avc"), 3);
  assert(str_cmp("n", "z") < 0);
  assert(str_cmp("z", "n") > 0);
  assert(!str_greater("n", "z"));
  assert(str_greater("z", "n"));

  sys_set_locale("es_ES.utf8");
  TESTI(str_cmp_locale("é", "n"), -68);
  assert(!str_greater_locale("é", "n"));
  sys_set_locale("C");
  TESTI(str_cmp_locale("é", "n"), 85);
  assert(str_greater_locale("é", "n"));


  r = str_f("%s%s", s0, s1);
  TEST(r, "1");

  char *r2 = str_f("%s%s", r, "ab");
  TEST(r2, "1ab");
  r = str_f("%s%s%s%s%s%s%s", r2, "ab", "", "cd", "ab", s1, "cd");
  TEST(r, "1ababcdab1cd");

  // sub

  r = str_left("ab", 1);
  TEST(r, "a");
  r = str_f("%s%s", r, "b");

  r = str_sub(r, -2, -1);
  TEST(r, "a");
  r = str_f("%s%s", r, "b");
  r = str_sub(r, 0, 0);
  TEST(r, "");
  r = str_f("%s%s", r, "ab");
  r = str_sub(r, -1, 0);
  TEST(r, "");
  r = str_f("%s%s", r, "ab");
  r = str_sub(r, 0, 35);
  r = str_f("%s%s", r, "ab");
  r = str_sub(r, 3, 3);
  TEST(r, "");
  r = str_f("%s%s", r, "ab");
  TEST(r, "ab");
  r = str_sub(r, 2, 2);
  TEST(r, "");
  r = str_f("%s%s", r, "ab");
  r = str_sub(r, 0, 2);
  TEST(r, "ab");
  r = str_sub(r, 1, 2);
  TEST(r, "b");
  r = str_right(r, 20);
  r = str_left(r, 0);
  TEST(r, "");
  r = str_f("%s%s", r, "ab");
  r = str_right(r, 0);
  TEST(r, "ab");
  r = str_right(r, -1);
  TEST(r, "b");

  // trim

  r = str_trim("nothing to trim");
  TEST(r, "nothing to trim");
  r = str_trim("trim the back     ");
  TEST(r, "trim the back");
  r = str_trim(" trim one char front and back ");
  TEST(r, "trim one char front and back");
  r = str_trim(" trim one char front");
  TEST(r, "trim one char front");
  r = str_trim("trim one char back ");
  TEST(r, "trim one char back");
  r = str_trim("                   ");
  TEST(r, "");
  r = str_trim(" ");
  TEST(r, "");
  r = str_trim("a");
  TEST(r, "a");
  r = str_trim("");
  TEST(r, "");
  r = str_trim("nothing to trim");
  TEST(r, "nothing to trim");
  r = str_ltrim("trim the back     ");
  TEST(r, "trim the back     ");
  r = str_ltrim(" trim one char front and back ");
  TEST(r, "trim one char front and back ");
  r = str_ltrim(" trim one char front");
  TEST(r, "trim one char front");
  r = str_ltrim("trim one char back ");
  TEST(r, "trim one char back ");
  r = str_ltrim("                   ");
  TEST(r, "");
  r = str_ltrim(" ");
  TEST(r, "");
  r = str_ltrim("a");
  TEST(r, "a");
  r = str_ltrim("");
  TEST(r, "");
  r = str_rtrim("nothing to trim");
  TEST(r, "nothing to trim");
  r = str_rtrim("trim the back     ");
  TEST(r, "trim the back");
  r = str_rtrim(" trim one char front and back ");
  TEST(r, " trim one char front and back");
  r = str_rtrim(" trim one char front");
  TEST(r, " trim one char front");
  r = str_rtrim("trim one char back ");
  TEST(r, "trim one char back");
  r = str_rtrim("                   ");
  TEST(r, "");
  r = str_rtrim(" ");
  TEST(r, "");
  r = str_rtrim("a");
  TEST(r, "a");
  r = str_rtrim("");
  TEST(r, "");

  // split / join

  // <char>
  Arr *a;
  char *tx0 = "";
  char *tx01 = ";";
  char *tx1 = "ab";
  char *tx2 = "ab;";
  char *tx3 = "ab;c;de";
  char *tx01b = ";--";
  char *tx2b = "ab;--";
  char *tx3b = "ab ;--  c \t ;--  de";
  char *tx3c = "ab;--c;--de";


  a = str_csplit(tx0, ';');
  r = arr_cjoin(a, ';');
  TEST(r, tx0);
  a = str_csplit(tx01, ';');
  r = arr_cjoin(a, ';');
  TEST(r, ";");
  a = str_csplit(tx1, ';');
  r = arr_cjoin(a, ';');
  TEST(r, tx1);
  a = str_csplit(tx2, ';');
  r = arr_cjoin(a, ';');
  TEST(r, "ab;");
  a = str_csplit(tx3, ';');
  r = arr_cjoin(a, ';');
  TEST(r, tx3);
  a = str_split(tx0, ";");
  r = arr_join(a, ";");
  TEST(r, tx0);
  a = str_split(tx01, ";");
  r = arr_join(a, ";");
  TEST(r, ";");
  a = str_split(tx1, ";");
  r = arr_join(a, ";");
  TEST(r, tx1);
  a = str_split(tx2, ";");
  r = arr_join(a, ";");
  TEST(r, tx2);
  a = str_split(tx3, ";");
  r = arr_join(a, ";");
  TEST(r, tx3);
  a = str_split(tx0, ";--");
  r = arr_join(a, ";--");
  TEST(r, tx0);
  a = str_split(tx01b, ";--");
  r = arr_join(a, ";--");
  TEST(r, tx01b);
  a = str_split(tx1, ";--");
  r = arr_join(a, ";--");
  TEST(r, tx1);
  a = str_split(tx2b, ";--");
  r = arr_join(a, ";--");
  TEST(r, tx2b);
  a = str_split(tx3b, ";--");
  r = arr_join(a, ";--");
  TEST(r, tx3b);
  a = str_split_trim(tx3b, ";--");
  r = arr_join(a, ";--");
  TEST(r, tx3c);
  a = str_split("", "");
  assert(arr_size(a) == 0);
  r = arr_join(a, "");
  TEST(r, "");
  a = str_split("abñ", "");
  assert(arr_size(a) == 3);
  r = arr_join(a, "");
  TEST(r, "abñ");

  // replace

  r = str_new("");
  r = str_creplace(r, '.', ',');
  TEST(r, "");
  r = str_new(".");
  r = str_creplace(r, '.', ',');
  TEST(r, ",");
  r = str_new("f.j");
  r = str_creplace(r, '.', ',');
  TEST(r, "f,j");
  r = str_new("f.j.");
  r = str_creplace(r, '.', ',');
  TEST(r, "f,j,");

  r = str_replace("", ".", "");
  TEST(r, "");
  r = str_replace(".", ".", "");
  TEST(r, "");
  r = str_replace("f.j", ".", "");
  TEST(r, "fj");
  r = str_replace("f.j.", ".", "");
  TEST(r, "fj");

  r = str_replace("", "..", "---");
  TEST(r, "");
  r = str_replace("..", "..", "---");
  TEST(r, "---");
  r = str_replace("f..j", "..", "---");
  TEST(r, "f---j");
  r = str_replace("f.j..", "..", "---");
  TEST(r, "f.j---");

  r = str_f("");
  TEST(r, "");
  r = str_f("ab");
  TEST(r, "ab");
  r = str_f("Result is %d", 1);
  TEST(r, "Result is 1");
  r = str_f("Your %s number %d", "book", 1);
  TEST(r, "Your book number 1");
  r = str_f("%s%s%s", "1", "ab", "");
  TEST(r, "1ab");
  r = str_f("%s%s%s", "1", "ab", "c");
  TEST(r, "1abc");

  // utf8 / unicode / iso

  TESTI(str_runes_len(""), 0);
  TESTI(str_runes_len("a"), 1);
  TESTI(str_runes_len("\n"), 1);
  TESTI(str_runes_len("ú"), 1);
  TESTI(str_runes_len("ñ"), 1);
  TESTI(str_runes_len("€"), 1);
  TESTI(str_runes_len("a b"), 3);
  TESTI(str_runes_len("a\nb"), 3);
  TESTI(str_runes_len("añb"), 3);
  TESTI(str_runes_len("a€b"), 3);
  TESTI(str_runes_len(" a\tb €\n"), 7);

  char *s3 = "cañón | a\tb €\n";
  r = str_new("");
  char *ctmp = str_next_rune(&r, s3);
  Buf *bf = buf_new();
  while (*r) {
    buf_add(bf, r);
    ctmp = str_next_rune(&r, ctmp);
  }
  r = str_new(buf_str(bf));

  // <char>
  Arr *runes = str_runes("");
  TESTI(arr_size(runes), 0);
  runes = str_runes("a b");
  TESTI(arr_size(runes), 3);
  TEST(arr_join(runes, ""), "a b");
  runes = str_runes(" a\tb €\n");
  TESTI(arr_size(runes), 7);
  TEST(arr_join(runes, ""), " a\tb €\n");
  char *wrong = str_new(" a\tb €\n");
  wrong[0] = (char)255;
  runes = str_runes(wrong);
  TEST(arr_join(runes, ""), "�a\tb €\n");

  TEST(r, s3);

  unsigned *u = NULL;
  u = opt_get(str_to_unicode(""));
  r = opt_get(str_from_unicode(u));
  TEST("", r);
  u = opt_get(str_to_unicode("a"));
  r = opt_get(str_from_unicode(u));
  TEST("a", r);
  u = opt_get(str_to_unicode("\n"));
  r = opt_get(str_from_unicode(u));
  TEST("\n", r);
  u = opt_get(str_to_unicode("ú"));
  r = opt_get(str_from_unicode(u));
  TEST("ú", r);
  u = opt_get(str_to_unicode("ñ"));
  r = opt_get(str_from_unicode(u));
  TEST("ñ", r);
  u = opt_get(str_to_unicode("€"));
  r = opt_get(str_from_unicode(u));
  TEST("€", r);
  u = opt_get(str_to_unicode("a b"));
  r = opt_get(str_from_unicode(u));
  TEST("a b", r);
  u = opt_get(str_to_unicode("a\nb"));
  r = opt_get(str_from_unicode(u));
  TEST("a\nb", r);
  u = opt_get(str_to_unicode("a€b"));
  r = opt_get(str_from_unicode(u));
  TEST("a€b", r);
  u = opt_get(str_to_unicode("añb"));
  r = opt_get(str_from_unicode(u));
  TEST("añb", r);
  u = opt_get(str_to_unicode(" a\tb €\n"));
  r = opt_get(str_from_unicode(u));
  TEST(" a\tb €\n", r);

  r = str_new("1234");
  r[1] = 0xf1;
  r[2] = 0xe9;
  TEST(str_from_iso(r), "1ñé4");
  r = "";
  TEST(str_from_iso(r), "");

  // upper - lower

  sys_set_locale("es_ES.utf8");
  r = str_to_upper("cañón");
  TEST("CAÑÓN", r);
  r = str_to_lower(r);
  TEST("cañón", r);
  sys_set_locale("C");

  r = str_to_escape("cuña\\\"abc\"");
  TEST("\"cuña\\\\\\\"abc\\\"\"", r);
  r = str_from_escape(r);
  TEST("cuña\\\"abc\"", r);

  puts("... Finished");
}
