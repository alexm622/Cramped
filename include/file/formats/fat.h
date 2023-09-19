#ifndef FAT_H
#define FAT_H

#include "file/formats/format.h"

class Fat : public Format
{
public:
  Fat(char *fname, lli size, Format_e f)
      : Format(fname, size, f), fname(fname), size(size), f(f){};
  void format() override;

  virtual void writeBPB(){
      /*write bpb*/
  };
  virtual void writeExBR(){
      /*write exbr*/
  };
  virtual void writeExtras(){
      /*write extras*/
  };
  virtual Format_e getFatType() { return UNKNOWN; };

private:
  char *fname;
  lli size;
  Format_e f;
};

#endif
