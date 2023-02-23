// Copyright 21-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Map object

#ifndef LKUT_SYS_H
  #define LKUT_SYS_H

/// Initializes the system.
/// It should be called only one time at the beginning of the program.
void sys_init(void);

/// Sets LC_ALL, for example:
///   sys_set_locale("es_ES.utf8")
void sys_set_locale (char *language);

/// Returns the current locale.
char *sys_get_locale (void);

/// Generates a new double between 0.0 (inclusive) and 1.0 (exclusive).
/// For working, 'sys_init()' must be calle before.
double sys_rnd_d (void);

/// Generates a new int between 0 (inclusive) and 'top' (exclusive).
/// For working, 'sys_init()' must be calle before.
int sys_rnd_i (int top);

/// Stops the current thread.
void sys_sleep (int millis);

#endif
