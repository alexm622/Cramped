#ifndef CONVERTER_H
#define CONVERTER_H

#include <iomanip>
#include <ios>
#include <iostream>
#include <string>
class Converter
{
public:
  static void IntToLittleEndianHex(unsigned char *buff, long long num);
  static long long hexToInt(unsigned char *sector, long long offset,
                            long long size_in_bytes);
  static std::string getHex(unsigned char *arr, long long length,
                            long long cuts = 512);
  static uint64_t u64ToLittleEndian(uint64_t num);
  static void u64ToLittleEndianHex(unsigned char *buf, uint64_t num);

private:
};

#endif
