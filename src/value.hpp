#ifndef __REGEDIT_VALUE_HPP__
#define __REGEDIT_VALUE_HPP__


#include <napi.h>

namespace regedit {

template<typename T32>
inline T32 swap32(const T32& v) {
  return (v >> 24 & 0x000000ff)
    | ((v & 0x00ff0000) >> 8 & 0x00ffffff)
    | ((v & 0x0000ff00) << 8)
    | (v << 24);
}

Napi::Array createStringArray(const Napi::Env& env, const unsigned char* data, unsigned long cbData);

Napi::Value createData(const Napi::Env& env, unsigned long type, const unsigned char* data, unsigned long cbData);

}

#endif
