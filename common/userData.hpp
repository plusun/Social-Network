#ifndef _USER_DATA_
#define _USER_DATA_

#include <cstddef>
#include <fstream>

const size_t MAXLEN = 140;

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

