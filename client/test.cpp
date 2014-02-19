#include "user.hpp"
#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>

using std::cout;
using std::endl;
using std::flush;
using std::string;
using std::map;
using std::vector;
using std::ofstream;
using std::ios_base;

ofstream log("process.log", ios_base::out);
const size_t N = 100000;

#define CORRECTNESS
//#define TIMETEST

#ifdef CORRECTNESS
bool correct();
#endif


bool randBool();
string randString();

int main()
{
  srand(time(NULL));
  
#ifdef CORRECTNESS
  cout << "Testing for correctness..." << endl;
  if (!correct())
    cout << "Fail. :(" << endl;
  else
    cout << "Success. :)" << endl;
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
  const size_t LOT = 70; // deal of truth
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
  cout << list.size() << " accounts created." << endl;
#ifndef TIMETEST
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
