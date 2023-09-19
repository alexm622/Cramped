#ifndef EXFAT_H
#define EXFAT_H

#include "file/formats/format.h"

#include <chrono>

#include <math.h>
#include <time.h>

class ExFat : public Format
{
public:
  ExFat(char *fname, lli size) : Format(fname, size, EXT2), fname(fname), size(size){};
  ~ExFat(){};
  void format() override;
  void writeBS();
  void writeEBS();
  void writeOEMP();
  void writeRS();
  void writeBC();
  Format_e getFormat() const { return EXFAT; };

private:
  char *fname;
  lli size;
  Format_e f;

  // variables necessary for code

  // BS
  std::string str = "cramped ";
  std::string fsName = "ExFat  ";

  const int BYTES_PER_SECTOR_SHIFT = 9;

  // these should all be dynamic at some point
  const int BYTES_PER_SECTOR = 512;
  const lli NUM_SECTORS = size / BYTES_PER_SECTOR / 4;
  const int CLUSTER_COUNT = 4;
  const int FAT_OFFSET = 24;
  const int FAT_LENGTH = ((CLUSTER_COUNT + 2) * pow(2, 2)) / (pow(2, BYTES_PER_SECTOR_SHIFT));
  const int CLUSTER_HEAP_OFFSET = FAT_OFFSET + FAT_LENGTH * CLUSTER_COUNT;
  const int FIRST_CLUSTER_OF_ROOT_DIRECTORY = 2;

  // EBS
  const int EBS_START = 0xfe + 0x02;
  const int EBS_SIZE = pow(2, BYTES_PER_SECTOR_SHIFT) - 4;

  /// OEMP
  const int OEMP_START = EBS_START + EBS_SIZE + 0x04;

  //flash parameters 

  const char * OEM_ID = "0A0C7E463399402190C8FA6D389C4BA2";
  const int OEM_ID_SIZE = 16;


  
  // guid for the volume
  uint64_t guid_p1;
  uint64_t guid_p2;
  const int OEMP_SECTION_SIZE = 48;

  const uint64_t FLASH_p1 = 0x0A0C7E4633994021;
  const uint64_t FLASH_p2 = 0x90C8FA6D389C4BA2;

  // FAT_RS
  const int FAT_RS_START = 0x1e0 + pow(2, BYTES_PER_SECTOR_SHIFT) - 480;
  const int MEDIA_DESCRIPTOR = 0xF8FFFFFF;
};

#endif
