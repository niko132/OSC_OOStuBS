#ifndef __memory_include__
#define __memory_include__

void memcpy(void* dst, void* src, unsigned int count);

template<typename T>
void memset(void* dst, T val, unsigned int count) {
  T* dstCast = (T*)dst;
  for (unsigned int i = 0; i < count; i++) {
    dstCast[i] = val;
  }
};

#endif
