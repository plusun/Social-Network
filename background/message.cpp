#include "message.hpp"
#include "file.hpp"
#include "../common/error.hpp"

using std::fstream;
using std::ios_base;

size_t Pub::store(const Message &message)
{
  fstream os;
  if (!openOrCreate(os, messageFile, ios_base::in | ios_base::out))
    throw Error("Error on openning message file!");
  os.seekp(0, ios_base::end);
  size_t offset = os.tellp();
  os.write((char *)&message, sizeof message);
  os.close();
  return offset;
}

void Pub::load(size_t offset, Message &message)
{
  fstream is;
  if (!openOrCreate(is, messageFile, ios_base::in | ios_base::out))
    throw Error("Error on openning message file!");
  is.seekg(offset);
  is.read((char *)&message, sizeof message);
  is.close();
}

size_t Pub::message(const char *content)
{
  fstream os;
  if (!openOrCreate(os, contentFile, ios_base::in | ios_base::out))
    throw Error("Error on openning message's content file!");
  os.seekp(0, ios_base::end);
  size_t offset = os.tellp();
  os.write(content, MAXLEN);
  return offset;
}
