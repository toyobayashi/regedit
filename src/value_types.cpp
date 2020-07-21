#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

#include "api.hpp"

namespace regedit {

Napi::Object _valueTypes(const Napi::Env& env) {
  Napi::Object valueTypes = Napi::Object::New(env);
  valueTypes["REG_NONE"] = Napi::Number::New(env, REG_NONE);
  valueTypes["REG_SZ"] = Napi::Number::New(env, REG_SZ);
  valueTypes["REG_EXPAND_SZ"] = Napi::Number::New(env, REG_EXPAND_SZ);
  valueTypes["REG_BINARY"] = Napi::Number::New(env, REG_BINARY);
  valueTypes["REG_DWORD"] = Napi::Number::New(env, REG_DWORD);
  // valueTypes["REG_DWORD_LITTLE_ENDIAN"] = Napi::Number::New(env, REG_DWORD_LITTLE_ENDIAN);
  valueTypes["REG_DWORD_BIG_ENDIAN"] = Napi::Number::New(env, REG_DWORD_BIG_ENDIAN);
  valueTypes["REG_LINK"] = Napi::Number::New(env, REG_LINK);
  valueTypes["REG_MULTI_SZ"] = Napi::Number::New(env, REG_MULTI_SZ);
  valueTypes["REG_RESOURCE_LIST"] = Napi::Number::New(env, REG_RESOURCE_LIST);
  valueTypes["REG_FULL_RESOURCE_DESCRIPTOR"] = Napi::Number::New(env, REG_FULL_RESOURCE_DESCRIPTOR);
  valueTypes["REG_RESOURCE_REQUIREMENTS_LIST"] = Napi::Number::New(env, REG_RESOURCE_REQUIREMENTS_LIST);
  valueTypes["REG_QWORD"] = Napi::Number::New(env, REG_QWORD);
  // valueTypes["REG_QWORD_LITTLE_ENDIAN"] = Napi::Number::New(env, REG_QWORD_LITTLE_ENDIAN);

  return valueTypes;
}

}
