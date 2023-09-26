#ifndef MOUNT_H
#define MOUNT_H

#include "file/formats/format.h"

#include <linux/loop.h>
#include <vector>

#define errExit(msg)    \
  do                    \
  {                     \
    perror(msg);        \
    exit(EXIT_FAILURE); \
  } while (0)

class Mount
{
public:
  //mouting specific functions
  static bool mountFile(Format f, std::string destination);
  static std::string fdToFPath(unsigned long fd);

  //disconnecting specific functions
  static bool disconnectFile(Format f);
  static bool disconnectFile(int fd, std::string path);
  


private:
  static void interpretError(int err);
  static std::string createLoop(int fd);
  static void removeLoop(std::string loopname);
  static void removeRedundantLoop(Format f);
  static std::vector<std::pair<int, std::string>> getLoops();
  static loop_info getLoopInfo(int fd);
  static void setOpts(char* opts, Format_e f, int gid, int uid);

};

#endif
