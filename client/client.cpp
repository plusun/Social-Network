#include "user.hpp"

#include <termios.h>
#include <unistd.h>
#include <cstring>
#include <cstdio>
#include <cctype>
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>

using std::cout;
using std::endl;
using std::cin;
using std::flush;
using std::string;
using std::vector;

const char * PROMPT = "# ";
const char * INDENT = "  * ";
const size_t MAXMESSAGES = 10;


string getUserName(void);
string getPassword(void);
void createUser(const string &, User &);
void doOrder(string, User &, vector<Package> &messages);
bool checkOK(const string &);

int main(void)
{
  cout << PROMPT << "Welcome to this ugly social interaction software." << endl;
  cout << PROMPT << "Input '!' at the start of user name to create a new account." << endl;
  cout << PROMPT << "The password should be made up of visible characters on your keyboard." << endl;
  cout << endl;
  string name;
  User user;
  vector<Package> messages;
  do
    {
      do
	{
	  cout << PROMPT << "User Name: " << flush;
	  name = getUserName();
	}
      while (name.empty() || !checkOK(name));
      if (*name.begin() == '!')
	{
	  name.erase(name.begin());
	  createUser(name, user);
	}
      else
	{
	  string password;
	  do
	    {
	      cout << PROMPT << "Password: " << flush;
	      password = getPassword();
	    }
	  while (password.empty());
	  
	  user.set(name, password);
	  if (!user.valid())
	    cout << INDENT << "Invalid user name or wrong password. :(" << endl;
	}
    }
  while(!user.valid());
  
  cout << endl;
  
  cout << "Welcome back, " << user.name() << " :)" << endl
       << "You can input 'help' for help." << endl << endl;
  
  string order;
  do
    {
      cout << PROMPT << flush;
      getline(cin, order);
      doOrder(order, user, messages);
    }
  while (true);

  return 0;
}

int getch (void) // get char without showing the character
{
  int ch;
  struct termios oldt, newt;
  
  tcgetattr(STDIN_FILENO, &oldt);
  memcpy(&newt, &oldt, sizeof(newt));
  newt.c_lflag &= ~( ECHO | ICANON | ECHOE | ECHOK |
		     ECHONL | ECHOPRT | ECHOKE | ICRNL);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  ch = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  
  return ch;
}

bool checkOK(const string &name)
{
  for (string::const_iterator itr = name.begin(); itr != name.end(); ++itr)
    if (!(isalnum(*itr) || ispunct(*itr)))
      return false;
  return true;
}

string getUserName(void)
{
  string name = "";
  /*
  char c;
  do
    {
      c = getch();
      if (isalnum(c) || ispunct(c))
	{
	  name += c;
	  cout << c << flush;
	}
      if (c == '\b')
	cout << '\b' << flush;
    }
  while (c != '\n');
  cout << endl;
  return name;*/
  getline(cin, name);
  return name;
}

string getPassword(void)
{
  const string PW = "";
  string  pw = "";
  char c;
  do
    {
      c = getch();
      if (isalnum(c) || ispunct(c))
	{
	  pw += c;
	  cout << PW << flush;
	}
      else if (c == '\177')
	{
	  if (!pw.empty())
	    pw.erase(pw.end() - 1);
	}
    }
  while (c != '\n');
  cout << endl;
  return pw;
}

void changeGender(User &user)
{
  string tmp;
  do
    {
      cout << INDENT << "Your gender: " << flush;
      getline(cin, tmp);
    }
  while (!user.gender(tmp));
}

void changeBirth(User &user)
{
  Date birth;
  size_t y, m, d;
  string line;
  do
    {
      cout << INDENT << "Your birthday: " << endl;;
      cout << INDENT << " Year: ";
      getline(cin, line);
      y = atoi(line.c_str());
      cout << INDENT << " Month: ";
      getline(cin, line);
      m = atoi(line.c_str());
      cout << INDENT << " Day: ";
      getline(cin, line);
      d = atoi(line.c_str());
    }
  while (!birth.set(y, m, d) || !user.birthday(birth));
  
}

