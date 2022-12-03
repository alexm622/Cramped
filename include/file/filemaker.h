#ifndef FILEMAKER_H
#define FILEMAKER_H

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>

#include "file/formats/format.h"

class FileMaker {
public:
  FileMaker(const char *fname, lli size) {
    this->fname = new char[strlen(fname)];
    this->fname = strcpy(this->fname, fname);

    this->size = size;
  };
  ~FileMaker() { delete[] fname; };

  void makeFile();
  void formatFile(Format_e f);

  void setFormatType(Format_e f) { this->fmt = f; };

  Format readFormat();

private:
  char *fname;
  lli size = 0;
  Format_e fmt = UNKNOWN;
  Format *f;

  void writeBlock(std::ofstream &file, std::vector<char> &vec, lli num) {
    for (int i = 0; i < num; i++) {
      if (!file.write(&vec[0], vec.size())) {
        std::cerr << "failed to write file" << std::endl;
        exit(255);
      }
    }
  };

  Format getFormat(Format_e f_e);
};
#endif
