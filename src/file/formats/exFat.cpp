#include "file/formats/exFat.h"

#include "utils/converter.h"

#include <fstream>
#include <ios>
#include <math.h>
#include <time.h>

void ExFat::format(){
  writeBS();
  writeEBS();
  writeOEMP();
  writeRS();
  writeBC();
}

void ExFat::writeBS(){
  printf("writing BS");
  const int BYTES_PER_SECTOR_SHIFT = 9;
  //initialization
  std::fstream file;
  file.open(fname);
  file.seekp(0);
  char *data = new char[3];
  unsigned char *buf = new unsigned char[4];

  std::string str = "cramped ";
  std::string fsName = "ExFat  ";

  //jumpboot data
  data[0]=(char)0xEB;
  data[1]=(char)0x76;
  data[2]=(char)0x90;

  //jumpboot
  file.write(data,3);

  //filesystemname
  file.write(fsName.c_str(), 8);

  //must be zero
  buf[0] = 0x00;
  for(int i =0; i < 53; i++){
    file.write(reinterpret_cast<const char*>(buf),1);
  }
  //partition offset
  file.seekg(8,std::ios::cur);

  //volume length

  //this should be dynamic at some point
  const int bytes_per_sector = 512;
  //calculate the number
  const lli num_sectors = size / bytes_per_sector / 4;
  const int cluster_count = 4;


  
  Converter::IntToLittleEndianHex(buf, num_sectors);
  file.write(reinterpret_cast<const char *>(buf), 4);

  //fat offset field
  const int FAT_OFFSET = 24;
  Converter::IntToLittleEndianHex(buf, FAT_OFFSET);
  file.write(reinterpret_cast<const char *>(buf), 4);

  //FATLength
  //this will eventually be dynamic as well
  int fatLength = ((cluster_count + 2) * pow(2,2))/(pow(2, BYTES_PER_SECTOR_SHIFT));
  Converter::IntToLittleEndianHex(buf, fatLength);
  file.write(reinterpret_cast<const char *>(buf), 4);

  //cluster heap offset
  const int CLUSTER_HEAP_OFFSET = FAT_OFFSET+fatLength*cluster_count;
  Converter::IntToLittleEndianHex(buf, CLUSTER_HEAP_OFFSET);
  file.write(reinterpret_cast<const char *>(buf), 4);

  //cluster count
  Converter::IntToLittleEndianHex(buf, cluster_count);
  file.write(reinterpret_cast<const char *>(buf), 4);

  //first cluster of root directory
  const int FIRST_CLUSTER_OF_ROOT_DIRECTORY = 2;
  Converter::IntToLittleEndianHex(buf, FIRST_CLUSTER_OF_ROOT_DIRECTORY);
  file.write(reinterpret_cast<const char *>(buf),4);

  //serial number
  struct timespec spec;
  clock_gettime(CLOCK_REALTIME, &spec);
  long time_nano = spec.tv_nsec;
  lli sn = (lli)(time_nano * pow(243,7) - 888 * 8 +012341423423) >> 23;
  Converter::IntToLittleEndianHex(buf, sn);
  file.write(reinterpret_cast<const char *>(buf), 4);


  //revision number
  Converter::IntToLittleEndianHex(buf, 1);
  file.write(reinterpret_cast<const char *>(buf), 2);

  //volumeflags
  //should be able to fill this with zeros on formatting
  buf[0] = 0x00;
  buf[1] = 0x00;
  file.write(reinterpret_cast<const char*>(buf), 2);

  //bytes per sector shift
  Converter::IntToLittleEndianHex(buf, BYTES_PER_SECTOR_SHIFT);
  file.write(reinterpret_cast<const char*>(buf), 1);

  //nunber of fats
  Converter::IntToLittleEndianHex(buf, 1);
  file.write(reinterpret_cast<const char *>(buf), 1);

  //drive select
  buf[0]=0x23;
  file.write(reinterpret_cast<const char*>(buf),1);

  //percent in use
  buf[0]=0x00;
  file.write(reinterpret_cast<const char*>(buf),1);
  //seek to bootsig
  file.seekp(0x1fe);

  //write bootsig bytes
  buf[0] = 0x55;
  buf[1] = 0xAA;
  file.write(reinterpret_cast<const char*>(buf),2);
  printf("done writing bootsector");
}

void ExFat::writeEBS() {};
void ExFat::writeOEMP(){};
void ExFat::writeRS(){};
void ExFat::writeBC(){};
