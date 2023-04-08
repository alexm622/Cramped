#include "file/formats/exFat.h"

#include "utils/converter.h"

#include <fstream>
#include <ios>

#include <inttypes.h>


void ExFat::format(){
  writeBS();
  writeEBS();
  writeOEMP();
  writeRS();
  writeBC();
}

void ExFat::writeBS(){
  printf("writing BS\n");  //initialization
  std::fstream file;
  file.open(fname);
  file.seekp(0);
  char *data = new char[3];
  unsigned char *buf = new unsigned char[4];

  
  //jumpboot data
  data[0]=(char)0xEB;
  data[1]=(char)0x76;
  data[2]=(char)0x90;

  //jumpboot
  file.write(data,3);

  //filesystemname
  file.write(this->fsName.c_str(), 8);

  //must be zero
  buf[0] = 0x00;
  for(int i =0; i < 53; i++){
    file.write(reinterpret_cast<const char*>(buf),1);
  }
  //partition offset
  file.seekg(8,std::ios::cur);

  //volume length


  
  Converter::IntToLittleEndianHex(buf, this->NUM_SECTORS);
  file.write(reinterpret_cast<const char *>(buf), 4);

  //fat offset field
    Converter::IntToLittleEndianHex(buf, this->FAT_OFFSET);
  file.write(reinterpret_cast<const char *>(buf), 4);

  //FATLength
  //this will eventually be dynamic as well
    Converter::IntToLittleEndianHex(buf, this->FAT_LENGTH);
  file.write(reinterpret_cast<const char *>(buf), 4);

  //cluster heap offset
    Converter::IntToLittleEndianHex(buf, this->CLUSTER_HEAP_OFFSET);
  file.write(reinterpret_cast<const char *>(buf), 4);

  //cluster count
  Converter::IntToLittleEndianHex(buf, this->CLUSTER_COUNT);
  file.write(reinterpret_cast<const char *>(buf), 4);

  //first cluster of root directory
    Converter::IntToLittleEndianHex(buf, this->FIRST_CLUSTER_OF_ROOT_DIRECTORY);
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
  printf("done writing bootsector\n");
  delete[] buf;
}

void ExFat::writeEBS() {
  printf("writings extended boot sector\n");
  std::fstream file;
  file.open(fname);
  file.seekp(EBS_START);

  file.seekg(EBS_SIZE, std::ios::cur);
  
  unsigned char *buf = new unsigned char[4];
  buf[0] = 0x00;
  buf[1] = 0x00;
  buf[2] = 0x55;
  buf[3] = 0xAA;
  file.write(reinterpret_cast<const char *>(buf), 4);

  printf("done writing extended boot sector\n");
  delete[] buf;
 

};
void ExFat::writeOEMP(){
  std::fstream file;
  file.open(fname);
  file.seekp(OEMP_START);
  //guid 1
  unsigned char *buf = new unsigned char[4];
  
  //print guid
  printf("GUID: %#" PRIx64 "%" PRIx64 "\n",GUID_p1,GUID_p2);

  //part one
  Converter::IntToLittleEndianHex(buf, GUID_p1>>32);
  file.write(reinterpret_cast<const char*>(buf),4);
  Converter::IntToLittleEndianHex(buf, GUID_p1);
  file.write(reinterpret_cast<const char*>(buf),4);

  //part 2
  Converter::IntToLittleEndianHex(buf, GUID_p2>>32);
  file.write(reinterpret_cast<const char*>(buf),4);
  Converter::IntToLittleEndianHex(buf, GUID_p2);
  file.write(reinterpret_cast<const char*>(buf),4);

  file.seekg(32);

};
void ExFat::writeRS(){};
void ExFat::writeBC(){};
