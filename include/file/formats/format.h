#ifndef FORMATS_H
#define FORMATS_H

#include <fstream>

#include "utils/types.h"

enum Format_t { FAT12 };

class Format {
public:
  Format(std::ofstream &file, lli type);
  ~Format();
  virtual void format();

private:
  lli type;
  std::ofstream &file;
};

#endif
