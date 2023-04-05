#include "file/formats/fat12.h"

#include "utils/converter.h"

#include <fstream>
#include <ios>
//write bios parameter block
void Fat12::writeBPB() {
  std::fstream file;
  file.open(fname);
  file.seekp(0);
  char *data = new char[3];
  data[0] = (char)0xEB;
  data[1] = (char)0x3C;
  data[2] = (char)0x90;

  file.write(data, 3);
  delete[] data;
  std::string str = "alexcore";
  file.write(str.c_str(), 8);

  int bytes_per_sector = 512;
  unsigned char *buf = new unsigned char[4];
  // 0x0b bytes per sector
  Converter::IntToLittleEndianHex(buf, bytes_per_sector);
  printf("0x0B as hex:%x%x\n", buf[0], buf[1]);
  file.write(reinterpret_cast<const char *>(buf), 2);
  // 0x0D sectors per cluster
  Converter::IntToLittleEndianHex(buf, 4);
  printf("0x0D as hex:%x\n", buf[0]);
  file.write(reinterpret_cast<const char *>(buf), 1);
  // 0x0e reserved sectors count
  Converter::IntToLittleEndianHex(buf, 1);
  printf("0x0E as hex:%x%x\n", buf[0], buf[1]);
  file.write(reinterpret_cast<const char *>(buf), 2);
  // 0x10 number of fat copies
  Converter::IntToLittleEndianHex(buf, 2);
  printf("0x10 as hex:%x\n", buf[0]);
  file.write(reinterpret_cast<const char *>(buf), 1);
  // 0x11 number of root directory entries
  Converter::IntToLittleEndianHex(buf, 224);
  printf("0x11 as hex:%x%x\n", buf[0], buf[1]);
  file.write(reinterpret_cast<const char *>(buf), 2);
  // 0x13 number of sectors in filesystem
  Converter::IntToLittleEndianHex(buf, size / bytes_per_sector);
  printf("0x13 as hex:%x%x\n", buf[0], buf[1]);
  file.write(reinterpret_cast<const char *>(buf), 2);
  // 0x15 media descriptor type
  buf[0] = 0xf8;
  printf("0x15 as hex:%x\n", buf[0]);
  file.write(reinterpret_cast<const char *>(buf), 1);
  // 0x16 number of sectors per FAT
  Converter::IntToLittleEndianHex(buf, 9);
  printf("0x16 as hex:%x%x\n", buf[0], buf[1]);
  file.write(reinterpret_cast<const char *>(buf), 2);
  // 0x18 number of sectors per track
  Converter::IntToLittleEndianHex(buf, 12);
  printf("0x18 as hex:%x%x\n", buf[0], buf[1]);
  file.write(reinterpret_cast<const char *>(buf), 2);
  // 0x1a number of heads
  Converter::IntToLittleEndianHex(buf, 2);
  printf("0x1a as hex:%x%x\n", buf[0], buf[1]);
  file.write(reinterpret_cast<const char *>(buf), 2);
  // 0x1c number of hidden sectors
  Converter::IntToLittleEndianHex(buf, 0);
  printf("0x1c as hex:%x%x%x%x\n", buf[0], buf[1], buf[2], buf[3]);
  file.write(reinterpret_cast<const char *>(buf), 4);
  // 0x20 large sector count
  Converter::IntToLittleEndianHex(buf, 0);
  printf("0x20 as hex:%x%x%x%x\n", buf[0], buf[1], buf[2], buf[3]);
  file.write(reinterpret_cast<const char *>(buf), 4);
  file.close();
  delete[] buf;
}

void Fat12::writeExBR() {
  std::fstream file;
  file.open(fname);
  file.seekp(0x024);
  unsigned char *buf = new unsigned char[4];
  // 0x024 drive number
  Converter::IntToLittleEndianHex(buf, 1);
  file.write(reinterpret_cast<const char *>(buf), 1);
  // 0x025 windows nt flags
  file.write(reinterpret_cast<const char *>(buf), 1);
  // 0x025 signature
  buf[0] = 0x28;
  file.write(reinterpret_cast<const char *>(buf), 1);
  // 0x027 serial number
  Converter::IntToLittleEndianHex(buf, 1234567890);
  file.write(reinterpret_cast<const char *>(buf), 4);
  // 0x02b volume label
  std::string str = "cramped_vol";
  file.write(str.c_str(), 11);
  // 0x036 sysyem identifier
  str = "FAT12   ";
  file.write(str.c_str(), 8);
  // 0x1fe bootable partition signature
  file.seekg(448, std::ios::cur);
  buf[0] = 0x55;
  buf[1] = 0xaa;
  file.write(reinterpret_cast<const char *>(buf), 2);

  delete[] buf;
  return;
}

void Fat12::writeExtras() { return; }

Format_e Fat12::getFatType() { return FAT12; }
