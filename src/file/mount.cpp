#include "file/mount.h"

#include <filesystem>
#include <regex>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

/**
 * @brief mount a file to a destination
 * 
 * @param f the file to mount
 * @param destination the destination to mount to
 * @return true if successful
 * @return false if unsuccessful
 */
bool Mount::mountFile(Format f, std::string destination)
{
  if (f.getFormat() == UNKNOWN)
  {
    printf("cannot mount, there is no filesystem\n");
    return false;
  }
  std::string loopname = createLoop(f);
  f.setLoopDevice(loopname);
  std::filesystem::path dest = destination;
  dest = std::filesystem::absolute(dest);

  // get gid and uid of folder
  int destfd = open(dest.c_str(), O_RDWR);
  struct stat file_stat;
  fstat(destfd, &file_stat);
  int uid = file_stat.st_uid;
  int gid = file_stat.st_gid;
  char *opts = new char[64];
  sprintf(opts, "uid=%d,gid=%d,umask=0750,fmask=0750,dmask=0750", uid, gid);

  printf("attempting to mount %s at %s\n", f.getLoopDevice().c_str(),
         dest.c_str());

  int ret =
      mount(f.getLoopDevice().c_str(), dest.c_str(),
            reinterpret_cast<const char *>(f.getFormatStr().c_str()), 0, opts);
  delete[] opts;
  if (ret != 0)
  {
    interpretError(ret, destination, f);
    return false;
  }

  return true;
}
/**
 * @brief intepret error code from mount
 * 
 * @param err error
 * @param path path to mount to
 * @param f format of file
 */
void Mount::interpretError(int err, std::string path, Format f)
{
  switch (errno)
  {
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
    break;
  case (EMFILE):
    printf("table of dummy devices is full\n");
    break;
  case (ENAMETOOLONG):
    printf("name too long\n");
    break;
  case (ENODEV):
    printf("filesystem not configured in the kernel\n");
    break;
  case (ENOENT):
    printf("bad pathname\n");
    break;
  case (ENOMEM):
    printf("kernel could not allocate memory for mounting\n");
    break;
  case (ENOTBLK):
    printf("source is not a block device\n");
    break;
  case (ENOTDIR):
    printf("target is not a directory\n");
    break;
  case (ENXIO):
    printf("the number of block device is out of range\n");
    break;
  case (EPERM):
    printf("insufficient permissions\n");
    break;
  case (EROFS):
    printf("this filesystem is read only\n");
    break;
  default:
    printf("unknown error: %d\n", errno);
  }
}

/**
 * @brief create a loop device for the file
 * 
 * @param f the file to create a loop device for
 * @return std::string the name of the loop device
 */
std::string Mount::createLoop(Format f)
{
  int loopctlfd, loopfd, backingfile;
  long devnr;
  char loopname[4096];

  removeRedundantLoop(f);
  // TODO add autoclear flag to device

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
  std::string loopname_str = loopname;
  return loopname_str;
}
/**
 * @brief remove a loop device if it is redundant
 * 
 * @param f file to check
 */
void Mount::removeRedundantLoop(Format f)
{
  std::vector<std::pair<int, std::string>> loops = getLoops();
  std::filesystem::path path = f.getFname();
  int parent = open(path.c_str(), O_RDWR);
  struct stat file_stat;
  fstat(parent, &file_stat);
  unsigned long inode = file_stat.st_ino;
  close(parent);
  for (std::pair<int, std::string> p : loops)
  {
    int loopfd = open(p.second.c_str(), O_RDWR);
    if (loopfd == -1)
    {
      printf("%s failed\n", p.second.c_str());
      continue;
    }
    loop_info li = getLoopInfo(loopfd);
    printf("lo #%d name: %s, fd: %lu \n", li.lo_number, li.lo_name,
           li.lo_inode);
    if (li.lo_inode == inode)
    {
      char *fname = new char[64];
      sprintf(fname, "/dev/loop%d", li.lo_number);
      int r = ioctl(loopfd, LOOP_CLR_FD, li.lo_number);
      if (r == -1)
      {
        printf("loop%d busy\n", li.lo_number);
      }
      delete[] fname;
      close(loopfd);
    }
  }
}
/**
 * @brief get a list of all mounted loop devices
 * 
 * @return std::vector<std::pair<int, std::string>> list of loop devices
 */
std::vector<std::pair<int, std::string>> Mount::getLoops()
{
  std::vector<std::pair<int, std::string>> loops;

  std::vector<std::string> files;
  for (const auto &entry : std::filesystem::directory_iterator("/dev/"))
  {
    // printf("got entry: %s\n", entry.path().c_str());
    files.push_back(entry.path());
  }

  std::regex loop("\\/dev\\/loop[0-9]{1,}");

  std::fstream fs;
  fs.open("/proc/partitions", std::ios::in);
  std::vector<std::string> currloops;
  while (fs)
  {
    std::string device;
    std::getline(fs, device);
    std::regex partitions("loop[0-9]{1,}");
    std::smatch m;
    std::regex_search(device, m, partitions);
    if (m.empty())
    {
      continue;
    }
    device = m[0];
    device = regex_replace(device, std::regex("loop"), "");
    currloops.push_back(device);
  }
  fs.close();
  for (std::string s : files)
  {

    if (std::regex_match(s, loop))
    {
      // printf("found loop %s\n", s.c_str());
      int number;
      std::string name = s;
      std::regex getNumber("/dev/loop");
      s = std::regex_replace(s, getNumber, "");

      number = std::stoi(s);

      if (std::count(currloops.begin(), currloops.end(), s))
      {
        std::pair<int, std::string> pair(number, name);
        loops.push_back(pair);
      }
    }
  }

  return loops;
}

/**
 * @brief get loop info from a file descriptor
 * 
 * @param fd file descriptor
 * @return loop_info loop info
 */
loop_info Mount::getLoopInfo(int fd)
{
  loop_info li;
  ioctl(fd, LOOP_GET_STATUS, &li);

  return li;
}
