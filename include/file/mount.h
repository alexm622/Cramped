#ifndef MOUNT_H
#define MOUNT_H

#include "file/formats/format.h"

#include <linux/loop.h>
#include <vector>

#define errExit(msg)                                                           \
  do {                                                                         \
    perror(msg);                                                               \
    exit(EXIT_FAILURE);                                                        \
  } while (0)
class Mount {
public:
  static bool mountFile(Format f, std::string destination);
  static std::string fdToFPath(unsigned long fd);

private:
  static void interpretError(int err, std::string path, Format f);
  static std::string createLoop(Format f);
  static void removeRedundantLoop(Format f);
  static std::vector<std::pair<int, std::string>> getLoops();
  static loop_info getLoopInfo(int fd);
};

#endif
