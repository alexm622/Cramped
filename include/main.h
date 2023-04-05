#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <file/formats/format.h>

#include <boost/program_options.hpp>

namespace po = boost::program_options;

class Main {
public:
  static po::options_description desc;
  static po::variables_map vm;

  static void add_arguments();
  static void map_variables(int argc, char *argv[]);
  static Format_e format_from_string(std::string fmt);

private:
};

#endif
