#ifndef FORMATS_H
#define FORMATS_H

#include <cstring>
#include <fstream>

#include "utils/types.h"

enum Format_e { UNKNOWN, FAT12, FAT32, EXFAT, EXT2, EXT4, XFS };

class Format {
public:
  Format(char *fname, lli size, Format_e f) : size(size), f(f) {
    this->fname = new char[strlen(fname) + 1];
    strcpy(this->fname, fname);
    
  }
  virtual ~Format(){
    delete[] fname;
  };
  virtual void format(){};

  void setSize(lli size) { this->size = size; };

  Format_e getFormat() { return this->f; };
  std::string getFormatStr() { return fmtToStr(f); };

  static std::string fmtToStr(Format_e fmt);

  char *getFname() { return fname; };

  void setLoopDevice(const std::string &loopDevice) {
    this->loopDevice = loopDevice;
    printf("loopDevice is now %s\n", this->loopDevice.c_str());
  };

  std::string getLoopDevice() {
    printf("loopDevice is now %s\n", this->loopDevice.c_str());
    return this->loopDevice;
  };

private:
  lli size;
  char *fname;
  Format_e f;
  std::string loopDevice;
};

#endif
