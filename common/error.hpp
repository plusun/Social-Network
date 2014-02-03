#ifndef _ERROR_
#define _ERROR_

#include <string>
#include <exception>


class Error: public std::exception
{
private:
  std::string content;
public:
  Error(const std::string &) throw();
  virtual const char* what() const throw();
  virtual ~Error() throw();
};

#endif
