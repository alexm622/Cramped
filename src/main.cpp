#include "main.h"

#include "file/filemaker.h"
#include "file/formats/format.h"
#include "file/mount.h"

#include <string>
#include <iterator>

po::options_description Main::desc("Cramped usage");
po::variables_map Main::vm;

static bool create = false;
static bool mount = false;
static bool disconnect = false;

int main(int argc, char *argv[]) {
  Main::add_arguments();
  Main::map_variables(argc, argv);
  // try creating a file?
  std::string fname = "test";

  std::string directory = "test_m/";
  if (Main::vm.count("create")){
    create = true;
    fname = Main::vm["create"].as<std::string>();
  }

  if (Main::vm.count("mount")){
    mount = true;
  }

  if (Main::vm.count("disconnect")){
    disconnect = true;
  }

  if(create + mount + disconnect > 1){
    printf("too many args\n");
    exit(1);
  }
  

  
  FileMaker fm(fname.c_str(), (lli)std::pow(1024, 2));
  fm.setFormatType(FAT12);
  Mount::mountFile(fm.readFormat(), directory);
  fm.makeFile();
  fm.formatFile(FAT12);

  return 0;
}

void Main::add_arguments() {
  
  desc.add_options()("help,h", "Display this help message");
  desc.add_options()("create,c",po::value<std::string>()->required()->value_name("file"),
      "create a new device" "where <file> is the name of the file");
  desc.add_options()("mount,m", po::value<std::string>()->required()->value_name("file"),
      "mount a file");
  desc.add_options()("disconnect,D",po::value<std::string>()->required()->value_name("device"),
      "disconnect device");
}

void Main::map_variables(int argc, char **argv) {
  try{
  po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);

  if (vm.count("help")) {
    std::cout << desc;
    exit(0);
  }
  }catch(boost::program_options::error &e){
        std::cout << "ERROR: " << e.what() << "\n";
        std::cout << desc << "\n";
        exit(1);
  }
}

