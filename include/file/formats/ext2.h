#ifndef EXT2_H
#define EXT2_H

#include "file/formats/format.h"

#include <chrono>

#include <math.h>
#include <time.h>

class Ext2 : public Format
{
public:
  Ext2(char *fname, lli size)
      : Format(fname, size, EXT2), fname(fname), size(size){};
  void format();
  // calculate the number of blocks and inodes
  void calculate();
  // basic superblock
  void writeBSB();
  // extended superblock
  void writeESB();
  // block group descriptor table
  void writeBGDT();

private:
  char *fname;
  lli size;

  // ext2 consts
  const lli EXT2_SUPER_MAGIC = 0xEF53;

  const lli EXT2_INODE_SIZE = 128;
  const lli EXT2_BLOCK_SIZE = 1024;

  // guid for the volume
  uint64_t guid_p1;
  uint64_t guid_p2;

  lli numInodes;
  lli numBlocks;

  lli freeInodes;
  lli freeBlocks;
};

#endif
