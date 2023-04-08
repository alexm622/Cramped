#ifndef EXT2_H
#define EXT2_H

#include "file/formats/format.h"

class Ext2 : public Format{
  public:
    Ext2(char *fname, lli size)
        : Format(fname, size, EXT2), fname(fname), size(size) {};
    void format();
    void writeBSB();
    void writeESB();
    void writeBGDT();
  private:
    char *fname;
    lli size;
};

#endif
