#include "file/formats/fat12.h"

#include "utils/converter.h"

#include <ios>

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
  Converter::IntToLittleEndianHex(buf, bytes_per_sector);
  printf("as hex:%x%x%x%x\n", buf[0], buf[1], buf[2], buf[3]);
  file.write((const char *)buf, 4);
}

void Fat12::writeExBR() { return; }

void Fat12::writeExtras() { return; }

Format_e Fat12::getFatType() { return FAT12; }
