#ifndef _FILE_
#define _FILE_

#include <fstream>

static const char *userFile = "user.dat";
static const char *indexFile = "index.dat";
static const char *followerFile = "follower.dat";
static const char *followingFile = "following.dat";
static const char *messageFile = "message.dat";
static const char *contentFile = "content.dat";

// open or create a input file
bool openOrCreate(std::ifstream &stream, const char *file, std::ios_base::openmode bit);
bool openOrCreate(std::ofstream &stream, const char *file, std::ios_base::openmode bit);
bool openOrCreate(std::fstream &stream, const char *file, std::ios_base::openmode bit);

#endif
