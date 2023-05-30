// Copyright 25-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Date-time management.

#ifndef LKUT_TIME_H
  #define LKUT_TIME_H

#include <stdint.h>

/// Time is an integer.
typedef int64_t Time;

/// Returns the current time.
Time time_now (void);

/// Makes a new Date.
///   day  : Day in base 1 (1 to 31).
///   month: Month in base 0 (0 to 11).
///   year : Year with four digits.
Time time_new (int day, int month, int year);

/// Makes a new time.
///   day    : Day in base 1 (1 to 31).
///   month  : Month in base 0 (0 to 11).
///   year   : Year with four digits.
///   hour   : Hour (0 to 59)
///   minute : Minute (0 to 59)
///   second : Second (0 to 59)
Time time_new_time (
  int day, int month, int year, int hour, int minute, int second
);

/// Makes a date from a string is in format 'yyyymmdd'.
/// (month and day in base 1).
/// Throws EXC_ILLEGAL_ARGUMENT if 'date' is not a valid string.
Time time_from_str (char *date);

/// Makes a date from a string in format '[x]x/[x]x/[xx]xx'.
/// (month and day in base 1).
/// Throws EXC_ILLEGAL_ARGUMENT if 'date' is not a valid string.
Time time_from_iso (char *date);

/// Mmakes a date from a string in format '[x]x/[x]x/[xx]xx'.
/// (month and day in base 1).
/// Throws EXC_ILLEGAL_ARGUMENT if 'date' is not a valid string.
Time time_from_us (char *date);

/// Makes a date from a string is in format '[x]xSP[x]xSP[xx]xx'.
/// If 'data' is not valid, returns '0'.
/// Throws EXC_ILLEGAL_ARGUMENT if 'date' is not a valid string.
Time time_from_iso_sep (char *date, char sep);

/// Makes a date from a string in format '[x]xSP[x]xSP[xx]xx'.
/// If 'data' is not valid, returns '0'.
/// Throws EXC_ILLEGAL_ARGUMENT if 'date' is not a valid string.
Time time_from_us_sep (char *date, char sep);

/// Returns month day.
int time_day (Time this);

/// Returns month (0-11).
int time_month (Time this);

/// Returns year.
int time_year (Time this);

/// Returns week day (0 - 6 => Sunday - Saturday)
int time_week_day (Time this);

/// Returns the day of year.
int time_year_day (Time this);

/// Returns hour.
int time_hour (Time this);

/// Returns minute.
int time_minute (Time this);

/// Returns second.
int time_second (Time this);

/// Restuns millisecond
int time_millisecond (Time this);

/// Returns a new Time adding this + days.
/// NOTE: For add milliseconds use directly 'this + days" (Example: 'tm + 234').
Time time_add (Time this, int days);

/// Returns the days difference between this and tm (t1 - t2).
/// NOTE: For calculate the difference in milliseconds use directly 't1 - t2".
int time_df (Time t1, Time t2);

/// Returns TRUE if t1 and t2 are times of the same day.
/// NOTE: For milliseconds use directly 't1 == t2".
int time_eq (Time t1, Time t2);

/// Formats a Time. Format can be:
///   %a     The abbreviated name of the day of the week according to the
///          current locale.
///   %A     The full name of the day of the week according to the current
///          locale.
///   %b     The abbreviated month name according to the current locale.
///   %B     The full month name according to the current locale.
///   %c     The preferred date and time representation for the current
///          locale.
///   %C     The century number (year/100) as a 2-digit integer. (SU)
///   %d     The day of the month as a decimal number (range 01 to 31).
///   %D     Equivalent to %m/%d/%y.  (Yecch—for Americans only.  Americans
///          should note that in other countries %d/%m/%y is rather common.
///          This means that in international context this format is
///          ambiguous and should not be used.) (SU).
///   %e     Like %d, the day of the month as a decimal number, but a
///          leading zero is replaced by a space. (SU).
///   %F     Equivalent to %Y-%m-%d (the ISO 8601 date format). (C99).
///   %G     The ISO 8601 week-based year (see NOTES) with century as a
///          decimal number.  The 4-digit year corresponding to the ISO
///          week number (see %V).  This has the same format and value as
///          %Y, except that if the ISO week number belongs to the previous
///          or next year, that year is used instead. (TZ).
///   %g     Like %G, but without century, that is, with a 2-digit year
///          (00-99). (TZ).
///   %h     Equivalent to %b.  (SU).
///   %H     The hour as a decimal number using a 24-hour clock (range 00
///          to 23).
///   %I     The hour as a decimal number using a 12-hour clock (range 01
///          to 12).
///   %j     The day of the year as a decimal number (range 001 to 366).
///   %k     The hour (24-hour clock) as a decimal number (range 0 to 23);
///          single digits are preceded by a blank.  (See also %H.)  (TZ)
///   %l     The hour (12-hour clock) as a decimal number (range 1 to 12);
///          single digits are preceded by a blank.  (See also %I.)  (TZ)
///   %m     The month as a decimal number (range 01 to 12).
///   %M     The minute as a decimal number (range 00 to 59).
///   %n     A newline character. (SU)
///   %O     Modifier: use alternative format, see below. (SU)
///   %p     Either "AM" or "PM" according to the given time value, or the
///          corresponding strings for the current locale.  Noon is treated
///          as "PM" and midnight as "AM".
///   %P     Like %p but in lowercase: "am" or "pm" or a corresponding
///          string for the current locale. (GNU)
///   %r     The time in a.m. or p.m. notation.  In the POSIX locale this
///          is equivalent to %I:%M:%S %p.  (SU)
///   %R     The time in 24-hour notation (%H:%M).  (SU) For a version
///          including the seconds, see %T below.
///   %s     The number of seconds since the Epoch, 1970-01-01 00:00:00
///          +0000 (UTC). (TZ)
///   %S     The second as a decimal number (range 00 to 60).  (The range
///          is up to 60 to allow for occasional leap seconds).
///   %t     A tab character. (SU).
///   %T     The time in 24-hour notation (%H:%M:%S). (SU).
///   %u     The day of the week as a decimal, range 1 to 7, Monday being
///          1.  See also %w.  (SU)
///   %U     The week number of the current year as a decimal number, range
///          00 to 53, starting with the first Sunday as the first day of
///          week 01.  See also %V and %W.
///   %V     The ISO 8601 week number (see NOTES) of the current year as a
///          decimal number, range 01 to 53, where week 1 is the first week
///          that has at least 4 days in the new year.  See also %U and %W.
///          (SU).
///   %w     The day of the week as a decimal, range 0 to 6, Sunday being
///          0.  See also %u.
///   %W     The week number of the current year as a decimal number, range
///          00 to 53, starting with the first Monday as the first day of
///          week 01.
///   %x     The preferred date representation for the current locale
///          without the time.
///   %X     The preferred time representation for the current locale
///          without the date.
///   %y     The year as a decimal number without a century (range 00 to
///          99).
///   %Y     The year as a decimal number including the century.
///   %z     The +hhmm or -hhmm numeric timezone (that is, the hour and
///          minute offset from UTC). (SU).
///   %Z     The timezone name or abbreviation.
///   %%     A literal '%' character.
char *time_f (Time this, char *template);

/// Returns a string in format 'yyyymmdd'.
char *time_to_str (Time this);

/// Returns a string in format 'dd/mm/yyyy'.
char *time_to_iso (Time this);

/// Returns a string in format 'mm/dd/yyyy'.
char *time_to_us (Time this);


#endif
