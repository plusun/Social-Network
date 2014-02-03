#ifndef _DATE_
#define _DATE_

#include <cstddef>
#include <iostream>

class Date
{
private:
  size_t y, m, d; // year, month and day
public:
  bool set(size_t, size_t, size_t);
  bool check(size_t, size_t, size_t);
  Date(size_t = 1995, size_t = 1, size_t = 9);
  size_t year() const;
  size_t month() const;
  size_t day() const;
  friend std::istream& operator>>(std::istream &is, Date &date);
  friend std::ostream& operator<<(std::ostream &os, const Date &date);
};

std::ostream& operator<<(std::ostream &os, const Date &date);

#endif
