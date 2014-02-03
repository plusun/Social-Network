#ifndef _USER_INFO_
#define _USER_INFO_

#include "../common/userData.hpp"

class UserInfo
{
public:
  UserInfo();
  bool exist(const char *);
  bool get(UserData &); // get the user's data
  bool newAccount(UserData &);
  bool alter(UserData &);
  bool remove(const char *);
  bool valid();
private:
  static UserData search(const char *, size_t &pos);
  size_t pos;
};

#endif
