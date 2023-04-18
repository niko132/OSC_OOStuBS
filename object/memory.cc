#include "object/memory.h"

void memcpy(void* dst, void* src, unsigned int count) {
  unsigned char* dstCast = (unsigned char*)dst;
  unsigned char* srcCast = (unsigned char*)src;

  for (unsigned int i = 0; i < count; i++) {
    dstCast[i] = srcCast[i];
  }
}
