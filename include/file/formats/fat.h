#ifndef FAT_H
#define FAT_H

#include "file/formats/format.h"

class Fat : public Format {
public:
  Fat(char *fname, lli size) : Format(fname, size), fname(fname), size(size){};
  virtual void format() override;

  virtual void writeBPB(){};
  virtual void writeExBR(){};
  virtual void writeExtras(){};
  virtual Format_e getFatType() { return UNKNOWN; };

private:
  char *fname;
  lli size;
};

#endif
