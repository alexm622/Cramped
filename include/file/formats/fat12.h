#ifndef FAT12_H
#define FAT12_H

#include "file/formats/fat.h"

class Fat12 : public Fat {
public:
  Fat12(char *fname, lli size) : Fat(fname, size) {
    this->fname = fname;
    this->size = size;
  };
  ~Fat12() { delete fname; }
  void writeBPB() override;
  void writeExBR() override;
  void writeExtras() override;

private:
  char *fname;
  lli size;
};

#endif
