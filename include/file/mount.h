#ifndef MOUNT_H
#define MOUNT_H

#include "file/formats/format.h"
class Mount {
public:
  static bool mountFile(Format f, std::string destination);

private:
  static void interpretError(int err, std::string path, Format f);
};

#endif
