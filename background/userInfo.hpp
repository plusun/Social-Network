#ifndef _USER_INFO_
#define _USER_INFO_

#include "../common/userData.hpp"
#include <vector>
#include <string>

class UserInfo
{
public:
  UserInfo();
  bool exist(const char *);
  bool get(UserData &); // get the user's data
  bool newAccount(UserData &);
  bool alter(UserData &);
  bool remove(const char *);
  bool follow(const char *);
  bool unfollow(const char *);
  std::vector<std::string> follower();
  std::vector<std::string> following();
  bool valid();
private:
  bool get(size_t, UserData &);
  static UserData search(const char *, size_t &pos);
  size_t pos;
};

#endif
