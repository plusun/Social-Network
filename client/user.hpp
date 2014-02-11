#ifndef _USER_
#define _USER_

#include <string>
#include <vector>
#include "../common/date.hpp"
#include "../common/userData.hpp"
#include "../background/userInfo.hpp"

/*
 * Class for user information
 */
class User
{
public:
  User();
  User(const std::string &name, const std::string &password);
  std::string name() const; // get name
  Date birthday() const; // get birthday
  bool birthday(Date); // set birthday, return true if success
  std::string gender() const; // get the gender
  bool gender(std::string); // set gender, return true if success
  std::string telephone() const; // get telephone number
  bool telephone(std::string); // set telephone
  std::string address() const; //get the address
  bool address(std::string); // set address
  bool password(std::string old, std::string now); // set password
  
  bool close(std::string pwd); // close this account
  bool valid(); // to check if it's new user
  void set(UserData &);
  void set(const std::string &, const std::string &);
  bool newAccount(const std::string &, const std::string &);
  bool open(const std::string &, const std::string &);
  bool exist(const std::string &);
  bool follow(const std::string &);
  bool unfollow(const std::string &);
  std::vector<std::string> follower();
  std::vector<std::string> following();
private:
  std::string encrypt(const std::string &);
  std::string user, pw; // name and password
  Date birth; // birthday
  Gender gend; // gender
  std::string tele, addr; // telephone and address
  UserData transfer() const; // transfer to userData type
  UserInfo server; // data on th server
  bool update();
};

#endif
