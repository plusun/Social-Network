#include <iostream>
#include <cstring>

using std::cout;
using std::endl;

#include "userInfo.hpp"
#include "../common/error.hpp"

void output(userData &u)
{
  cout << u.user << " " << u.pw << " " << u.gend << " " << u.y
       << " " << u.m << " " << u.d << " " << u.tele << " " << u.addr << endl;
}

int main(void)
{
  userData a;
  strcpy(a.user, "hello");
  strcpy(a.pw, "1");
  a.gend = male;
  a.y = 1;
  a.m = 2;
  a.d = 3;
  strcpy(a.tele, "1");
  strcpy(a.addr, "2");

  userData b;
  strcpy(b.user, "world");
  strcpy(b.pw, "2");
  b.gend = female;
  b.y = 3;
  b.m = 4;
  b.d = 2;
  strcpy(b.tele, "3");
  strcpy(b.addr, "1");

  userData c;
  strcpy(c.user, "f");
  strcpy(c.pw, "-");
  c.gend = female;
  c.y = 343;
  c.m = 4343;
  c.d = 22;
  strcpy(c.tele, "3434");
  strcpy(c.addr, "113");

  userInfo user;
  user.newAccount(a);
  user.newAccount(b);
  user.newAccount(c);

  userData d;
  strcpy(d.user, "world");
  strcpy(d.pw, "2");
  user.get(d);
  if (memcmp(&d, &b, sizeof d) != 0)
    throw Error("damn..");

  userData e;
  d.y = 1000;
  memcpy(&e, &d, sizeof e);
  user.alter(d);

  userData dd;
  strcpy(dd.user, "world");
  strcpy(dd.pw, "2");
  user.get(dd);
  if (memcmp(&dd, &e, sizeof dd) != 0)
    {
      output(dd);
      output(e);
    }


  return 0;
}
