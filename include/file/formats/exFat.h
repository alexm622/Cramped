#ifndef EXFAT_H
#define EXFAT_H

#include "file/formats/format.h"

class ExFat : public Format{
  public:
  ExFat(char *fname, lli size) 
    :Format(fname, size, EXFAT), fname(fname), size(size){};
  void format();
  void writeBS();
  void writeEBS();
  void writeOEMP();
  void writeRS();
  void writeBC();
  Format_e getFormat(){return UNKNOWN;};

private:
  char *fname;
  lli size;
  Format_e f;
  
};

#endif

