#include "file/formats/exFat.h"

#include "utils/converter.h"

#include <fstream>
#include <ios>

#include <inttypes.h>

#include <random>

void ExFat::format()
{

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

  //clean up
  delete[] b1;
  delete[] b2;

  writeBS();
  writeEBS();
  writeOEMP();
  writeRS();
  writeBC();
}

void ExFat::writeBS()
{
  printf("writing BS\n"); // initialization
  std::fstream file;

  //print the name of the file
  printf("fname: %s\n", fname);

  file.open(fname);
  file.seekp(0);
  auto *data = new char[3];
  auto *buf = new unsigned char[4];

  // jumpboot data
  data[0] = (char)0xEB;
  data[1] = (char)0x76;
  data[2] = (char)0x90;

  // jumpboot
  file.write(data, 3);

  delete[] data;

  // filesystemname
  file.write(this->fsName.c_str(), 8);

  // must be zero
  buf[0] = 0x00;
  for (int i = 0; i < 53; i++)
  {
    file.write(reinterpret_cast<const char *>(buf), 1);
  }
  // partition offset
  file.seekg(8, std::ios::cur);

  // volume length

  Converter::IntToLittleEndianHex(buf, this->NUM_SECTORS);
  file.write(reinterpret_cast<const char *>(buf), 4);

  // fat offset field
  Converter::IntToLittleEndianHex(buf, this->FAT_OFFSET);
  file.write(reinterpret_cast<const char *>(buf), 4);

  // FATLength
  // this will eventually be dynamic as well
  Converter::IntToLittleEndianHex(buf, this->FAT_LENGTH);
  file.write(reinterpret_cast<const char *>(buf), 4);

  // cluster heap offset
  Converter::IntToLittleEndianHex(buf, this->CLUSTER_HEAP_OFFSET);
  file.write(reinterpret_cast<const char *>(buf), 4);

  // cluster count
  Converter::IntToLittleEndianHex(buf, this->CLUSTER_COUNT);
  file.write(reinterpret_cast<const char *>(buf), 4);

  // first cluster of root directory
  Converter::IntToLittleEndianHex(buf, this->FIRST_CLUSTER_OF_ROOT_DIRECTORY);
  file.write(reinterpret_cast<const char *>(buf), 4);

  // serial number
  struct timespec spec;
  clock_gettime(CLOCK_REALTIME, &spec);
  long time_nano = spec.tv_nsec;
  lli sn = time_nano % 0xFFFFFFFF;
  Converter::IntToLittleEndianHex(buf, sn);
  file.write(reinterpret_cast<const char *>(buf), 4);

  // revision number
  Converter::IntToLittleEndianHex(buf, 1);
  file.write(reinterpret_cast<const char *>(buf), 2);

  // volumeflags
  // should be able to fill this with zeros on formatting
  buf[0] = 0x00;
  buf[1] = 0x00;
  file.write(reinterpret_cast<const char *>(buf), 2);

  // bytes per sector shift
  Converter::IntToLittleEndianHex(buf, BYTES_PER_SECTOR_SHIFT);
  file.write(reinterpret_cast<const char *>(buf), 1);

  // nunber of fats
  Converter::IntToLittleEndianHex(buf, 1);
  file.write(reinterpret_cast<const char *>(buf), 1);

  // drive select
  buf[0] = 0x23;
  file.write(reinterpret_cast<const char *>(buf), 1);

  // percent in use
  buf[0] = 0x00;
  file.write(reinterpret_cast<const char *>(buf), 1);
  // seek to bootsig
  file.seekp(0x1fe);

  // write bootsig bytes
  buf[0] = 0x55;
  buf[1] = 0xAA;
  file.write(reinterpret_cast<const char *>(buf), 2);
  printf("done writing bootsector\n");
  delete[] buf;
}

void ExFat::writeEBS()
{
  printf("writings extended boot sector\n");
  std::fstream file;
  file.open(fname);
  file.seekp(EBS_START);

  file.seekg(EBS_SIZE, std::ios::cur);

  auto buf = new unsigned char[4];
  buf[0] = 0x00;
  buf[1] = 0x00;
  buf[2] = 0x55;
  buf[3] = 0xAA;
  file.write(reinterpret_cast<const char *>(buf), 4);

  printf("done writing extended boot sector\n");
  delete[] buf;
};
void ExFat::writeOEMP()
{
  std::fstream file;
  file.open(fname);
  file.seekp(OEMP_START);
  // guid

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

  file.seekg(32);

};

// region structure
// TODO write region structure
void ExFat::writeRS(){
  // not implemented
  std::fstream file;
  file.open(fname);

  file.seekp(FAT_RS_START);

  //buffer
  auto buf = new unsigned char[4];

  //media descriptor
  Converter::IntToLittleEndianHex(buf, MEDIA_DESCRIPTOR);
  file.write(reinterpret_cast<const char *>(buf), 4);

  //write 4 bytes oxFF
  buf[0] = buf[1] = buf[2] = buf[3] = 0xFF;
  file.write(reinterpret_cast<const char *>(buf), 4);

  //write 4 bytes of 0x00
  buf[0] = buf[1] = buf[2] = buf[3] = 0x00;
  file.write(reinterpret_cast<const char *>(buf), 4);

  file.close();  

  delete[] buf;
};

// TODO boot checksum
void ExFat::writeBC()
{
  // boot checksum
  uint32_t NumberOfBytes = (uint32_t)BYTES_PER_SECTOR * 11;
  uint32_t Checksum = 0;
  uint32_t Index;
};
