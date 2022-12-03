#ifndef FAT12_H
#define FAT12_H

#include "file/formats/fat.h"

class Fat12 : public Fat {
public:
  Fat12(char *fname, lli size)
      : Fat(fname, size, FAT12), fname(fname), size(size){};
  void writeBPB() override;
  void writeExBR() override;
  void writeExtras() override;

  Format_e getFatType() override;

private:
  char *fname;
  lli size;
};

#endif
