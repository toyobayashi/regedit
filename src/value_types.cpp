#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

#include "api.hpp"
#include "enum.hpp"

namespace regedit {

Napi::Object _valueTypes(const Napi::Env& env) {
  Napi::Object valueTypes = Napi::Object::New(env);

  REGEDIT_MAKE_ENUM(valueTypes, REG_NONE);
  REGEDIT_MAKE_ENUM(valueTypes, REG_SZ);
  REGEDIT_MAKE_ENUM(valueTypes, REG_EXPAND_SZ);
  REGEDIT_MAKE_ENUM(valueTypes, REG_BINARY);
  REGEDIT_MAKE_ENUM(valueTypes, REG_DWORD);
  // REGEDIT_MAKE_ENUM(valueTypes, REG_DWORD_LITTLE_ENDIAN);
  REGEDIT_MAKE_ENUM(valueTypes, REG_DWORD_BIG_ENDIAN);
  REGEDIT_MAKE_ENUM(valueTypes, REG_LINK);
  REGEDIT_MAKE_ENUM(valueTypes, REG_MULTI_SZ);
  REGEDIT_MAKE_ENUM(valueTypes, REG_RESOURCE_LIST);
  REGEDIT_MAKE_ENUM(valueTypes, REG_FULL_RESOURCE_DESCRIPTOR);
  REGEDIT_MAKE_ENUM(valueTypes, REG_RESOURCE_REQUIREMENTS_LIST);
  REGEDIT_MAKE_ENUM(valueTypes, REG_QWORD);
  // REGEDIT_MAKE_ENUM(valueTypes, REG_QWORD_LITTLE_ENDIAN);

  return valueTypes;
}

}
