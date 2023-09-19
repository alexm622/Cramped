//ext2
#include "file/formats/ext2.h"

#include "utils/converter.h"

#include "inttypes.h"


#include <random>
#include <vector>

void Ext2::format(){
  calculate();
  writeBSB();
  writeESB();
  writeBGDT();
}

void Ext2::calculate(){
  //calculate the number of blocks and inodes
  numInodes = size / EXT2_INODE_SIZE;
  numBlocks = size / EXT2_BLOCK_SIZE;

  //calculate the number of free blocks and inodes
  freeInodes = numInodes;
  freeBlocks = numBlocks;

  //generate two random uint64_t's for the guid using c++11 random
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<uint64_t> dis(0, UINT64_MAX);
  guid_p1 = dis(gen);
  guid_p2 = dis(gen);

}

/**
 * @brief write basic superblock
 * 
 */
void Ext2::writeBSB() {
  std::fstream file;
  file.open(fname);
  
  //seek forwards to the superblock
  file.seekp(1024);
  


  // 0x00 total inodes
  unsigned char *buf = new unsigned char[4];
  Converter::IntToLittleEndianHex(buf, 0);
  file.write(reinterpret_cast<const char *>(buf), 4);

  // 0x04 total blocks
  Converter::IntToLittleEndianHex(buf, size / EXT2_BLOCK_SIZE);
  file.write(reinterpret_cast<const char *>(buf), 4);

  // 0x08 reserved blocks
  Converter::IntToLittleEndianHex(buf, 0);
  file.write(reinterpret_cast<const char *>(buf), 4);

  // 0x0C free blocks
  Converter::IntToLittleEndianHex(buf, numBlocks);
  file.write(reinterpret_cast<const char *>(buf), 4);

  // 0x10 free inodes
  Converter::IntToLittleEndianHex(buf, numInodes);
  file.write(reinterpret_cast<const char *>(buf), 4);

  // 0x14 first data block
  Converter::IntToLittleEndianHex(buf, 0);
  file.write(reinterpret_cast<const char *>(buf), 4);

  // 0x18 log2(block size) - 10
  lli log2 = 0;
  while (EXT2_BLOCK_SIZE >> log2 != 1) {
    log2++;
  }
  Converter::IntToLittleEndianHex(buf, log2);
  file.write(reinterpret_cast<const char *>(buf), 4);

  // 0x1C log2(fragment size) - 10
  Converter::IntToLittleEndianHex(buf, log2);
  file.write(reinterpret_cast<const char *>(buf), 4);

  // 0x20 blocks per group
  Converter::IntToLittleEndianHex(buf, numBlocks);
  file.write(reinterpret_cast<const char *>(buf), 4);

  // 0x24 fragments per group
  Converter::IntToLittleEndianHex(buf, numBlocks);
  file.write(reinterpret_cast<const char *>(buf), 4);

  // 0x28 inodes per group
  Converter::IntToLittleEndianHex(buf, numInodes);
  file.write(reinterpret_cast<const char *>(buf), 4);

  // 0x2C mount time
  Converter::IntToLittleEndianHex(buf, 0);
  file.write(reinterpret_cast<const char *>(buf), 4);

  // 0x30 write time
  Converter::IntToLittleEndianHex(buf, 0);
  file.write(reinterpret_cast<const char *>(buf), 4);

  // 0x34 mount count
  Converter::IntToLittleEndianHex(buf, 0);
  file.write(reinterpret_cast<const char *>(buf), 2);

  // 0x36 max mount count
  Converter::IntToLittleEndianHex(buf, 10);
  file.write(reinterpret_cast<const char *>(buf), 2);

  // 0x38 magic signature
  Converter::IntToLittleEndianHex(buf, EXT2_SUPER_MAGIC);
  file.write(reinterpret_cast<const char *>(buf), 2);

  // 0x3A filesystem state
  Converter::IntToLittleEndianHex(buf, 1);
  file.write(reinterpret_cast<const char *>(buf), 2);

  // 0x3C error handling
  Converter::IntToLittleEndianHex(buf, 1);
  file.write(reinterpret_cast<const char *>(buf), 2);

  // 0x3E minor revision level
  Converter::IntToLittleEndianHex(buf, 1);
  file.write(reinterpret_cast<const char *>(buf), 2);

  // 0x40 last check time
  Converter::IntToLittleEndianHex(buf, 0);
  file.write(reinterpret_cast<const char *>(buf), 4);

  // 0x44 check interval
  Converter::IntToLittleEndianHex(buf, 0);
  file.write(reinterpret_cast<const char *>(buf), 4);

  // 0x48 os id
  Converter::IntToLittleEndianHex(buf, 0);
  file.write(reinterpret_cast<const char *>(buf), 4);

  // 0x4C major revision level
  Converter::IntToLittleEndianHex(buf, 6);
  file.write(reinterpret_cast<const char *>(buf), 2);

  // 0x4E default uid for reserved blocks
  Converter::IntToLittleEndianHex(buf, 0);
  file.write(reinterpret_cast<const char *>(buf), 2);

  // 0x50 default gid for reserved blocks
  Converter::IntToLittleEndianHex(buf, 0);
  file.write(reinterpret_cast<const char *>(buf), 2);
 
  // done with basic superblock
  file.close();
  delete[] buf;

}

