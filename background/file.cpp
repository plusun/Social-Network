#include "file.hpp"

// open or create a input file
bool openOrCreate(std::ifstream &stream, const char *file, std::ios_base::openmode bit) 
{
  bit |= std::ios_base::binary;
  stream.open(file, bit);
  if (!stream)
    {
      std::ofstream newIs(file, std::ios_base::out);
      if (!newIs)
	return false;
      newIs.close();
      stream.open(file, bit);
    }
  if (!stream)
    return false;
  return true;
}

bool openOrCreate(std::ofstream &stream, const char *file, std::ios_base::openmode bit) 
{
  bit |= std::ios_base::binary;
  stream.open(file, bit);
  if (!stream)
    {
      std::ofstream newIs(file, std::ios_base::out);
      if (!newIs)
	return false;
      newIs.close();
      stream.open(file, bit);
    }
  if (!stream)
    return false;
  return true;
}

bool openOrCreate(std::fstream &stream, const char *file, std::ios_base::openmode bit) 
{
  bit |= std::ios_base::binary;
  stream.open(file, bit);
  if (!stream)
    {
      std::ofstream newIs(file, std::ios_base::out);
      if (!newIs)
	return false;
      newIs.close();
      stream.open(file, bit);
    }
  if (!stream)
    return false;
  return true;
}
