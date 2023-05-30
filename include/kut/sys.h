// Copyright 21-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// System utilities.

#ifndef LKUT_SYS_H
  #define LKUT_SYS_H

#include "rs.h"
#include "map.h"

/// Initializes the system.
/// It should be called only one time at the beginning of the program.
void sys_init(void);

/// Returns a Map<char> with the environment variables.
Map *sys_environ(void);

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

/// Returns the current user home directory.
int sys_user_id(void);

/// Returns the current user home directory.
/// If the current user can not be found, it returns "".
char *sys_user_name();

/// Returns the current user home directory.
/// If the current user can not be found, it returns "".
char *sys_user_home(void);

/// Executes 'command' and returns a Rs<char> with values: [stderr - stdout].
/// If the command can not be exectued returns the stderr message
/// "NOEXEC: <cmd>", where 'cmd' is 'command'.
Rs *sys_cmd(char *command);

/// Reads a line from console.
/// The trailing '\n' is not read.
/// Inputs using [Control] casts unpredicted results.
char *sys_read_line (void);

#endif
