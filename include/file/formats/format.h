#ifndef FORMATS_H
#define FORMATS_H

#include <cstring>
#include <fstream>

#include "utils/types.h"

enum Format_e { UNKNOWN, FAT12 };

class Format {
public:
  Format(char *fname, lli size, Format_e f) : size(size), f(f) {
    this->fname =
        reinterpret_cast<char *>(malloc(sizeof(char) * strlen(fname)));
    strcpy(this->fname, fname);
  }
  virtual ~Format(){};
  virtual void format(){};

  void setSize(lli size) { this->size = size; };

  Format_e getFormat() { return this->f; };
  std::string getFormatStr() { return fmtToStr(f); };

  static std::string fmtToStr(Format_e fmt);

  char *getFname() { return fname; };

private:
  lli size;
  char *fname;
  Format_e f;
};

#endif
