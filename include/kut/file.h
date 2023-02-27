// Copyright 21-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Files and file system management.

#ifndef LKUT_FILE_H
  #define LKUT_FILE_H

#include <time.h>
#include "kut/arr.h"
#include "kut/bytes.h"
#include "kut/time.h"

///
typedef struct file_File File;

/// Returns a string with a not duplicate path whose template:
///   'dir' + '/' + 'path' + "xxxxxxxx"
/// If 'dir' is "", the current working directory is used.
/// NOTE: Before call this function must be called 'sys.init()'
char *file_tmp (char *dir, char *path);

/// Returns the working directory.
/// Throws EX_IO.
char *file_wd (void);

/// Sets the working directory.
/// Throws EX_IO.
void file_cd (char *path);

/// Makes a directory with 0755 permissions.
/// If parent directory does not exist it creates it.
/// If 'path' already exists it does nothing.
/// Throws EX_IO.
void file_mkdir (char *path);

/// fReturns an Arr<char> with names of files and directories existing in
/// 'path'.
/// Values '.' and '..' are not in the return.
/// Throws EX_IO.
Arr *file_dir (char *path);

/// Deletes file or directory named 'path' although it is a not empty directory.
/// If 'path' does not exists it does nothing.
/// Throws EX_IO.
void file_del (char *path);

/// Renames 'old_path' to 'new_path'.
/// Throws EX_IO.
void file_rename (char *old_path, char *new_path);

/// Makes a symbol link from 'link' to 'path'.
/// Throws EX_IO.
void file_link (char *path, char *link);

/// Returns TRUE if 'path' exists in the file system.
/// It fallows symbolic links.
int file_exists (char *path);

/// Returns TRUE if file is a directory.
/// It fallows symbolic links.
int file_is_directory (char *path);

/// Returns TRUE if file is a link.
/// It DOES NOT fallow symbolic links.
int file_is_link (char *path);

/// Returns TRUE if file is a regular file.
/// It fallows symbolic links.
int file_is_regular (char *path);

/// Returns information of 'path'.
/// Some fields to check in 'struct stat' are:
///   mode_t st_mode  - It can be tested with S_ISDIR(), S_ISREG or S_ISLNK
///                     among others. This macros are defined in sys/stat.h
///   uid_t st_uid    - User id
///   gid_t st_gid    - Group id
///   off_t st_size   - File size
///   time_t st_atime - Last access to file in seconds.
///   time_t st_mtime - Last file modification in seconds.
/// Throws EXC_IO if path does no exist or another error happends.
struct stat *file_info (char *path);

/// Returns the size of 'path'.
/// Throws EX_IO.
int file_size(char *path);

/// Returns the last access in seconds.
/// Throws EX_IO.
Time file_modified(char *path);

/// Reads data from 'path', including ends of line.
/// This function opens, reads and closes a file.
/// Throws EX_IO.
char *file_read (char *path);

/// Writes 'data' on 'path'.
/// This function opens, writes and closes a file.
/// Throws EX_IO.
void file_write (char *path, char *text);

/// Appends 'data' on 'path'.
/// This function opens, writes and closes a file.
/// Throws EX_IO.
void file_append (char *path, char *text);

/// Copy source to target.
/// Throws EX_IO.
void file_copy (char *source_path, char *target_path);

/// Opens a file to read with file_read_line or file_read_bin.
/// It returns a File object which will be freed when close is called.
/// Throws EX_IO.
File *file_ropen (char *path);

/// Opens a file to write with file_write_line or file_write_bin.
/// It returns a FileLck object which will be freed when close is called.
/// Throws EX_IO.
File *file_wopen (char *path);

/// Opens a file to append with file_write_line or file_write_bin.
/// It returns a FileLck object which will be freed when close is called..
/// Throws EX_IO.
File *file_aopen (char *path);

/// Reads a text file open with file_ropen.
/// It does not delete ends of line.
/// When reading is finished, returns a blank string.
/// Throws EX_IO.
char *file_read_line (File *lck);

/// Writes a text file open with file_wopen or file_aopen.
/// Throws EX_IO.
void file_write_text (File *lck, char *text);

/// Reads a binary file open with file_ropen.
/// When reading is finished, returns an empty Bytes.
/// Throws EX_IO.
Bytes *file_read_bin_buf (File *lck, int buffer);

/// file_read_bin is the same as 'file_read_bin_bf' using a buffer of 8192.
/// Throws EX_IO.
Bytes *file_read_bin (File *lck);

/// Writes a binary file open with file_wopen.
/// Returns 0 if there is no error.
/// Throws EX_IO.
void file_write_bin (File *lck, Bytes *bs);

/// Closes a file open with file_ropen, file_wopen or file_aopen.
void file_close (File *lck);

#endif
