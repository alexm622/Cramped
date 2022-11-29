#ifndef MAIN_H
#define MAIN_H

#include <iostream>

#include <boost/program_options.hpp>

namespace po = boost::program_options;

class Main {
public:
  static po::options_description desc;
  static po::variables_map vm;

  static void add_arguments();
  static void map_variables(int argc, char *argv[]);

private:
};

#endif
