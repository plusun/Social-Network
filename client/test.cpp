#include "user.hpp"
#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>
#include <cstdio>

using std::cout;
using std::endl;
using std::flush;
using std::string;
using std::map;
using std::vector;
using std::ofstream;
using std::ios_base;

ofstream log("process.log", ios_base::out);
ofstream result("time.result", ios_base::out);
const size_t MAX = 100000;
size_t N = MAX;
size_t LOT = 70; // deal of truth

#define CORRECTNESS
#define TIMETEST

#ifdef CORRECTNESS
bool correct();
#endif

#ifdef TIMETEST
void step();
#endif

bool randBool();
string randString();
void clear();

int main()
{
  srand(time(NULL));
  
#ifndef TIMETEST
  cout << "Testing for correctness..." << endl;
  clear();
  if (!correct())
    cout << "Fail. :(" << endl;
  else
    cout << "Success. :)" << endl;
#else
  for (N = 10; N < MAX; N *= 10)
    step();
  N /= 10;
  const size_t STEP = N;
  N += N;
  for (; N <= MAX; N += STEP)
    step();
#endif
  
  return 0;
}

char randChar()
{
  const size_t MOD = 128;
  char tmp;
  for (tmp = rand() % MOD;
       tmp != '\0' && !isalnum(tmp) && !ispunct(tmp); tmp = rand() % MOD)
    ;
  return tmp;
}

string randString()
{
  string str;
  char tmp('\0');
  while (tmp == '\0')
    tmp = randChar();
  while (str.length() < MAXLEN - 1 && tmp != '\0')
    {
      str += tmp;
      tmp = randChar();
    }
  return str;
}

bool randBool()
{
  const size_t TOTAL = 100;
  return rand() % TOTAL < LOT;
}

#ifdef CORRECTNESS
bool correct()
{
  map<string, string> repertoire;
  vector<string> list;
  for (size_t i = 0; i < N; ++i)
    if (randBool())
      {
	log << "Creating new account: " << endl;
	string name;
	do
	  name = randString();
	while (repertoire.find(name) != repertoire.end());
	User user;
	string pw = randString();
	log << name << endl;
	if (!user.newAccount(name, pw))
	  return false;
	repertoire[name] = pw;
	list.push_back(name);
      }
    else
      {
	log << "Checking existed account: " << flush;
	if (list.empty())
	  continue;
	size_t off = rand() % list.size();
	User user(list[off], repertoire[list[off]]);
	log << list[off] << endl;
	if (!user.valid())
	  return false;
      }  
#ifndef TIMETEST
  cout << list.size() << " accounts created." << endl;
  cout << "Testing for all accounts..." << flush;
  for (size_t i = 0; i < list.size(); ++i)
    {
      User user(list[i], repertoire[list[i]]);
      if (!user.valid())
	return false;
    }
  cout << "Finished." << endl;
#endif
  return true;
}
#endif

void clear()
{
  remove(userFile);
  remove(indexFile);
  remove(snapFile);
}

#ifdef TIMETEST
void step()
{
  clear();
  cout << "Testing: N=" << N << ", LOT=" << LOT << "..." << flush;
  clock_t t = clock();
  bool right = correct();
  t = clock() - t;
  if (!right)
    {
      cout << "Failed. :(" << endl;
      exit(0);
    }
  else
    {
      cout << "Success. :)" << endl;
      result << N << '\t' << ((double)t) / CLOCKS_PER_SEC << endl;
    }
}
#endif
