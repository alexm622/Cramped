#include "file/formats/format.h"

std::string Format::fmtToStr(Format_e fmt)
{
  switch (fmt)
  {
  case UNKNOWN:
    return "none";
  case FAT12: case FAT32:
    return "vfat"; 
  case EXFAT:
    return "exfat";
  case EXT2:
    return "ext2";
  case EXT4:
    return "ext4";
  case XFS:
    return "xfs";
  default:
    return "none";
  }
}
