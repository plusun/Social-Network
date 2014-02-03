#ifndef _OPEN_FILE_
#define _OPEN_FILE_

#include <fstream>

// open or create a input file
bool openOrCreate(std::ifstream &stream, const char *file, std::ios_base::openmode bit);
bool openOrCreate(std::ofstream &stream, const char *file, std::ios_base::openmode bit);
bool openOrCreate(std::fstream &stream, const char *file, std::ios_base::openmode bit);

#endif
