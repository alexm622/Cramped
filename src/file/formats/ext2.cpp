// ext2
#include "file/formats/ext2.h"

#include "utils/converter.h"

#include "inttypes.h"

#include <random>
#include <vector>

void Ext2::format()
{
  calculate();
  writeBSB();
  writeESB();
  writeBGDT();
}

void Ext2::calculate()
{
  // calculate the number of blocks and inodes
  numInodes = size / EXT2_INODE_SIZE;
  numBlocks = size / EXT2_BLOCK_SIZE;

  // calculate the number of free blocks and inodes
  freeInodes = numInodes;
  freeBlocks = numBlocks;

  // generate two random uint64_t's for the guid using c++11 random
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<uint64_t> dis(0, UINT64_MAX);
  guid_p1 = dis(gen);
  guid_p2 = dis(gen);

  // print the guid as it will be written to the disk
  auto *b1 = new unsigned char[8];
  auto *b2 = new unsigned char[8];

  Converter::u64ToLittleEndianHex(b1, guid_p1);
  Converter::u64ToLittleEndianHex(b2, guid_p2);

  printf("guid: ");

  for (int i = 0; i < 8; i++)
  {
    printf("%02x", b1[i]);
  }

  for (int i = 0; i < 8; i++)
  {
    printf("%02x", b2[i]);
  }

  printf("\n");

  // clean up
  delete[] b1;
  delete[] b2;
}

/**
 * @brief write basic superblock
 *
 */
void Ext2::writeBSB()
{
  std::fstream file;
  file.open(fname);

  // seek forwards to the superblock
  file.seekp(1024);

  // 0x00 total inodes
  auto *buf = new unsigned char[4];
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
  while (EXT2_BLOCK_SIZE >> log2 != 1)
  {
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
void Ext2::writeESB()
{
  std::fstream file;
  file.open(fname);
  // seek forwards to the superblock then skip the basic superblock
  file.seekp(1024 + 84);

  // 0x00 first non-reserved inode
  auto *buf = new unsigned char[4];
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

  // convert the guid to little endian hex and put into a buffer
  auto b1 = new unsigned char[8];
  auto b2 = new unsigned char[8];

  Converter::u64ToLittleEndianHex(b1, guid_p1);
  Converter::u64ToLittleEndianHex(b2, guid_p2);

  // write the guid
  for (int i = 0; i < 8; i++)
  {
    file.write(reinterpret_cast<const char *>(&b1[i]), 1);
  }

  for (int i = 0; i < 8; i++)
  {
    file.write(reinterpret_cast<const char *>(&b2[i]), 1);
  }

  // we dont need these anymore
  delete[] b1;
  delete[] b2;

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

// TODO: write block group descriptor table
/**
 * @brief write block group descriptor table
 *
 */
void Ext2::writeBGDT()
{
  std::fstream file;
  file.open(fname);

  // seek forwards to the block group descriptor table
  file.seekp(1024 + 1024);

  // 0x00 block bitmap block
  auto *buf = new unsigned char[4];
  Converter::IntToLittleEndianHex(buf, 2);
  file.write(reinterpret_cast<const char *>(buf), 4);

  // 0x04 inode bitmap block
  Converter::IntToLittleEndianHex(buf, 3);
  file.write(reinterpret_cast<const char *>(buf), 4);

  // 0x08 inode table block
  Converter::IntToLittleEndianHex(buf, 4);
  file.write(reinterpret_cast<const char *>(buf), 4);

  // 0x0C free blocks count
  Converter::IntToLittleEndianHex(buf, freeBlocks);
  file.write(reinterpret_cast<const char *>(buf), 2);

  // 0x0E free inodes count
  Converter::IntToLittleEndianHex(buf, freeInodes);
  file.write(reinterpret_cast<const char *>(buf), 2);

  // 0x10 used directory count
  Converter::IntToLittleEndianHex(buf, 0);
  file.write(reinterpret_cast<const char *>(buf), 2);

  // 0x12 pad
  Converter::IntToLittleEndianHex(buf, 0);
  file.write(reinterpret_cast<const char *>(buf), 2);

  // 0x14 reserved
  Converter::IntToLittleEndianHex(buf, 0);
  file.write(reinterpret_cast<const char *>(buf), 12);

  // done with block group descriptor table
  file.close();
  delete[] buf;
}