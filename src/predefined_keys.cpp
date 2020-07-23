#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

#include "api.hpp"
#include "enum.hpp"

namespace regedit {

Napi::Object _predefinedKeys(const Napi::Env& env) {
  Napi::Object predefinedKeys = Napi::Object::New(env);
  
  REGEDIT_MAKE_ENUM_EX(predefinedKeys, HKEY_CLASSES_ROOT, (LONG)(ULONG_PTR));
  REGEDIT_MAKE_ENUM_EX(predefinedKeys, HKEY_CURRENT_USER, (LONG)(ULONG_PTR));
  REGEDIT_MAKE_ENUM_EX(predefinedKeys, HKEY_LOCAL_MACHINE, (LONG)(ULONG_PTR));
  REGEDIT_MAKE_ENUM_EX(predefinedKeys, HKEY_USERS, (LONG)(ULONG_PTR));
  REGEDIT_MAKE_ENUM_EX(predefinedKeys, HKEY_PERFORMANCE_DATA, (LONG)(ULONG_PTR));
  REGEDIT_MAKE_ENUM_EX(predefinedKeys, HKEY_PERFORMANCE_TEXT, (LONG)(ULONG_PTR));
  REGEDIT_MAKE_ENUM_EX(predefinedKeys, HKEY_PERFORMANCE_NLSTEXT, (LONG)(ULONG_PTR));
#if (WINVER >= 0x0400)
  REGEDIT_MAKE_ENUM_EX(predefinedKeys, HKEY_CURRENT_CONFIG, (LONG)(ULONG_PTR));
  REGEDIT_MAKE_ENUM_EX(predefinedKeys, HKEY_DYN_DATA, (LONG)(ULONG_PTR));
  REGEDIT_MAKE_ENUM_EX(predefinedKeys, HKEY_CURRENT_USER_LOCAL_SETTINGS, (LONG)(ULONG_PTR));
#endif
  return predefinedKeys;
}

}
