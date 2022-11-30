#ifndef FORMATS_H
#define FORMATS_H

#include <cstring>
#include <fstream>

#include "utils/types.h"

enum Format_e { UNKNOWN, FAT12 };

class Format {
public:
  Format(char *fname, lli size) : size(size) {
    this->fname = (char *)malloc(sizeof(char) * strlen(fname));
    strcpy(this->fname, fname);
  }
  virtual ~Format() { free(fname); };
  virtual void format(){};

  void setSize(lli size) { this->size = size; };

private:
  lli size;
  char *fname;
};

#endif
