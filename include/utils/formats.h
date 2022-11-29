#ifndef FORMATS_H
#define FORMATS_H

#include <iomanip>
#include <ios>
#include <iostream>
#include <string>

void IntToLittleEndianHex(unsigned char *buff, long long num) {
  num = __builtin_bswap32(num);
  buff[0] = (num >> 24) & 0xFF;
  buff[1] = (num >> 16) & 0xFF;
  buff[2] = (num >> 8) & 0xFF;
  buff[3] = num & 0xFF;
}

std::string getHex(unsigned char *arr, long long length, long long cuts = 512) {
  std::stringstream ss;
  ss << std::hex << std::setfill('0');
  for (long long i = 0; i < length; i++) {
    ss << std::setw(2) << (long long)arr[i] << " ";
    if ((i + 1) % cuts == 0 && i > 10) {
      ss << std::endl;
    }
  }
  ss << std::endl;
  return ss.str();
}

#endif
