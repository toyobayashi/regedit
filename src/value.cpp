#include "util.hpp"
#include "value.hpp"

namespace regedit {

Napi::Array createStringArray(const Napi::Env& env, const BYTE* data, DWORD cbData) {
  Napi::Array arr = Napi::Array::New(env);
  const wchar_t* p = (const wchar_t*)data;
  const wchar_t* c = p;
  uint32_t index = 0;
  for (DWORD i = 0; i < cbData / 2 - 1; i++) {
    if (*(p + i) == L'\0') {
      arr.Set(index, Napi::String::New(env, w2a((wchar_t*)c)));
      index++;
      c = p + i + 1;
    }
  }
  return arr;
}

Napi::Value createData(const Napi::Env& env, DWORD type, const BYTE* data, DWORD cbData) {
  switch (type) {
    case REG_SZ:
    case REG_EXPAND_SZ:
    case REG_LINK:
      return Napi::String::New(env, w2a((wchar_t*)data));
    case REG_DWORD:
      return Napi::Number::New(env, (double)(*((uint32_t*)data)));
    case REG_DWORD_BIG_ENDIAN:
      return Napi::Number::New(env, (double)swap32(*((uint32_t*)data)));
    case REG_QWORD:
      return Napi::BigInt::New(env, *((uint64_t*)data));
    case REG_MULTI_SZ:
      return createStringArray(env, data, cbData);
    default:
      return Napi::Buffer<BYTE>::Copy(env, data, cbData);
  }
}

}