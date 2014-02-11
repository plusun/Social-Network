#ifndef _B_TREE_
#define _B_TREE_

#include <string>
#include <iostream>
#include <algorithm>
#include <vector>

#include "file.hpp"
#include "../common/error.hpp"

/*
  BTree of array of keys and array of values
*/
template <typename SKey, typename SValue, size_t KSIZE, size_t VSIZE>
class BTree
{
private:
  typedef SKey Key[KSIZE];
  typedef SValue Value[VSIZE];

public:
  typedef std::vector<SKey> KeyVec;
  typedef std::vector<SValue> ValVec;

  typedef
  struct element
  {
    KeyVec key;
    ValVec value;
  } Element;

  typedef std::vector<Element> List;

  BTree(std::string); // Initialize a new B tree
  BTree(std::string, size_t); // Initialize a existed B tree

  void traversal(List &);
  size_t offset() const; // get the offset of root
  bool insert(Key, Value);
  bool find(Key, Value);
  bool remove(Key);

private:
  static const size_t NULLNODE = 0;
  static const size_t D = 1;
  static const size_t MAXDATA = 2 * D;
  static const size_t MAXKIDS = MAXDATA + 1;

  struct Node
  {
    size_t num; // number of child
    Key keys[MAXDATA + 1];
    Value values[MAXDATA + 1];
    size_t children[MAXKIDS + 1];
    size_t parent;
  };
  
  std::string name; // file name
  std::fstream file;
  size_t off; // offset of root in file

  static bool less(const Key a, const Key b);
  static bool equal(const Key a, const Key b);

  void open();
  static void bzero(void *, size_t);
  static void memcpy(void *, void *, size_t);
  
  void nullNode(); // generate a null node into file
  void getNode(size_t, Node &);
  void putNode(size_t, const Node &);
  size_t fileEnd(); // return the end offset for a file

  void put(Key, Value, size_t, size_t, size_t);
  void split(size_t, Key, Value, size_t&, size_t&);
  size_t position(Node &, Key, bool &);
  void rebalance(size_t);
  void erase(Node &, size_t);
  bool remove(size_t, Key);
  void traversal(size_t, List &);
};

#define TEMPLATE template <typename SKey, typename SValue, size_t KSIZE, size_t VSIZE>
#define BTREE BTree<SKey, SValue, KSIZE, VSIZE>

TEMPLATE
size_t BTREE::offset() const
{
  return off;
}

TEMPLATE
void BTREE::open()
{
  if (!openOrCreate(file, name.c_str(), std::ios_base::in | std::ios_base::out))
    throw Error("B tree: File openning error!");
}

TEMPLATE
void BTREE::bzero(void *ptr, size_t bytes)
{
  std::fill_n((char *)ptr, bytes, 0);  
}

TEMPLATE
bool BTREE::less(const Key a, const Key b)
{
  for (size_t i = 0; i < KSIZE; ++i)
    if (a[i] < b[i])
      return true;
    else if (a[i] > b[i])
      return false;
  return false;
}

TEMPLATE
bool BTREE::equal(const Key a, const Key b)
{
  for (size_t i = 0; i < KSIZE; ++i)
    if (a[i] != b[i])
      return false;
  return true;
}

TEMPLATE
void BTREE::nullNode()
{
  open();
  Node null;
  bzero(&null, sizeof(null));
  file.seekp(NULLNODE);
  file.write((char *)&null, sizeof(null));
  file.close();
}

TEMPLATE
BTREE::BTree(std::string filename): name(filename)
{
  nullNode();

  Node root;
  bzero(&root, sizeof(root));

  off = fileEnd();
  putNode(off, root);
}

TEMPLATE
size_t BTREE::fileEnd()
{
  size_t off;
  open();
  file.seekp(0, file.end);
  off = file.tellp();
  file.close();
  if (off == (size_t)-1)
    throw Error("B tree: File seeking error!");
  return off;
}

TEMPLATE
void BTREE::getNode(size_t offset, Node &node)
{
  open();
  file.seekg(offset);
  file.read((char *)&node, sizeof(node));
  file.close();
}

