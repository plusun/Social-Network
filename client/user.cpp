#include <cstring>
#include "user.hpp"
#include "../background/userInfo.hpp"

using std::string;
using std::vector;

string User::encrypt(const std::string &raw)
{
  return raw;
}

string User::name() const
{
  return user;
}

Date User::birthday() const
{
  return birth;
}

string User::gender() const
{
  return gend == male ? "male" : "female";
}

string User::telephone() const
{
  return tele;
}

string User::address() const
{
  return addr;
}

UserData User::transfer() const
{
  UserData u;
  bzero((char *)&u, sizeof u);
  strcpy(u.user, user.c_str());
  strcpy(u.pw, pw.c_str());
  u.gend = gend;
  u.y = birth.year();
  u.m = birth.month();
  u.d = birth.day();
  strcpy(u.tele, tele.c_str());
  strcpy(u.addr, addr.c_str());

  return u;
  
}

bool validName(const string &name)
{
  return name.length() > 0 && name.length() < MAXLEN;
}

void User::set(UserData &tmp)
{
  if (!server.get(tmp))
    return;
  user = tmp.user;
  pw = tmp.pw;
  birth = Date(tmp.y, tmp.m, tmp.d);
  gend = tmp.gend;
  tele = tmp.tele;
  addr = tmp.addr;
}

void User::set(const string &name, const string &password)
{
  if (!validName(name) || !validName(password))
    return;
  user = name;
  pw = encrypt(password);
  UserData tmp = transfer();
  set(tmp);
}

User::User(const string &name, const string &password)
{
  set(name, password);
}

User::User() {}

bool User::newAccount(const string &name, const string &password)
{
  if (!validName(name) || !validName(password))
    return false;
  user = name;
  pw = encrypt(password);
  UserData tmp = transfer();
  if (server.newAccount(tmp))
    {
      set(tmp);
      return true;
    }
  else
    return false;
}

bool User::valid()
{
  return server.valid();
}

bool User::update()
{
  UserData tmp = transfer();
  if (server.alter(tmp))
    return true;
  else
    return false;
}

bool User::birthday(Date date)
{
  Date tmp = birth;
  birth = date;
  if (update())
    return true;
  else
    {
      birth = tmp;
      return false;
    }
}

const size_t MAXGEND = 3;
string boy[] = {"male", "man", "boy"};
string girl[] = {"female", "woman", "girl"};

bool User::gender(string g)
{
  Gender tmp;
  size_t i;
  for (i = 0; i < MAXGEND; ++i)
    if (boy[i] == g)
      {
	tmp = male;
	break;
      }
    else if (girl[i] == g)
      {
	tmp = female;
	break;
      }
  if (i == 3)
    return false;
  Gender backup = gend;
  gend = tmp;
  if (update())
    return true;
  else
    {
      gend = backup;
      return false;
    }
}

bool User::telephone(string s)
{
  if (!validName(s))
    return false;
  string tmp = tele;
  tele = s;
  if (update())
    return true;
  else
    {
      tele = tmp;
      return false;
    }
}

bool User::address(string s)
{
  if (!validName(s))
    return false;
  string tmp = addr;
  addr = s;
  if (update())
    return true;
  else
    {
      addr = tmp;
      return false;
    }
}

bool User::password(string old, string now)
{
  old = encrypt(old);
  now = encrypt(now);
  if (old != pw)
    return false;
  string tmp = pw;
  pw = now;
  if (update())
    return true;
  else
    {
      pw = tmp;
      return false;
    }
}

bool User::close(string pwd)
{
  pwd = encrypt(pwd);
  if (pwd != pw)
    return false;
  return server.remove(name().c_str());
}

bool User::exist(const string &name)
{
  String tmp;
  bzero(tmp, MAXLEN);
  strcpy(tmp, name.c_str());
  return server.exist(tmp);
}

bool User::follow(const string &name)
{
  String tmp;
  bzero(tmp, MAXLEN);
  strcpy(tmp, name.c_str());
  return server.follow(tmp);
}

bool User::unfollow(const string &name)
{
  String tmp;
  bzero(tmp, MAXLEN);
  strcpy(tmp, name.c_str());  
  return server.unfollow(tmp);
}

vector<string> User::follower()
{
  return server.follower();
}

vector<string> User::following()
{
  return server.following();
}

bool User::message(const string &content)
{
  String tmp;
  bzero(tmp, MAXLEN);
  strcpy(tmp, content.c_str());  
  return server.message(tmp);
}

vector<Package> User::list(const size_t &number)
{
  return server.list(number);
}

vector<Package> User::list(const string &who, const size_t &number)
{
  String tmp;
  bzero(tmp, MAXLEN);
  strcpy(tmp, who.c_str());

  return server.list(tmp, number);
}

bool User::forward(const Message &message)
{
  return server.forward(message);
}

vector<UserData> User::find(const string &keyword)
{
  String tmp;
  bzero(tmp, MAXLEN);
  strcpy(tmp, keyword.c_str());
  vector<UserData> list = server.find(tmp);
  for (vector<UserData>::iterator itr = list.begin();
       !list.empty() && itr != list.end(); ++itr)
    if (string(itr->user) == user)
      {
	list.erase(itr);
	itr = list.begin();
      }
  return list;
}
