//ext2
#include "file/formats/ext2.h"

void Ext2::format(){
  writeBSB();
  writeESB();
  writeBGDT();
}

void Ext2::writeBSB() {}
void Ext2::writeESB() {}
void Ext2::writeBGDT() {}