TEMPLATE
void BTREE::putNode(size_t offset, const Node &node)
{
  open();
  file.seekp(offset);
  file.write((char *)&node, sizeof(node));
  file.close();
}

TEMPLATE
BTREE::BTree(std::string filename, size_t offset): name(filename), off(offset)
{
  nullNode();
}

TEMPLATE
void BTREE::memcpy(void *destination, void *source, size_t bytes)
{
  char *dest = (char *)destination;
  char *src = (char *)source;
  for (size_t i = 0; i < bytes; ++i)
    dest[i] = src[i];
}

TEMPLATE
bool BTREE::find(Key key, Value value)
{
  size_t offset(off);
  Node cur;
  getNode(off, cur);

  while (offset != NULLNODE)
    {
      bool found;
      
      size_t pos = position(cur, key, found);

      if (found)
	{
	  memcpy(value, cur.values[pos], VSIZE * sizeof(SValue));
	  return true;
	}

      offset = cur.children[pos];
      getNode(offset, cur);
    }
  return false;
}

TEMPLATE
void BTREE::put(Key key, Value value, size_t off, size_t l, size_t r)
{
  Node node;
  getNode(off, node);

  size_t i;
  if (node.num > MAXDATA + 1)
    throw Error("B Tree: error number of node datas.");
  for (i = 0; i < node.num; ++i)
    if (less(key, node.keys[i]))
      break;
  for (size_t j = node.num++; j > i; --j)
    {
      memcpy(node.keys[j], node.keys[j - 1], KSIZE * sizeof(SKey));
      memcpy(node.values[j], node.values[j - 1], VSIZE * sizeof(SValue));
      node.children[j + 1] = node.children[j];
    }
  memcpy(node.keys[i], key, KSIZE * sizeof(SKey));
  memcpy(node.values[i], value, VSIZE * sizeof(SValue));
  node.children[i] = l;
  node.children[i + 1] = r;
  putNode(off, node);

  if (l != NULLNODE)
    {
      getNode(l, node);
      node.parent = off;
      putNode(l, node);
    }

  if (r != NULLNODE)
    {
      getNode(r, node);
      node.parent = off;
      putNode(r, node);
    }
}

TEMPLATE
void BTREE::split(size_t off, Key key, Value value, size_t &left, size_t &right)
{
  /*
    0 ~ D-1: left
    D: middle
    D+1 ~ MAXDATA: right
  */

  Node origin;
  getNode(off, origin);
  if (origin.num != MAXDATA + 1)
    throw Error("B Tree: Wrong number of splitted node's datas.");

  // right part
  Node node;
  bzero(&node, sizeof(node));
  right = fileEnd();
  for (size_t i = D + 1; i < MAXDATA + 1; ++i)
    {
      memcpy(node.keys[node.num], origin.keys[i], KSIZE * sizeof(SKey));
      memcpy(node.values[node.num], origin.values[i], VSIZE * sizeof(SValue));
      node.children[node.num] = origin.children[i];

      Node tmp;
      getNode(node.children[node.num], tmp);
      tmp.parent = right;
      putNode(node.children[node.num], tmp);

      node.num++;
    }
  node.children[node.num] = origin.children[MAXDATA + 1];
  Node tmp;
  getNode(node.children[node.num], tmp);
  tmp.parent = right;
  putNode(node.children[node.num], tmp);
  putNode(right, node);

  // middle part
  memcpy(key, origin.keys[D], KSIZE * sizeof(SKey));
  memcpy(value, origin.values[D], VSIZE * sizeof(SValue));

  // left part
  for (size_t i = D; i < MAXDATA + 1; ++i)
    {
      bzero(origin.keys[i], KSIZE * sizeof(SKey));
      bzero(origin.values[i], VSIZE * sizeof(SValue));
      origin.children[i + 1] = 0;
    }
  origin.num = D;
  origin.parent = 0;
  left = off;
  putNode(left, origin);
  
}

