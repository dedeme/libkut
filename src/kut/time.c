// Copyright 25-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <time.h>
#include <sys/time.h>
#include "kut/time.h"
#include "kut/DEFS.h"
#include "kut/dec.h"

static time_t to_time (Time this) {
  return (time_t)(this / 1000);
}

static Time from_time (time_t tm) {
  return (Time)tm * 1000;
}

static int day_light_correction (int day, int month, int year) {
  struct tm t;
  memset(&t, 0, sizeof(struct tm));
  t.tm_year = year - 1900;
  t.tm_mon = month;
  t.tm_mday = day;
  t.tm_hour = 12;

  time_t t2 = mktime(&t);
  int h = localtime(&t2)->tm_hour;
  return 12 - h;
}

Time time_now (void) {
  struct timeval tm;
  gettimeofday(&tm, NULL);
  return tm.tv_sec * 1000 + tm.tv_usec / 1000;
}

Time time_new (int day, int month, int year) {
  struct tm t;
  memset(&t, 0, sizeof(struct tm));
  t.tm_year = year - 1900;
  t.tm_mon = month;
  t.tm_mday = day;
  t.tm_hour = 12 + day_light_correction(day, month, year);
  return from_time((time_t) mktime(&t));
}

Time time_new_time (
  int day, int month, int year, int hour, int minute, int second
) {
  struct tm t;
  memset(&t, 0, sizeof(struct tm));
  t.tm_year = year - 1900;
  t.tm_mon = month;
  t.tm_mday = day;
  t.tm_hour = hour + day_light_correction(day, month, year);
  t.tm_min = minute;
  t.tm_sec = second;

  return from_time((time_t) mktime(&t));
}

static Time from_str(char *date) {
  char tpl[5];
  memset (tpl, 0, 5);
  memcpy(tpl, date + 6, 2);
  int d = atoi(tpl);
  memcpy(tpl, date + 4, 2);
  int m = atoi(tpl);
  memcpy(tpl, date, 4);
  int y = atoi(tpl);
  return time_new(d, m - 1, y);
}

Time time_from_str (char *date) {
  if (strlen(date) != 8 && !dec_digits(date))
    EXC_ILLEGAL_ARGUMENT("Bad date", "Date string", date);
  return from_str(date);
}

static Time from_iso_us (int is_iso, char *date, char sep) {
  // <char>
  Arr *parts = str_csplit(date, sep);
  if (arr_size(parts) == 3) {
    char *d = arr_get(parts, (is_iso) ? 0 : 1);
    char *m = arr_get(parts, (is_iso) ? 1 : 0);
    char *y = arr_get(parts, 2);

    if (strlen(d) == 1) d = str_f("%s%s", "0", d);
    if (strlen(m) == 1) m = str_f("%s%s", "0", m);
    if (strlen(y) == 2) y = str_f("%s%s", "20", y);

    if (strlen(d) == 2 && strlen(m) == 2 && strlen(y) == 4)
      return from_str(str_f("%s%s%s", y, m, d));
  }
  EXC_ILLEGAL_ARGUMENT(
    "Bad date",
    str_f(
      "%s date string with separator '%c'",
      (is_iso) ? "ISO" : "'EN'", sep
    ),
    date
  );

  return 0; // Unreacheable
}

Time time_from_iso (char *date) {
  return from_iso_us(TRUE, date, '/');
}

Time time_from_us (char *date) {
  return from_iso_us(FALSE, date, '/');
}

Time time_from_iso_sep (char *date, char sep) {
  return from_iso_us(TRUE, date, sep);
}

Time time_from_us_sep (char *date, char sep) {
  return from_iso_us(FALSE, date, sep);
}

int time_day (Time this) {
  time_t t = to_time(this);
  return localtime(&t)->tm_mday;
}

int time_month (Time this) {
  time_t t = to_time(this);
  return localtime(&t)->tm_mon;
}

int time_year (Time this) {
  time_t t = to_time(this);
  return localtime(&t)->tm_year + 1900;
}

int time_year_day (Time this) {
  time_t t = to_time(this);
  return localtime(&t)->tm_yday;
}

int time_week_day (Time this) {
  time_t t = to_time(this);
  return localtime(&t)->tm_wday;
}

int time_hour (Time this) {
  time_t t = to_time(this);
  return localtime(&t)->tm_hour;
}

int time_minute (Time this) {
  time_t t = to_time(this);
  return localtime(&t)->tm_min;
}

int time_second (Time this) {
  time_t t = to_time(this);
  return localtime(&t)->tm_sec;
}

int time_millisecond (Time this) {
  return this % 1000;
}

Time time_add (Time this, int days) {
  return this + (Time)days * 86400000;
}

int time_df (Time t1, Time t2) {
  return (t1 / 86400000) - (t2 / 86400000);
}

int time_eq (Time t1, Time t2) {
  return (time_df(t1, t2)) ? FALSE : TRUE;
}

char *time_f (Time this, char *template) {
  char *s, *rs;
  time_t tthis = to_time(this);
  struct tm *t = localtime(&tthis);
  int size = 126;
  while (1) {
    rs = (char *)calloc(size, 1); // calloc ok
    if (strftime (rs, size, template, t)) {
      s = str_new(rs);
      free(rs); // free ok
      break;
    }
    free(rs); // free ok
    size += size;
  }
  return s;
}

char *time_to_str (Time this) {
  return time_f(this, "%Y%m%d");
}

char *time_to_iso (Time this) {
  return time_f(this, "%d/%m/%Y");
}

char *time_to_us (Time this) {
  return time_f(this, "%m-%d-%Y");
}
