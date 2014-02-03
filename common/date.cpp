#include "date.hpp"
#include "error.hpp"
#include <ctime>

#include <iostream>
using namespace std;

bool leapYear(size_t year)
{
  if (year % 400 == 0)
    return true;
  if (year % 100 == 0)
    return false;
  if (year % 4 == 0)
    return true;
  return false;
}

size_t days(size_t year, size_t month) // return max days that month
{
  switch (month)
    {
    case 1: case 3: case 5:
    case 7: case 8: case 10:
    case 12:
      return 31;
      break;
    case 2:
      return leapYear(year) ? 29 : 28;
      break;
    default:
      return 30;
      break;
    }
}
bool Date::check(size_t y, size_t m, size_t d)
{
  if (m < 1 || m > 12)
    return false;
  if (d < 1 || d > days(y, m))
    return false;
  time_t curr_time;
  struct tm *timeinfo;
  curr_time = time(NULL);
  timeinfo = localtime(&curr_time);
  timeinfo->tm_year += 1900;
  timeinfo->tm_mon += 1;
  if (y > (size_t)timeinfo->tm_year ||
      (y == (size_t)timeinfo->tm_year && m > (size_t)timeinfo->tm_mon) ||
      (y == (size_t)timeinfo->tm_year && m == (size_t)timeinfo->tm_mon && d > (size_t)timeinfo->tm_mday))
    return false;
  return true;
}

bool Date::set(size_t year, size_t month, size_t day)
{
  if (check(year, month, day))
    {
      y = year;
      m = month;
      d = day;
      return true;
    }
  else
    return false;
}

Date::Date(size_t year, size_t month, size_t day)
{
  if (!set(year, month, day))
    throw Error("Error date!");
}

size_t Date::year() const
{
  return y;
}

size_t Date::month() const
{
  return m;
}

size_t Date::day() const
{
  return d;
}

ostream & operator<<(ostream &os, const Date &date)
{
  os << date.y << "-" << date.m << "-" << date.d << flush;
  return os;
}