TEMPLATE
bool BTREE::insert(Key key, Value value)
{
  size_t offset(off), last;
  Node cur;
  getNode(off, cur);

  while (offset != NULLNODE)
    {
      last = offset;

      bool existed;

      size_t pos = position(cur, key, existed);
      if (existed)
	return false;

      offset = cur.children[pos];
      getNode(offset, cur);
    }
  offset = last;
  put(key, value, offset, NULLNODE, NULLNODE);
  getNode(offset, cur);
  while (cur.num > MAXDATA)
    {
      Key midkey;
      Value midvalue;
      size_t left, right, parent(cur.parent);
      split(offset, midkey, midvalue, left, right);
      
      if (parent == NULLNODE)
	{
	  // add a new node into file
	  offset = fileEnd();
	  bzero(&cur, sizeof(cur));
	  putNode(offset, cur);
	  // set it as root
	  off = offset;
	}
      else
	{
	  offset = parent;
	}
      put(midkey, midvalue, offset, left, right);
      getNode(offset, cur);
    }
  
  return true;
}

TEMPLATE
size_t BTREE::position(Node &node, Key key, bool &eq)
{
  eq = false;
  for (size_t i = 0; i < node.num; ++i)
    if (equal(key, node.keys[i]))
      {
	eq = true;
	return i;
      }
    else if (less(key, node.keys[i]))
      return i;
  
  return node.num;
}

TEMPLATE
bool BTREE::remove(size_t root, Key key)
{
  size_t offset(root), pos;
  bool found;
  Node node;
  while (offset != NULLNODE)
    {
      getNode(offset, node);

      pos = position(node, key, found);

      if (found)
	break;
      offset = node.children[pos];
    }
  if (!found)
    return false;

  if (node.children[pos] != NULLNODE &&
      node.children[pos + 1] != NULLNODE)
    {
      Node cur;
      size_t off(node.children[pos + 1]), last(offset);
      while (off != NULLNODE)
	{
	  last = off;
	  getNode(off, cur);
	  off = cur.children[0];
	}
      off = last;
      memcpy(node.keys[pos], cur.keys[0], KSIZE * sizeof(SKey));
      memcpy(node.values[pos], cur.values[0], VSIZE * sizeof(SValue));
      putNode(offset, node);
      remove(off, node.keys[pos]);
    }
  else
    {
      size_t child;
      if (node.children[pos] != NULLNODE)
	child = node.children[pos];
      else
	child = node.children[pos + 1];
      for (size_t i = pos; i < node.num - 1; ++i)
	{
	  memcpy(node.keys[i], node.keys[i + 1], KSIZE * sizeof(SKey));
	  memcpy(node.values[i], node.values[i + 1], VSIZE * sizeof(SValue));
	  node.children[i + 1] = node.children[i + 2];
	}
      node.children[node.num - 1] = node.children[node.num];
      bzero(node.keys[node.num - 1], KSIZE * sizeof(SKey));
      bzero(node.values[node.num - 1], VSIZE * sizeof(SValue));
      node.children[pos] = child;
      --node.num;

      putNode(offset, node);
      rebalance(offset);
    }

  return true;
}

