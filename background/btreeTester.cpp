#include "btree.hpp"
#include <iostream>

using namespace std;

/*void printNode(BTree<size_t, size_t, 1, 1>::Node &node)
  {
  cout << "num: " << node.num << endl;
  cout << "datas: " << endl;
  for (size_t i = 0; i < node.num; ++i)
  cout << "\t" << node.keys[i][0] << " : " << node.values[i][0] << endl;
  cout << "children:" << flush;
  for (size_t i = 0; i < node.num + 1; ++i)
  cout << " " << node.children[i] << flush;
  cout << endl;
  cout << "parent: " << node.parent << endl;
  }
*/
int main(void)
{
  /*
    BTree<char, size_t, 17, 3> t("test.dat");
    string a, b;
    cout << "Input two strings: " << endl;
    cin >> a >> b;
    cout << endl << a << endl << b << endl;
    cout << "less? " << t.less(a.c_str(), b.c_str()) << endl;
    cout << "equal? " << t.equal(a.c_str(), b.c_str()) << endl; 

    BTree<char, size_t, 17, 3>::Node node, null;
    node.num = 7;
    node.keys[3][9] = 'c';
    node.values[2][2] = 3;
    node.children[3] = 1;
    node.parent = 44;
    t.bzero(&null, sizeof(null));
    t.memcpy(&null, &node, sizeof(node));

    t.putNode(17, null);
    t.bzero(&null, sizeof(null));
    t.getNode(17, null);
    cout << null.num << null.keys[3][9] << null.values[2][2]
    << null.children[3] << null.parent << endl; 
    t.nullNode();
    t.getNode(0, null);
    cout << null.num << null.keys[3][9] << null.values[2][2]
    << null.children[3] << null.parent << endl; 
    cout << t.fileEnd() << endl;

    t.bzero(&node, sizeof(node));
    node.num = 2;
    char str[17] = {'c', 'd', 'a', 'c', '\0'};
    size_t arr[3] = {1, 4, 3};
    node.keys[0][0] = 'a';
    node.keys[1][0] = 'd';
    node.children[0] = 1;
    node.children[1] = 2;
    node.children[2] = 3;
    t.putNode(1, node);

    size_t left, right;
    t.put(str, arr, 1, 9, 34);
    t.bzero(&node, sizeof(node));
    t.getNode(1, node);

    cout << node.num << endl
    << node.keys[0] << endl << node.keys[1] << endl << node.keys[2] <<endl
    << node.values[1][0] << node.values[1][1] << node.values[1][2] << endl
    << node.children[0] << " " << node.children[1] << " " << node.children[2] << " " << node.children[3] << endl;

    t.split(1, str, arr, left, right);
    BTree<char, size_t, 17, 3>::Node l, r;
    t.getNode(left, l);
    t.getNode(right, r);
    cout << l.num << endl;
    cout << l.keys[0] << " " << l.values[0][0] << l.values[0][1] << l.values[0][2] << endl;
    cout << l.children[0] << " " << l.children[1] << endl;
    cout << r.num << endl;
    cout << r.keys[0] << " " << r.values[0][0] << r.values[0][1] << r.values[0][2] << endl;
    cout << r.children[0] << " " << r.children[1] << endl;
    cout << str << endl;
    cout << arr[0] << arr[1] << arr[2] << endl;
  */
  BTree<size_t, size_t, 1, 1> t("test.dat");
  size_t a[1], b[1];

  size_t N(100000);

  for (size_t i = 1; i < N; ++i)
    {
      a[0] = i;
      b[0] = a[0] + 7;
      t.insert(a, b);
    }

  for (size_t i = 1; i < N; ++i)
    {
      a[0] = i;
      t.remove(a);
    }

  a[0] = 3;
  b[0] = 999;
  t.insert(a, b);
  a[0] = 7;
  b[0] = 99;
  t.insert(a, b);
  a[0] = 9989;
  b[0] = 9;
  t.insert(a, b);
  a[0] = 10086;
  b[0] = 10086;
  t.insert(a, b);
  a[0] = 99999;
  b[0] = 3;
  t.insert(a, b);
  a[0] = 9234;
  b[0] = 34;
  t.insert(a, b);
  a[0] = 34;
  b[0] = 2151;
  t.insert(a, b);

  a[0] = 7;
  t.remove(a);
  a[0] = 7;
  t.remove(a);
  a[0] = 99999;
  t.remove(a);
  

  /*  a[0] = 405;
      t.remove(t.offset(), a);
      a[0] = 404;
      t.remove(t.offset(), a);
      a[0] = 401;
      t.remove(t.offset(), a);

      a[0] = 5;
      t.remove(t.offset(), a);
      a[0] = 4;
      t.remove(t.offset(), a);
      a[0] = 999;
      t.remove(t.offset(), a);*/

  size_t off = t.offset();
  BTree<size_t, size_t, 1, 1> tt("test.dat", off);

  
  for (size_t i = 1; i < N; ++i)
    {
      a[0] = i;
      /*      if (!tt.find(a, b))
	      {
	      cout << "Not found " << i << endl;
	      }
	      if (b[0] != i + 7)
	      cout << "Value error!" << endl;*/
      if (tt.find(a, b))
	{
	  cout << "Error: " << a[0] << ", " << b[0] << endl;
	}
    }
  /*
    size_t a[1], b[1];
    a[0] = 6; b[0] = 13;
    BTree<size_t, size_t, 1, 1> tt("test.dat", 288);
    tt.insert(a, b);
    
    off = 0;
    while (off < tt.fileEnd())
    {
    BTree<size_t, size_t, 1, 1>::Node node;
    tt.getNode(off, node);
    cout << "Node(" << off << "):" << endl;
    printNode(node);
    cout << endl;
    off += sizeof(node);
    } */

  BTree<char, size_t, 3, 2> tree("test.dat");
  char key[] = {'h', 'e', '\0'};
  size_t value[] = {3, 4};

  tree.insert(key, value);

  key[0] = 'a';
  key[1] = 'c';
  value[0] = 434;
  value[1] = 1234;
  tree.insert(key, value);

  key[0] = 'b';
  key[1] = 'd';
  value[0] = 34;
  value[1] = 543;
  tree.insert(key, value);

  key[0] = 'h';
  key[1] = 'k';  
  value[0] = 4545;
  value[1] = 4514;
  tree.insert(key, value);

  key[0] = 't';
  key[1] = 'e';  
  value[0] = 45646245;
  value[1] = 134254;
  tree.insert(key, value);

  key[0] = 'l';
  key[1] = 'k';
  value[0] = 3514;
  value[1] = 31513;
  tree.insert(key, value);

  size_t values[2];
  key[0] = 'a';
  key[1] = 'c';  
  
  value[0] = 434;
  value[1] = 1234;
  if (tree.find(key, values))
    {
      if (value[0] != values[0] ||
	  value[1] != values[1])
	cout << "Not found!" << endl;
    }
  else
    cout << "Error!" << endl;


  key[0] = 'b';
  key[1] = 'd';
  value[0] = 34;
  value[1] = 543;
  if (tree.find(key, values))
    {
      if (value[0] != values[0] ||
	  value[1] != values[1])
	cout << "Not found!" << endl;
    }
  else
    cout << "Error!" << endl;

  key[0] = 'h';
  key[1] = 'k';  
  value[0] = 4545;
  value[1] = 4514;
  if (tree.find(key, values))
    {
      if (value[0] != values[0] ||
	  value[1] != values[1])
	cout << "Not found!" << endl;
    }
  else
    cout << "Error!" << endl;

  key[0] = 't';
  key[1] = 'e';  
  value[0] = 45646245;
  value[1] = 134254;
  if (tree.find(key, values))
    {
      if (value[0] != values[0] ||
	  value[1] != values[1])
	cout << "Not found!" << endl;
    }
  else
    cout << "Error!" << endl;

  key[0] = 'l';
  key[1] = 'k';  
  value[0] = 3514;
  value[1] = 31513;
  if (tree.find(key, values))
    {
      if (value[0] != values[0] ||
	  value[1] != values[1])
	cout << "Not found!" << endl;
    }
  else
    cout << "Error!" << endl;

  

  return 0;
}
 

