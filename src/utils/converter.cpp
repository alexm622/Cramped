#include "utils/converter.h"

void Converter::IntToLittleEndianHex(unsigned char *buff, long long num) {
  num = __builtin_bswap32(num);
  buff[0] = (num >> 24) & 0xFF;
  buff[1] = (num >> 16) & 0xFF;
  buff[2] = (num >> 8) & 0xFF;
  buff[3] = num & 0xFF;
}
//hexidecimal to integer
long long hexToInt(unsigned char *sector, long long offset,
                   long long size_in_bytes) {
  long long result;
  std::stringstream ss;
  ss << std::hex << std::setfill('0');
  for (long long i = 0; i < size_in_bytes; i++) {
    ss << std::setw(2) << (long long)sector[offset + i];
  }
  ss >> result;
  return result;
}
//get the hex value of a set of values
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

