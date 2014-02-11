#ifndef _USER_DATA_
#define _USER_DATA_

#include <fstream>

const size_t MAXLEN = 140;
const size_t MAXACC = (size_t)-1;

typedef char String[MAXLEN];
typedef enum {male, female} Gender;

typedef
struct user
{
  size_t off, message, follower, following;
  String user, pw;
  Gender gend;
  size_t y, m, d;
  String tele, addr;
} UserData;

#endif