void changeAddress(User &user)
{
  string tmp;
  do
    {
      cout << INDENT << "Your address: " << flush;
      getline(cin, tmp);
    }
  while (!user.address(tmp));
}

void changeTele(User &user)
{
  string tmp;
  do
    {
      cout << INDENT << "Your telephone: " << flush;
      getline(cin, tmp);
    }
  while (!user.telephone(tmp));
}

string newPassword()
{
  string pw, tmp;
  do
    {
      cout << INDENT << "Your password: " << flush;
      pw = getPassword();
      cout << INDENT << "Once again please: " << flush;
      tmp = getPassword();
      if (pw != tmp)
	cout << INDENT << "There are some differences between two inputs." << endl;
    }
  while (pw != tmp);
  return pw;
}

void createUser(const std::string &name, User &user)
{
  if (user.exist(name))
    {
      cout << "Account exists. :(" << endl;
      return;
    }
  cout << "Create a new account! :)" << endl;
  string pw = newPassword();
  if (user.newAccount(name, pw))
    {
      changeGender(user);
      changeBirth(user);
      changeAddress(user);
      changeTele(user);
    }
  else
    cout << "Creating account failed. :(" << endl;
}

string LIST[] = {"help", "info", "pw", "address", "birth", "tele", "gender", "close", "quit",
		 "follow", "unfollow", "follower", "following", "message", "list", "forward", "visit", "search", ""};
enum Order{HELP, INFO, PW, ADDRESS, BIRTH, TELE, GENDER, CLOSE, QUIT, FOLLOW, UNFOLLOW, FOLLOWER, FOLLOWING,
	   MESSAGE, LISTS, FORWARD, VISIT, SEARCH, NONE};

Order getOrder(const string &order)
{
  for (size_t i = 0; LIST[i] != ""; ++i)
    if (LIST[i] == order)
      return Order(i);
  return NONE;
}

