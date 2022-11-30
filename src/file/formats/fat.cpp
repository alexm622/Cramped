#include <file/formats/fat.h>

void Fat::format() {
  writeBPB();
  writeExBR();
  writeExtras();
}
