#include "file/mount.h"

#include <filesystem>
#include <regex>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

bool Mount::mountFile(Format f, std::string destination) {
  if (f.getFormat() == UNKNOWN) {
    printf("cannot mount, there is no filesystem\n");
    return false;
  }
  createLoop(f);
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

void Mount::createLoop(Format f) {
  int loopctlfd, loopfd, backingfile;
  long devnr;
  char loopname[4096];

  removeRedundantLoop(f);

  loopctlfd = open("/dev/loop-control", O_RDWR);
  if (loopctlfd == -1)
    errExit("open: /dev/loop-control");

  devnr = ioctl(loopctlfd, LOOP_CTL_GET_FREE);
  if (devnr == -1)
    errExit("ioctl-LOOP_CTL_GET_FREE");

  sprintf(loopname, "/dev/loop%ld", devnr);
  printf("loopname = %s\n", loopname);

  loopfd = open(loopname, O_RDWR);
  if (loopfd == -1)
    errExit("open: loopname");

  backingfile = open(f.getFname(), O_RDWR);
  if (backingfile == -1)
    errExit("open: backing-file");

  if (ioctl(loopfd, LOOP_SET_FD, backingfile) == -1)
    errExit("ioctl-LOOP_SET_FD");
  f.setLoopDevice(loopname);
}

void Mount::removeRedundantLoop(Format f) {
  std::vector<std::pair<int, std::string>> loops = getLoops();
  int loopfd;

  for (std::pair<int, std::string> p : loops) {
    loopfd = open(p.second.c_str(), O_RDWR);
    if (loopfd == -1) {
      printf("%s failed\n", p.second.c_str());
      continue;
    }
    loop_info li = getLoopInfo(loopfd);
    printf("lo #%d name: %s\n, fd: %lu \n", li.lo_number, li.lo_name,
           li.lo_inode);
    printf("lo #%d file_path:%s\n", li.lo_number,
           fdToFPath(li.lo_inode).c_str());
  }
}

std::vector<std::pair<int, std::string>> Mount::getLoops() {
  std::vector<std::pair<int, std::string>> loops;

  std::vector<std::string> files;
  for (const auto &entry : std::filesystem::directory_iterator("/dev/")) {
    // printf("got entry: %s\n", entry.path().c_str());
    files.push_back(entry.path());
  }

  // remove non-loop items
  std::regex loop("/dev/loop[0-9]*");
  for (std::string s : files) {
    if (std::regex_match(s, loop)) {
      printf("found loop %s\n", s.c_str());
      int number;
      std::string name = s;
      std::regex getNumber("/dev/loop");
      s = std::regex_replace(s, getNumber, "");
      number = std::stoi(s);
      std::pair<int, std::string> pair(number, name);
      loops.push_back(pair);
    }
  }

  return loops;
}

loop_info Mount::getLoopInfo(int fd) {
  loop_info li;
  ioctl(fd, LOOP_GET_STATUS, &li);

  return li;
}

std::string Mount::fdToFPath(unsigned long fd) {
  struct stat sb;
  std::string filePath = "";
  int ret = fstat(fd, &sb);
  return filePath;
}
