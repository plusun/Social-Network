#include "userInfo.hpp"
#include "file.hpp"
#include <fstream>
#include <cstring>
#include <iostream>
#include "../common/error.hpp"

using std::ios_base;
using std::fstream;
using std::ifstream;
using std::ofstream;
using std::ofstream;

const char *userFile = "user.dat";
const char *indexFile = "index.dat";
const char *followerFile = "follower.dat";
const char *followingFile = "following.dat";
const size_t MAXACC = (size_t)-1;

UserInfo::UserInfo(): pos(MAXACC) {}

UserData UserInfo::search(const char *user, size_t &pos)
{
  ifstream is;
  if (!openOrCreate(is, userFile, ios_base::in))
    throw Error("File error of user's data!");
  UserData read;
  pos = MAXACC;
  do
    {
      is.read((char *)&read, sizeof read);
      ++pos;
    }
  while (strcmp(read.user, user) != 0 && is);
  if (!is)
    {
      read.user[0] = '\0';
      pos = MAXACC;
    }
  is.close();
  return read;
}

bool UserInfo::exist(const char *name)
{
  size_t tmp;
  search(name, tmp);
  return tmp != MAXACC;
}

bool UserInfo::get(UserData &user)
{
  UserData read = search(user.user, pos);
  if (pos != MAXACC && strcmp(user.pw, read.pw) == 0)
    {
      memcpy(&user, &read, sizeof user);
      return true;
    }
  pos = MAXACC;
  return false;
}

bool UserInfo::newAccount(UserData &user)
{
  search(user.user, pos);
  if (pos != MAXACC)
    {
      pos = MAXACC;
      return false;
    }

  fstream index;
  if (!openOrCreate(index, indexFile, ios_base::in | ios_base::out))
    throw Error("FIle error of index!");
  size_t n;
  index.read((char *)&n, sizeof n);
  if (!index)
    {
      n = MAXACC;
      index.clear();
    }

  user.off = ++n;
  user.follower = user.following = user.message = MAXACC;
  if (n >= MAXACC)
    throw Error("Too many users!");

  ofstream os;
  if (!openOrCreate(os, userFile, ios_base::out | ios_base::app))
    throw Error("File error of user's date(write)!");

  os.write((char *)&user, sizeof user);
  os.close();
  if (os)
    {
      index.seekp(0);
      pos = n;
      index.write((char *)&n, sizeof n);
      if (!index)
	throw Error("Error on writing index file!");
      index.close();
      return true;
    }
  else
    {
      index.close();
      return false;
    }
}

bool UserInfo::alter(UserData &user)
{
  if (!valid())
    return false;
  ofstream os;
  if (!openOrCreate(os, userFile, ios_base::out))
    throw Error("Error on opening user's data file!");
  os.seekp(pos * sizeof user);
  os.write((char *)&user, sizeof user);
  return !!os;
}

bool UserInfo::remove(const char *name)
{
  if (!valid())
    return false;
  UserData empty;
  empty.off = MAXACC;
  strcpy(empty.user, name);
  strcpy(empty.pw, "");
  if (alter(empty))
    {
      pos = MAXACC;
      return true;
    }
  else
    return false;
}

bool UserInfo::valid()
{
  return pos != MAXACC;
}