TEMPLATE
void BTREE::rebalance(size_t offset)
{
  Node node;
  getNode(offset, node);
  if (node.num >= D)
    return;
  if (node.num == 0 && (node.parent == NULLNODE || off == offset))
    {
      if (node.children[0] != NULLNODE)
	{
	  off = node.children[0];
	  Node root;
	  getNode(off, root);
	  root.parent = 0;
	  putNode(off, root);
	}
      bzero(&node, sizeof(node));
      putNode(offset, node);
      
      return;
    }
  Node parent;
  getNode(node.parent, parent);

  size_t pos;
  for (pos = 0; pos < parent.num + 1; ++pos)
    if (parent.children[pos] == offset)
      break;

  if (pos < parent.num)
    {
      Node right;
      getNode(parent.children[pos + 1], right);
      if (right.num > D)
	{
	  // rotate left
	  memcpy(node.keys[node.num], parent.keys[pos], KSIZE * sizeof(SKey));
	  memcpy(node.values[node.num], parent.values[pos], VSIZE * sizeof(SValue));

	  memcpy(parent.keys[pos], right.keys[0], KSIZE * sizeof(SKey));
	  memcpy(parent.values[pos], right.values[0], VSIZE * sizeof(SValue));

	  node.children[++node.num] = right.children[0];
	  right.children[0] = right.children[1];

	  erase(right, 0);

	  putNode(node.parent, parent);
	  putNode(offset, node);
	  putNode(parent.children[pos + 1], right);

	  return;
	}
    }
  if (pos > 0)
    {
      Node left;
      getNode(parent.children[pos - 1], left);
      if (left.num > D)
	{
	  // rotate right
	  node.children[node.num + 1] = node.children[node.num];
	  for (size_t i = node.num - 1; i >= 0; --i)
	    {
	      memcpy(node.keys[i + 1], node.keys[i], KSIZE * sizeof(SKey));
	      memcpy(node.values[i + 1], node.values[i], VSIZE * sizeof(SValue));
	      node.children[i + 1] = node.children[i];
	    }
	  ++node.num;

	  memcpy(node.keys[0], parent.keys[pos - 1], KSIZE * sizeof(SKey));
	  memcpy(node.values[0], parent.values[pos - 1], VSIZE * sizeof(SValue));

	  memcpy(parent.keys[pos - 1], left.keys[node.num - 1], KSIZE * sizeof(SKey));
	  memcpy(parent.values[pos - 1], left.values[node.num - 1], VSIZE * sizeof(SValue));

	  node.children[0] = left.children[node.num];

	  erase(left, left.num - 1);

	  putNode(parent.children[pos - 1], left);
	  putNode(offset, node);
	  putNode(node.parent, parent);

	  return;
	}
    }

  Node left, right;
  size_t mid, sibling;

  if (pos < parent.num)
    {
      mid = pos;
      memcpy(&left, &node, sizeof(left));
      sibling = parent.children[pos + 1];
      getNode(sibling, right);
    }
  else
    {
      mid = pos - 1;
      memcpy(&right, &node, sizeof(right));
      sibling = parent.children[pos - 1];
      getNode(sibling, left);
    }

  memcpy(left.keys[left.num], parent.keys[mid], KSIZE * sizeof(SKey));
  memcpy(left.values[left.num], parent.values[mid], VSIZE * sizeof(SValue));
  left.children[++left.num] = right.children[0];

  for (size_t i = 0; i < right.num; ++i)
    {
      memcpy(left.keys[left.num], right.keys[i], KSIZE * sizeof(SKey));
      memcpy(left.values[left.num], right.values[i], KSIZE * sizeof(SValue));
      left.children[++left.num] = right.children[i + 1];
    }
  bzero(&right, sizeof(right));
  putNode(sibling, right);

  putNode(offset, left);

  parent.children[mid] = offset;
  erase(parent, mid);
  putNode(left.parent, parent);

}

TEMPLATE
bool BTREE::remove(Key key)
{
  return remove(offset(), key);
}

TEMPLATE
void BTREE::erase(Node &node, size_t pos)
{
  if (pos >= node.num)
    return;
  for (size_t i = pos; i < node.num - 1; ++i)
    {
      memcpy(node.keys[i], node.keys[i + 1], KSIZE * sizeof(SKey));
      memcpy(node.values[i], node.values[i + 1], VSIZE * sizeof(SValue));
      node.children[i + 1] = node.children[i + 2];
    }
  bzero(node.keys[node.num - 1], KSIZE * sizeof(SKey));
  bzero(node.values[node.num - 1], VSIZE * sizeof(SValue));
  node.children[node.num] = 0;
  --node.num;
}

template<typename T>
void transfer(std::vector<T> &vec, T *arr, size_t len)
{
  vec.clear();
  for (size_t i = 0; i < len; ++i)
    vec.push_back(arr[i]);
}

TEMPLATE
void BTREE::traversal(List &list)
{
  list.clear();
  traversal(off, list);
}

TEMPLATE
void BTREE::traversal(size_t offset, List &list)
{
  if (offset == NULLNODE)
    return;
  Node node;
  getNode(offset, node);
  traversal(node.children[0], list);
  for (size_t i = 0; i < node.num; ++i)
    {
      Element ele;
      transfer(ele.key, node.keys[i], KSIZE);
      transfer(ele.value, node.values[i], VSIZE);
      list.push_back(ele);
      traversal(node.children[i + 1], list);
    }
}

#undef TEMPLATE
#undef BTREE

#endif