/**
 * @brief write extended superblock
 * 
 */
void Ext2::writeESB() {
  std::fstream file;
  file.open(fname);
  //seek forwards to the superblock then skip the basic superblock
  file.seekp(1024 + 83);

  // 0x00 first non-reserved inode
  unsigned char *buf = new unsigned char[4];
  Converter::IntToLittleEndianHex(buf, 11);
  file.write(reinterpret_cast<const char *>(buf), 4);

  // 0x04 inode size
  Converter::IntToLittleEndianHex(buf, EXT2_INODE_SIZE);
  file.write(reinterpret_cast<const char *>(buf), 2);

  // 0x06 block group number
  Converter::IntToLittleEndianHex(buf, 0);
  file.write(reinterpret_cast<const char *>(buf), 2);

  // 0x08 compatible feature set
  Converter::IntToLittleEndianHex(buf, 0);
  file.write(reinterpret_cast<const char *>(buf), 4);

  // 0x0C incompatible feature set
  Converter::IntToLittleEndianHex(buf, 0);
  file.write(reinterpret_cast<const char *>(buf), 4);

  // 0x10 read only compatible feature set
  Converter::IntToLittleEndianHex(buf, 0);
  file.write(reinterpret_cast<const char *>(buf), 4);

  // 0x14 uuid
  //print guid
  printf("GUID: %#" PRIx64 "%" PRIx64 "\n",guid_p1,guid_p2);

  //part one
  Converter::IntToLittleEndianHex(buf, guid_p1>>32);
  file.write(reinterpret_cast<const char*>(buf),4);
  Converter::IntToLittleEndianHex(buf, guid_p1);
  file.write(reinterpret_cast<const char*>(buf),4);

  //part 2
  Converter::IntToLittleEndianHex(buf, guid_p2>>32);
  file.write(reinterpret_cast<const char*>(buf),4);
  Converter::IntToLittleEndianHex(buf, guid_p2);
  file.write(reinterpret_cast<const char*>(buf),4);

  // 0x24 volume name
  std::string volName = "EXT2";
  file.write(volName.c_str(), 16);

  // 0x34 last mount path
  std::string path = "/";
  file.write(path.c_str(), 64);

  // 0x74 compression algorithms
  Converter::IntToLittleEndianHex(buf, 0);
  file.write(reinterpret_cast<const char *>(buf), 4);

  // 0x78 preallocated blocks for files
  Converter::IntToLittleEndianHex(buf, 0);
  file.write(reinterpret_cast<const char *>(buf), 1);

  // 0x79 preallocated blocks for directories
  Converter::IntToLittleEndianHex(buf, 0);
  file.write(reinterpret_cast<const char *>(buf), 1);

  // 0x7A reserved
  Converter::IntToLittleEndianHex(buf, 0);
  file.write(reinterpret_cast<const char *>(buf), 2);

  // 0x7C journal id
  Converter::IntToLittleEndianHex(buf, 0);
  file.write(reinterpret_cast<const char *>(buf), 4);

  // 0x80 journal inode
  Converter::IntToLittleEndianHex(buf, 0);
  file.write(reinterpret_cast<const char *>(buf), 4);

  // 0x84 journal device
  Converter::IntToLittleEndianHex(buf, 0);
  file.write(reinterpret_cast<const char *>(buf), 4);

  // 0x88 orphan inode list head
  Converter::IntToLittleEndianHex(buf, 0);
  file.write(reinterpret_cast<const char *>(buf), 4);

  // done with extended superblock
  file.close();
  delete[] buf;
  
}

/**
 * @brief write block group descriptor table
 * 
 */
void Ext2::writeBGDT() {}