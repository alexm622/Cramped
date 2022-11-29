#include "main.h"

#include "file/filemaker.h"

#include <string>

po::options_description Main::desc("Cramped usage");
po::variables_map Main::vm;

int main(int argc, char *argv[]) {
  Main::add_arguments();
  Main::map_variables(argc, argv);

  // try creating a file?
  std::string fname = "test";
  FileMaker fm(fname.c_str(), (lli)std::pow(1024, 2));
  fm.makeFile();
  return 0;
}

void Main::add_arguments() {
  desc.add_options()("help,h", "Display this help message");
}

void Main::map_variables(int argc, char **argv) {
  po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);

  if (vm.count("help")) {
    std::cout << desc;
  }
}
