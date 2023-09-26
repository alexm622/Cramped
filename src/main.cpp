#include "main.h"

#include "file/filemaker.h"
#include "file/formats/format.h"
#include "file/mount.h"

#include <string>
#include <iterator>

#include <boost/algorithm/string.hpp>

po::options_description Main::desc("Cramped usage");
po::variables_map Main::vm;

static bool create = false;
static bool mount = false;
static bool disconnect = false;
static bool directory_b = false;
static bool format_b = false;

int main(int argc, char *argv[])
{
  Main::add_arguments();
  Main::map_variables(argc, argv);
  // try creating a file?
  std::string fname = "test";
  std::string disconnect_str = fname;

  std::string directory = "test_m/";
  Format_e format = UNKNOWN;

  if (Main::vm.count("format"))
  {
    format_b = true;
    std::string format_str = Main::vm["format"].as<std::string>();
    format = Main::format_from_string(format_str);
  }

  if (Main::vm.count("create"))
  {
    create = true;
    fname = Main::vm["create"].as<std::string>();
  }

  if (Main::vm.count("mount"))
  {
    //make sure there is a directory
    if (Main::vm.count("directory") && format_b) {
      directory_b = true;
      mount = true;
      fname = Main::vm["mount"].as<std::string>();
      directory = Main::vm["directory"].as<std::string>();
    }else{
      printf("mount operation requires a directory destination (-D) for mounting and a format (-f) \n");
      std::cout << Main::desc;
      return 1;
    }
  }

  if (Main::vm.count("disconnect"))
  {
    disconnect = true;
    disconnect_str = Main::vm["disconnect"].as<std::string>();
  }


  // get the size
  lli size = -1;

  if (Main::vm.count("size"))
  {
    std::string size_str = Main::vm["size"].as<std::string>();

    boost::trim(size_str);

    printf("entered %s for size\n", Main::vm["size"].as<std::string>().c_str());
    size = Main::sizeStrToInt(size_str);
  }

  if (size == -1)
  {
    printf("size defaulting to 1M\n");
    size = std::pow(1024, 2);
  }

  printf("size is %lld\n", size);

  if (create)
  {
    FileMaker fm(fname.c_str(), size);
    fm.setFormatType(format);
    fm.makeFile();
    fm.formatFile(format);
  }
  else if (mount)
  {
    FileMaker fm(fname.c_str(), size);
    fm.setFormatType(format);
    Mount::mountFile(fm.readFormat(), directory);
  }
  else if (disconnect)
  {
    FileMaker fm(disconnect_str.c_str(), size);
    fm.setFormatType(format);
    Mount::disconnectFile(fm.readFormat());
  }

  return 0;
}

// add all the arguments
void Main::add_arguments()
{

  desc.add_options()("help,h", "Display this help message");
  desc.add_options()("create,c", po::value<std::string>()->required()->value_name("file"),
                     "create a new device"
                     "where <file> is the name of the file");
  desc.add_options()("size,s", po::value<std::string>()->required()->value_name("size"),
                     "the size of the mount\nappend the following characters to specify size increments (default: kilobytes)\n       B: bytes\n       K: kilobytes\n       M: megabytes\n       G: gigabytes");
  desc.add_options()("format,f", po::value<std::string>()->required()->value_name("format"), "the format of the mount (default: FAT12)\navailable formats:\n       FAT12\n       FAT32\n       exFat");
  desc.add_options()("mount,m", po::value<std::string>()->required()->value_name("file"),
                     "mount a <file>, requires a directory to be specified");
  desc.add_options()("directory,d", po::value<std::string>()->required()->value_name("directory"), "the directory to mount to");
  desc.add_options()("disconnect,D", po::value<std::string>()->required()->value_name("device"),
                     "disconnect device");
}

// map the arguments to parameters
void Main::map_variables(int argc, char **argv)
{
  try
  {
    po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);

    if (vm.count("help"))
    {
      std::cout << desc;
      exit(0);
    }
  }
  catch (boost::program_options::error &e)
  {
    std::cout << "ERROR: " << e.what() << "\n";
    std::cout << desc << "\n";
    exit(1);
  }
}

/**
 * @brief turn a string into a format enum
 *
 * @param fmt
 * @return Format_e
 */
Format_e Main::format_from_string(std::string fmt)
{
  boost::to_upper(fmt);
  if (fmt == "FAT12")
  {
    return FAT12;
  }
  else if (fmt == "FAT32")
  {
    return FAT32;
  }
  else if (fmt == "EXFAT")
  {
    return EXFAT;
  }
  else if (fmt == "EXT2")
  {
    return EXT2;
  }
  else if (fmt == "EXT4")
  {
    return EXT4;
  }
  else if (fmt == "XFS")
  {
    return XFS;
  }
  return UNKNOWN;
}

/**
 * @brief string to integer
 *
 * @param size
 * @return uint64_t
 */
lli Main::sizeStrToInt(std::string const &size)
{

  if (size.length() < 2)
  {
    // someting is wrong
    return -1;
  }

  char letter = size.c_str()[size.length() - 1];

  printf("letter is %c\n", letter);

  printf("size: %s\n", size.substr(0, size.length() - 1).c_str());

  lli size_int = std::stoll(size.substr(0, size.length() - 1));

  switch (letter)
  {
  case 'B':
    break;
  case 'K':
    size_int *= 1024;
    break;
  case 'M':
    size_int *= pow(1024, 2);
    break;
  case 'G':
    size_int *= pow(1024, 3);
    break;
  default:
    return -1;
  }

  printf("size is now %lld\n", size_int);

  return size_int;
}
