#ifndef _MESSAGE_
#define _MESSAGE_

#include "../common/userData.hpp"

namespace Pub
{
  size_t store(const Message &);
  void load(size_t, Message &);
  size_t message(const char *);
}

#endif
