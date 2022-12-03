#include "file/mount.h"

#include <sys/mount.h>

bool Mount::mountFile(Format f, std::string destination) {
  if (f.getFormat() == UNKNOWN) {
    printf("cannot mount, there is no filesystem\n");
    return false;
  }
  int ret =
      mount(reinterpret_cast<const char *>(f.getFname()), destination.c_str(),
            reinterpret_cast<const char *>(f.getFormatStr().c_str()), 0x00,
            "defaults");
  if (ret != 0) {
    interpretError(ret, destination, f);
    return false;
  }

  return true;
}
void Mount::interpretError(int err, std::string path, Format f) {
  switch (errno) {
  case (EACCES):
    printf("EACCES error\n");
    break;
  case (EBUSY):
    printf("EBUSY error\n");
    break;
  case (EFAULT):
    printf("EFAULT error\n");
    break;
  case (EINVAL):
    printf("EINVAL\n");
    break;
  case (ELOOP):
    printf("ELOOP\n");
  case (EMFILE):
    printf("table of dummy devices is full\n");
  case (ENAMETOOLONG):
    printf("name too long\n");
  case (ENODEV):
    printf("filesystem not configured in the kernel\n");
  case (ENOENT):
    printf("bad pathname\n");
  case (ENOMEM):
    printf("kernel could not allocate memory for mounting\n");
  case (ENOTBLK):
    printf("source is not a block device\n");
  case (ENOTDIR):
    printf("target is not a directory\n");
  case (ENXIO):
    printf("the number of block device is out of range\n");
  case (EPERM):
    printf("insufficient permissions\n");
  case (EROFS):
    printf("this filesystem is read only\n");
  default:
    printf("unknown error: %d\n", errno);
  }
}
