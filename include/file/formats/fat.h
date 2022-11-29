#ifndef FAT_H
#define FAT_H

#include "file/formats/format.h"

class Fat : public Format {
public:
  Fat(std::ofstream &file, lli type);
  void format() override;

  virtual void writeBPB();
  virtual void writeexBR();
  virtual void writeExtras();

private:
};

#endif
