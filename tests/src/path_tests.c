// Copyright 20-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "path_tests.h"
#include "kut/DEFS.h"
#include "kut/path.h"
#include "kut/opt.h"

void path_tests(void) {
  puts(">>> path:");

  TEST(path_cat("a", "b", NULL), "a/b");
  TEST(path_cat("a", "b", "c", NULL), "a/b/c");
  TEST(path_cat("a", "", NULL), "a/");
  TEST(path_cat("", "b", "", NULL), "b/");

  TEST(path_base(""), "");
  TEST(path_base("/"), "");
  TEST(path_base("ab"), "ab");
  TEST(path_base("/ab.c"), "ab.c");
  TEST(path_base("cd/"), "cd");
  TEST(path_base("c/ab.c"), "ab.c");

  TEST(path_cat(path_parent("a/b/"), path_base("a/b/"), NULL), "a/b");

  TEST(path_parent("ab"), "");
  TEST(path_parent("/ab.c"), "/");
  TEST(path_parent("cd/"), "");
  TEST(path_parent("cg/r/ab.c"), "cg/r");

  TEST(path_extension(""), "");
  TEST(path_extension("/"), "");
  TEST(path_extension("ab"), "");
  TEST(path_extension("/ab.c"), ".c");
  TEST(path_extension("cd/"), "");
  TEST(path_extension("cd/ab.c"), ".c");
  TEST(path_extension("cd/."), ".");
  TEST(path_extension("cd/f."), ".");

  TEST(path_clean(""), "");
  TEST(path_clean("/"), "/");
  TEST(path_clean("ab/.."), "");
  TEST(path_clean("ab/../ds/"), "ds");
  TEST(path_clean("/etc/apt/.//..//../etc"), "/etc");

  assert(!opt_get(path_canonical("/hom/dem")));
  assert(!opt_get(path_canonical("/bin/xx/.//..//../bin")));
  TEST(opt_get(path_canonical("/etc/apt/.//..//../etc")), "/etc");

  puts("... Finished");
}
