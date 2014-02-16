#ifndef _USER_INFO_
#define _USER_INFO_

#include "../common/userData.hpp"
#include "btree.hpp"
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
  bool message(const char *);
  bool forward(const Message &);
  std::vector<Package> list(size_t);
  std::vector<Package> list(const char *, size_t);
  std::vector<std::string> follower();
  std::vector<std::string> following();
  bool valid();
private:
  typedef BTree<char, size_t, MAXLEN, 1> Tree;
  static bool get(size_t, UserData &);
  static void put(size_t, const UserData &);
  static UserData search(const char *, size_t &pos);
  size_t pos;
};

#endif
