// Copyright 25-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "path_tests.h"
#include "kut/DEFS.h"
#include "kut/regex.h"

void regex_tests(void) {
  puts(">>> regex:");

  TEST(opt_get(regex_replace("a", "asasa", "e")), "esese");
  TEST(opt_get(regex_replace(".", "as\nsa", "e")), "eeeee");
  TEST(opt_get(regex_replace("e", "asasa", "e")), "asasa");
  TEST(
    opt_get(regex_replace(
      "1-\\w*\n", "1-ab\n2-ab\n1-cde\n", "1-xx\n"
    )),
    "1-xx\n2-ab\n1-xx\n"
  );

  TEST(opt_get(regex_replace_ic("a", "asAsa", "e")), "esese");

  puts("... Finished");
}
