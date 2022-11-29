#ifndef FORMATS_H
#define FORMATS_H

#include <fstream>

#include "utils/types.h"

enum Format_e { UNKNOWN, FAT12 };

class Format {
public:
  Format(char *fname, lli size) {
    this->fname = fname;
    this->size = size;
  };
  ~Format() { delete fname; };
  virtual void format();

private:
  lli size;
  char *fname;
};

#endif
