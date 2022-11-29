#include "file/formats/fat12.h"

#include "utils/formats.h"

#include <ios>

void Fat12::writeBPB() {
  std::ofstream file(fname, std::ios::binary | std::ios::out);
  file.seekp(0);
  char *data = new char[3];
  data[0] = (char)0xEB;
  data[1] = (char)0xFE;
  data[2] = (char)0x90;

  file.write(data, 1);
  delete[] data;
}

void Fat12::Fat::writeExBR() { return; }

void Fat12::writeExtras() { return; }