void doOrder(string input, User &user, vector<Package> &messages)
{
  string old, pw, content;
  size_t code, number;
  vector<string> list;
  vector<UserData> result;
  switch (getOrder(input))
    {
    case HELP:
      cout << INDENT << "help:       Print these information" << endl
	   << INDENT << "info:       Print your information" << endl
	   << INDENT << "follow:     To follow someone" << endl
	   << INDENT << "unfoloow:   To unfollow someone" << endl
	   << INDENT << "following:  Following list" << endl
	   << INDENT << "follower:   Follower list" << endl
	   << INDENT << "message:    To publish a message" << endl
	   << INDENT << "list:       List messages" << endl
	   << INDENT << "visit:      Visit a user" << endl
	   << INDENT << "pw:         Change password" << endl
	   << INDENT << "address:    Change your address" << endl
	   << INDENT << "birth:      Change your birthday" << endl
	   << INDENT << "tele:       Change your telephone" << endl
	   << INDENT << "gender:     Change your gender" << endl
	   << INDENT << "close:      Close this account" << endl
	   << INDENT << "quit:       Quit" << endl;
      break;
    case INFO:
      cout << INDENT << "User: " << user.name() << endl;
      cout << INDENT << "Gender: " << user.gender() << endl;
      cout << INDENT << "Birthday: " << user.birthday() << endl;
      cout << INDENT << "Telephone: " << user.telephone() << endl;
      cout << INDENT <<  "Address: " << user.address() << endl;
      break;
    case PW:
      cout << INDENT << "Old password: " << flush;
      old = getPassword();
      pw = newPassword();
      if (!user.password(old, pw))
	cout << INDENT << "Wrong old password! :(" << endl;
      else
	cout << INDENT << "Changed successfully! :)" << endl;
      break;
    case FOLLOW:
      cout << INDENT << "Who: " << flush;
      old = getUserName();
      if (!user.follow(old))
	cout << INDENT << "Failed. :(" << endl;
      else
	cout << INDENT << "Success! :)" << endl;
      break;
    case UNFOLLOW:
      cout << INDENT << "Who: " << flush;
      old = getUserName();
      if (!user.unfollow(old))
	cout << INDENT << "Failed. :(" << endl;
      else
	cout << INDENT << "Success! :)" << endl;
      break;
    case FOLLOWER:
      list = user.follower();
      if (list.size() > 0)
	{
	  cout << INDENT << flush;
	  cout << list[0] << flush;
	  for (size_t i = 1; i < list.size(); ++i)
	    cout << ", " << list[i] << flush;
	  cout << endl;
	}
      break;
    case FOLLOWING:
      list = user.following();
      if (list.size() > 0)
	{
	  cout << INDENT << flush;
	  cout << list[0] << flush;
	  for (size_t i = 1; i < list.size(); ++i)
	    cout << ", " << list[i] << flush;
	  cout << endl;
	}
      break;
    case MESSAGE:
      cout << INDENT << "What to say: " << flush;
      getline(cin, content);
      while (content.length() >= MAXLEN)
	content.erase(content.length() - 1);
      if (user.message(content))
	cout << INDENT << "Success! :)" << endl;
      else
	cout << INDENT << "Fail! :(" << endl;
      break;
    case LISTS:
      cout << INDENT << "Number: " << flush;
      getline(cin, content);
      number = strtol(content.c_str(), NULL, 10);
      if (number == 0)
	number = MAXMESSAGES;
      messages = user.list(number);
      cout << endl;
      for (size_t i = 0; i < messages.size(); ++i)
	{
	  cout << INDENT << messages[i].info.user << ": " << flush
	       << messages[i].content << flush;
	  if (strcmp(messages[i].info.user, messages[i].info.from))
	    cout << " <forwarded from " << messages[i].info.from << ">" << flush;
	  cout << " (" << i << ")" << endl;
	}
      break;
    case ADDRESS:
      changeAddress(user);
      break;
    case BIRTH:
      changeBirth(user);
      break;
    case TELE:
      changeTele(user);
      break;
    case GENDER:
      changeGender(user);
      break;
    case FORWARD:
      cout << INDENT << "Which(the code following the messages): " << flush;
      getline(cin, content);
      code = strtol(content.c_str(), NULL, 10);
      if (code >= messages.size() || !user.forward(messages[code].info))
	cout << INDENT << "Fail. :(" << endl;
      else
	cout << INDENT << "Success! :)" << endl;
      break;
    case CLOSE:
      cout << INDENT << "Your password: " << flush;
      pw = getPassword();
      if (!user.close(pw))
	cout << INDENT << "Wrong password! :(" << endl;
      else
	{
	  cout << "This account is closed." << endl;
	  cout << "Goodbye. :)" << endl;
	  getch();
	  exit(0);
	}
      break;
    case VISIT:
      cout << INDENT << "Who: " << flush;
      getline(cin, content);
      messages.clear();
      messages = user.list(content, MAXMESSAGES);
      for (size_t i = 0; i < messages.size(); ++i)
	{
	  cout << endl;
	  cout << INDENT << messages[i].info.user << ": " << flush
	       << messages[i].content << flush;
	  if (strcmp(messages[i].info.user, messages[i].info.from))
	    cout << " <forwarded from " << messages[i].info.from << ">" << flush;
	  cout << " (" << i << ")" << endl;
	}      
      break;
    case SEARCH:
      cout << INDENT << "Keyword: " << flush;
      getline(cin, content);
      result = user.find(content);
      if (result.size() > 0)
	cout << endl;
      for (vector<UserData>::iterator itr = result.begin();
	   itr != result.end(); ++itr)
	{
	  cout << INDENT << itr->user << flush;
	  if (itr->off != OTHERS)
	    cout << " >" << flush;
	  if (itr->off & GEND)
	    cout << " Gender" << flush;
	  if (itr->off & BIRTHDAY)
	    cout << " Birthday:" << Date(itr->y, itr->m, itr->d) << flush;
	  if (itr->off & TELEPHONE)
	    cout << " Telephone:" << itr->tele << flush;
	  if (itr->off & ADDR)
	    cout << " Address:" << itr->addr << flush;
	  cout << endl;
	}
      break;
    case QUIT:
      cout << "Goodbye. :)" << endl;
      getch();
      exit(0);
      break;
    default:
      cout << INDENT << "No such order! :(" << endl;
      break;
    }
}
