#include "file/filemaker.h"

#include <cmath>

#include "file/formats/fat12.h"
#include "file/formats/exFat.h"
#include "file/formats/ext2.h"
#include "file/formats/format.h"

void FileMaker::makeFile()
{
  std::ofstream file(fname, std::ios::binary | std::ios::out);
  // write file in size bytes

  // break into GB
  std::vector<char> empty((lli)std::pow(1024, 3), 0);
  lli gb_num = size;
  gb_num -= size % (lli)std::pow(1024, 3);
  gb_num /= (lli)std::pow(1024, 3);
  printf("writing %lld GB's\n", gb_num);
  writeBlock(file, empty, gb_num);

  // break into MB
  empty = std::vector<char>((lli)std::pow(1024, 2), 0);
  lli mb_num = size - (gb_num * (lli)std::pow(1024, 3));
  mb_num -= mb_num % (lli)std::pow(1024, 2);
  mb_num /= (lli)std::pow(1024, 2);
  printf("writing %lld MB's\n", mb_num);
  writeBlock(file, empty, mb_num);

  // break into KB
  lli kb_num = size - (gb_num * (lli)std::pow(1024, 3)) -
               (mb_num * (lli)std::pow(1024, 2));
  kb_num -= kb_num % 1024;
  kb_num /= 1024;
  empty = std::vector<char>(1024, 0);
  printf("writing %lld KB's\n", kb_num);
  writeBlock(file, empty, kb_num);

  // break into bytes
  lli byte_num = size - (gb_num * (lli)std::pow(1024, 3)) -
                 (mb_num * (lli)std::pow(1024, 2)) - (kb_num * 1024);
  empty = std::vector<char>(1, 0);
  printf("writing %lld bytes\n", byte_num);
  writeBlock(file, empty, byte_num);
}

// format the file
void FileMaker::formatFile(Format_e f_e)
{
  switch (f_e)
  {
  case FAT12:
    this->f = new Fat12(fname, size);
    this->f->format();
    delete this->f;
    break;
  case FAT32:
    printf("fat32 is not yet implemented\n");
    break;
  case EXFAT:
    this->f = new ExFat(fname, size);
    this->f->format();
    delete this->f;
    break;
  case EXT2:
    printf("ext2 is not yet implemented fully\n");
    this->f = new Ext2(fname, size);
    this->f->format();
    delete this->f;
    break;
  case EXT4:
    printf("ext4 is not yet implemented\n");
    break;
  case XFS:
    printf("xfs is not yet implemented\n");
    break;
  case UNKNOWN:
    printf("invalid format\n");

    return;
  default:
    printf("invalid format\n");
    return;
  }
}

Format FileMaker::readFormat()
{
  this->f = new Format(fname, size, fmt);
  return *this->f;
}
