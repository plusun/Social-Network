#include "file.hpp"
#include "relation.hpp"
#include "../common/userData.hpp"
#include "../common/error.hpp"
#include "btree.hpp"
#include <strings.h>

using std::ios_base;
using std::ifstream;
using std::ofstream;
using std::fstream;

UserData getUser(size_t pos)
{
  pos *= sizeof(UserData);
  UserData user;
  bzero(&user, sizeof user);
  fstream data;
  if (!openOrCreate(data, userFile, ios_base::in | ios_base::out))
    throw Error("Error on openning user's data file.");
  data.seekg(pos);
  data.read((char *)&user, sizeof user);
  data.close();
  return user;
}

void putUser(size_t pos, UserData &user)
{
  pos *= sizeof(UserData);
  fstream data;
  if (!openOrCreate(data, userFile, ios_base::out | ios_base::in))
    throw Error("Error on openning user's data file.");
  data.seekp(pos);
  data.write((char *)&user, sizeof user);
  data.close();
}

bool Relation::follow(size_t a, size_t b)
{
  if (a == b)
    return false;
  UserData follower = getUser(a);
  UserData followee = getUser(b);
  BTree<size_t, char, 1, 1> *atree, *btree; // a's following tree and b's follower tree
  if (follower.following != MAXACC)
    atree = new BTree<size_t, char, 1, 1>(followingFile, follower.following);
  else
    atree = new BTree<size_t, char, 1, 1>(followingFile);
  if (followee.follower != MAXACC)
    btree = new BTree<size_t, char, 1, 1>(followerFile, followee.follower);
  else
    btree = new BTree<size_t, char, 1, 1>(followerFile);
  size_t who[] = {b};
  char value[] = {'\0'};
  if (!atree->insert(who, value))
    {
      delete atree;
      delete btree;
      return false;
    }
  follower.following = atree->offset();
  putUser(a, follower);
  who[0] = a;
  if (!btree->insert(who, value))
    {
      
      delete atree;
      delete btree;
      return false;
    }
  followee.follower = btree->offset();
  putUser(b, followee);

  delete atree;
  delete btree;
  return true;
}

bool Relation::unfollow(size_t a, size_t b)
{
  if (a == b)
    return false;
  UserData follower = getUser(a);
  UserData followee = getUser(b);
  BTree<size_t, char, 1, 1> *atree, *btree; // a's following tree and b's follower tree
  if (follower.following != MAXACC)
    atree = new BTree<size_t, char, 1, 1>(followingFile, follower.following);
  else
    atree = new BTree<size_t, char, 1, 1>(followingFile);
  if (followee.follower != MAXACC)
    btree = new BTree<size_t, char, 1, 1>(followerFile, followee.follower);
  else
    btree = new BTree<size_t, char, 1, 1>(followerFile);
  size_t who[] = {b};

  if (!atree->remove(who))
    {
      delete atree;
      delete btree;
      return false;
    }
  follower.following = atree->offset();
  putUser(a, follower);
  who[0] = a;
  if (!btree->remove(who))
    {
      delete atree;
      delete btree;
      return false;
    }
  followee.follower = btree->offset();
  putUser(b, followee);
  delete atree;
  delete btree;

  return true;
}
