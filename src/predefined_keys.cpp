#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

#include "api.hpp"

namespace regedit {

Napi::Object _predefinedKeys(const Napi::Env& env) {
  Napi::Object predefinedKeys = Napi::Object::New(env);
  predefinedKeys["HKEY_CLASSES_ROOT"] = Napi::Number::New(env, (LONG)(ULONG_PTR)HKEY_CLASSES_ROOT);
  predefinedKeys["HKEY_CURRENT_USER"] = Napi::Number::New(env, (LONG)(ULONG_PTR)HKEY_CURRENT_USER);
  predefinedKeys["HKEY_LOCAL_MACHINE"] = Napi::Number::New(env, (LONG)(ULONG_PTR)HKEY_LOCAL_MACHINE);
  predefinedKeys["HKEY_USERS"] = Napi::Number::New(env, (LONG)(ULONG_PTR)HKEY_USERS);
  predefinedKeys["HKEY_PERFORMANCE_DATA"] = Napi::Number::New(env, (LONG)(ULONG_PTR)HKEY_PERFORMANCE_DATA);
  predefinedKeys["HKEY_PERFORMANCE_TEXT"] = Napi::Number::New(env, (LONG)(ULONG_PTR)HKEY_PERFORMANCE_TEXT);
  predefinedKeys["HKEY_PERFORMANCE_NLSTEXT"] = Napi::Number::New(env, (LONG)(ULONG_PTR)HKEY_PERFORMANCE_NLSTEXT);
#if (WINVER >= 0x0400)
  predefinedKeys["HKEY_CURRENT_CONFIG"] = Napi::Number::New(env, (LONG)(ULONG_PTR)HKEY_CURRENT_CONFIG);
  predefinedKeys["HKEY_DYN_DATA"] = Napi::Number::New(env, (LONG)(ULONG_PTR)HKEY_DYN_DATA);
  predefinedKeys["HKEY_CURRENT_USER_LOCAL_SETTINGS"] = Napi::Number::New(env, (LONG)(ULONG_PTR)HKEY_CURRENT_USER_LOCAL_SETTINGS);
#endif
  return predefinedKeys;
}

}
