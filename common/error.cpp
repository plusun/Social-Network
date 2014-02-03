#include "error.hpp"

using std::string;
using std::exception;

Error::Error(const string &c) throw():
  content(c) {}

const char* Error::what() const throw()
{
  return content.c_str();
}

Error::~Error() throw() {}
