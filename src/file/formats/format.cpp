#include "file/formats/format.h"

std::string Format::fmtToStr(Format_e fmt) {
  switch (fmt) {
  case (UNKNOWN):
    return "none";
  case (FAT12):
    return "vfat";
  default:
    return "none";
  }
}
