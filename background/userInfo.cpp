#include "userInfo.hpp"
#include "file.hpp"
#include "../common/error.hpp"
#include "btree.hpp"
#include "relation.hpp"
#include "message.hpp"

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

#define USING_BTREE

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
#ifdef USING_BTREE
      Tree tree(snapFile);
      size_t root = tree.offset();
      index.write((char *)&root, sizeof root);
#endif
      index.close();
      fstream os;
      if (!openOrCreate(os, userFile, ios_base::in | ios_base::out))
	throw Error("File error of user's data!");
      os.seekp(0);
      UserData empty;
      bzero(&empty, sizeof empty);
      os.write((char *)&empty, sizeof empty);
      os.close();
    }
  else
    index.close();
}

#define MIN(a, b) ((a) < (b) ? (a) : (b))

UserData UserInfo::search(const char *user, size_t &pos)
{
  fstream is;
  if (!openOrCreate(is, userFile, ios_base::in | ios_base::out))
    throw Error("File error of user's data!");
  UserData read;
  pos = MAXACC;
#ifndef USING_BTREE
  is.seekg(0);
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
#else
  fstream index;
  if (!openOrCreate(index, indexFile, ios_base::in | ios_base::out))
    throw Error("Error on openning index file!");
  index.seekg(0);
  size_t root;
  index.read((char *)&root, sizeof root);
  index.read((char *)&root, sizeof root);
  index.close();
  Tree tree(snapFile, root);
  String name;
  bzero(name, MAXLEN);
  strcpy(name, user);
  if (!tree.find(name, &pos))
    {
      pos = MAXACC;
      is.close();
      return read;
    }
  is.seekg(pos * sizeof read);
  is.read((char *)&read, sizeof read);
  is.close();
  return read;
#endif
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
#ifdef USING_BTREE
      size_t root;
      index.seekg(0);
      index.read((char *)&root, sizeof root);
      size_t position = index.tellg();
      index.read((char *)&root, sizeof root);      
#endif
#ifdef USING_BTREE
      Tree tree(snapFile, root);
      if (!tree.insert(user.user, &pos))
	throw Error("BTree insertion error on creating user!");
      root = tree.offset();
      index.seekp(position);
      index.write((char *)&root, sizeof root);
#endif
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
#ifdef USING_BTREE
      fstream index;
      if (!openOrCreate(index, indexFile, ios_base::in | ios_base::out))
	throw Error("Error on openning index file!");
      index.seekg(0);
      size_t root;
      index.read((char *)&root, sizeof root);
      size_t position = index.tellg();
      index.read((char *)&root, sizeof root);
      Tree tree(snapFile, root);
      if (!tree.remove(empty.user))
	throw Error("Error on BTree removing of removing account!");
      root = tree.offset();
      index.seekp(position);
      index.write((char *)&root, sizeof root);
      index.close();
#endif
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
  search(name, p);
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

bool UserInfo::message(const char *content)
{
  Message mess;
  mess.id = Pub::message(content);
  UserData user;
  if (pos == MAXACC || !get(pos, user))
    return false;
  mess.next = user.message;
  memcpy(mess.user, user.user, MAXLEN);
  memcpy(mess.from, user.user, MAXLEN);
  user.message = Pub::store(mess);
  put(pos, user);

  return true;
}

bool UserInfo::forward(const Message &message)
{
  Message mess;
  mess.id = message.id;
  UserData user;
  if (pos == MAXACC || !get(pos, user) ||
      strcmp(user.user, message.from) == 0)
    return false;
  mess.next = user.message;
  memcpy(mess.user, user.user, MAXLEN);
  memcpy(mess.from, message.from, MAXLEN);
  user.message = Pub::store(mess);
  put(pos, user);
  return true;
}

void UserInfo::put(size_t offset, const UserData &user)
{
  fstream os;
  if (!openOrCreate(os, userFile, ios_base::in | ios_base::out))
    throw Error("Error on opennning user's data file");
  os.seekp(offset * sizeof user);
  os.write((char *)&user, sizeof user);
  os.close();
}

vector<Package> UserInfo::list(size_t number)
{
  vector<Package> vec;
  fstream is, content;
  if (!openOrCreate(is, messageFile, ios_base::in | ios_base::out))
    throw Error("Error on openning message file!");
  if (!openOrCreate(content, contentFile, ios_base::in | ios_base::out))
    throw Error("Error on openning content file!");
  is.seekg(0, ios_base::end);
  size_t offset(is.tellg());
  UserData user;
  if (pos == MAXACC || !get(pos, user))
    return vec;
  BTree<size_t, char, 1, 1> *tree;
  if (user.following == MAXACC)
    tree = new BTree<size_t, char, 1, 1>(followingFile);
  else
    tree = new BTree<size_t, char, 1, 1>(followingFile, user.following);
  Message message;
  Package package;
  while (offset != 0)
    {
      offset -= sizeof message;
      is.seekg(offset);
      is.read((char *)&message, sizeof message);
      size_t pos;
      search(message.user, pos);
      size_t id[] = {pos};
      char tmp[1];
      if (strcmp(user.user, message.user) == 0 || tree->find(id, tmp))
	{
	  content.seekg(message.id);
	  content.read(package.content, MAXLEN);
	  memcpy((char *)&package.info, (char *)&message, sizeof message);
	  vec.push_back(package);
	}
    }
  return vec;
}

vector<Package> UserInfo::list(const char *who, size_t num)
{
  vector<Package> vec;
  size_t pos;
  UserData user = search(who, pos);
  if (pos == MAXACC)
    return vec;
  pos = user.message;
  fstream content;
  if (!openOrCreate(content, contentFile, ios_base::in | ios_base::out))
    throw Error("Error on openning content file!");
  while (pos != MAXACC && vec.size() != num)
    {
      Package package;
      Pub::load(pos, package.info);
      content.seekg(package.info.id);
      content.read(package.content, MAXLEN);
      vec.push_back(package);
      pos = package.info.next;
    }
  return vec;
}

std::vector<UserData> UserInfo::find(const char *keyword)
{
  fstream is;
  if (!openOrCreate(is, userFile, ios_base::in | ios_base::out))
    throw Error("Error on openning user's data file!");
  is.seekg(0);
  vector<UserData> vec;
  size_t number = strtol(keyword, NULL, 10);

  UserData user;
  is.read((char *)&user, sizeof user);
  while (is)
    {
      bzero((char *)&user, sizeof user);
      is.read((char *)&user, sizeof user);
      if (!is)
	break;
      size_t pattern = 0;
      if (user.y == number || user.m == number || user.d == number)
	pattern |= BIRTHDAY;
      if (string(user.user).find(keyword) != string::npos)
	pattern |= OTHERS;
      if (string(user.tele).find(keyword) != string::npos)
	pattern |= TELEPHONE;
      if (string(user.addr).find(keyword) != string::npos)
	pattern |= ADDR;
      if (((string("male").find(keyword) != string::npos ||
	    string("man").find(keyword) != string::npos ||
	    string("boy").find(keyword) != string::npos) && user.gend == male) ||
	  ((string("female").find(keyword) != string::npos ||
	    string("woman").find(keyword) != string::npos ||
	    string("girl").find(keyword) != string::npos) && user.gend == female))
	pattern |= GEND;
      if (pattern)
	{
	  user.off = pattern;
	  vec.push_back(user);
	}
    }
  return vec;
}
