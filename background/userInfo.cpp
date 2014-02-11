#include "userInfo.hpp"
#include "file.hpp"
#include "../common/error.hpp"
#include "btree.hpp"
#include "relation.hpp"

#include <fstream>
#include <cstring>
#include <iostream>
#include <strings.h>

using std::ios_base;
using std::fstream;
using std::ifstream;
using std::ofstream;
using std::ofstream;
using std::vector;
using std::string;

UserInfo::UserInfo(): pos(MAXACC)
{
  fstream index;
  if (!openOrCreate(index, indexFile, ios_base::in | ios_base::out))
    throw Error("Error on openning index file.");
  index.seekg(0);
  size_t n = MAXACC;
  index.read((char *)&n, sizeof n);
  if (!index || n == MAXACC)
    {
      index.clear();
      n = 0;
      index.seekp(0);
      index.write((char *)&n, sizeof n);
      fstream os;
      if (!openOrCreate(os, userFile, ios_base::in | ios_base::out))
	throw Error("File error of user's data!");
      os.seekp(0);
      UserData empty;
      bzero(&empty, sizeof empty);
      os.write((char *)&empty, sizeof empty);
      os.close();
    }
  index.close();
}

UserData UserInfo::search(const char *user, size_t &pos)
{
  fstream is;
  if (!openOrCreate(is, userFile, ios_base::in | ios_base::out))
    throw Error("File error of user's data!");
  is.seekg(0);
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

bool UserInfo::get(size_t pos, UserData &user)
{
  fstream is;
  if (!openOrCreate(is, userFile, ios_base::in | ios_base::out))
    throw Error("Error on openning user's data file!");
  is.seekg(pos * sizeof user);
  is.read((char *)&user, sizeof user);
  if (!is)
    {
      is.close();
      return false;
    }
  else
    {
      is.close();
      return true;
    }

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
      n = 0;
      index.clear();
    }

  user.off = ++n;
  user.follower = user.following = user.message = MAXACC;
  if (n >= MAXACC)
    throw Error("Too many users!");

  fstream os;
  if (!openOrCreate(os, userFile, ios_base::in | ios_base::out | ios_base::app))
    throw Error("File error of user's date(write)!");

  os.seekp(user.off * sizeof user);

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
  UserData another;
  memcpy(another.user, user.user, MAXLEN * sizeof(char));
  if (!get(pos, another))
    return false;
  user.off = another.off;
  user.message = another.message;
  user.follower = another.follower;
  user.following = another.following;
  fstream os;
  if (!openOrCreate(os, userFile, ios_base::in | ios_base::out))
    throw Error("Error on opening user's data file!");
  os.seekp(pos * sizeof user);
  os.write((char *)&user, sizeof user);
  bool value = !!os;
  os.close();
  return value;
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

bool UserInfo::follow(const char *name)
{
  size_t p;
  UserData user = search(name, p);
  return (p != MAXACC) && Relation::follow(pos, p);
}

bool UserInfo::unfollow(const char *name)
{
  size_t p;
  search(name, p);
  return (p != MAXACC) && Relation::unfollow(pos, p);
}

vector<string> UserInfo::follower()
{
  vector<string> list;
  UserData user;
  
  if (!get(pos, user) || user.follower == MAXACC)
    return list;
  BTree<size_t, char, 1, 1> tree(followerFile, user.follower);
  BTree<size_t, char, 1, 1>::List l;
  tree.traversal(l);
  for (size_t i = 0; i < l.size(); ++i)
    if (l[i].key[0] > 0)
      {
	get(l[i].key[0], user);
	list.push_back(string(user.user));
      }
  return list;
}

vector<string> UserInfo::following()
{
  vector<string> list;
  UserData user;
  if (!get(pos, user) || user.following == MAXACC)
    return list;
  BTree<size_t, char, 1, 1> tree(followingFile, user.following);
  BTree<size_t, char, 1, 1>::List l;
  tree.traversal(l);
  for (size_t i = 0; i < l.size(); ++i)
    if (l[i].key[0] > 0)
      {
	get(l[i].key[0], user);
	list.push_back(string(user.user));
      }
  return list;
}
