#include "util.hpp"
#include <napi.h>
#include <cstdio>
#include <cstddef>
#include <cstring>

namespace regedit {

static std::string errmsg(LSTATUS code) {
  std::string r = format_message(code);
  if (code == 5) {
    r += "\nTry to run Node.js with administrator privileges.";
  }
  return r;
}

static Napi::Object _predefinedKeys(const Napi::Env& env) {
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

static Napi::Value _openKey(const Napi::CallbackInfo& info) {
  size_t len = info.Length();
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  
  Napi::Value undefined = env.Undefined();

  if (len < 1) {
    Napi::TypeError::New(env, "[openKey] arguments.length < 1").ThrowAsJavaScriptException();
    return undefined;
  }

  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "[openKey] typeof arguments[0] != number").ThrowAsJavaScriptException();
    return undefined;
  }

  HKEY hKey = (HKEY)(ULONG_PTR)info[0].As<Napi::Number>().Int32Value();
  std::wstring subKey = L"";
  const wchar_t* lpSubKey = nullptr;

  if (len > 1) {
    bool str = info[1].IsString();
    bool nul = info[1].IsNull();
    bool und = info[1].IsUndefined();
    if (str) {
      subKey = a2w(info[1].As<Napi::String>().Utf8Value());
      lpSubKey = subKey.c_str();
    } else if (nul || und) {
      lpSubKey = nullptr;
    } else {
      Napi::TypeError::New(env, "[openKey] typeof arguments[1] != string | null | undefined").ThrowAsJavaScriptException();
      return undefined;
    }
  }

  HKEY result = 0;
  LSTATUS r = RegOpenKeyExW(hKey, lpSubKey, 0, KEY_ALL_ACCESS, &result);
  if (r != ERROR_SUCCESS) {
    Napi::Error::New(env, errmsg(r)).ThrowAsJavaScriptException();
    return undefined;
  }
  return Napi::Number::New(env, (LONG)(ULONG_PTR)result);
}

static Napi::Value _closeKey(const Napi::CallbackInfo& info) {
  size_t len = info.Length();
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  
  Napi::Value undefined = env.Undefined();

  if (len < 1) {
    Napi::TypeError::New(env, "[closeKey] arguments.length < 1").ThrowAsJavaScriptException();
    return undefined;
  }

  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "[closeKey] typeof arguments[0] != number").ThrowAsJavaScriptException();
    return undefined;
  }

  HKEY hKey = (HKEY)(ULONG_PTR)info[0].As<Napi::Number>().Int32Value();
  LSTATUS r = RegCloseKey(hKey);
  if (r != ERROR_SUCCESS) {
    Napi::Error::New(env, errmsg(r)).ThrowAsJavaScriptException();
  }
  return undefined;
}

static Napi::Value _queryInfoKey(const Napi::CallbackInfo& info) {
  size_t len = info.Length();
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  
  Napi::Value undefined = env.Undefined();

  if (len < 1) {
    Napi::TypeError::New(env, "[closeKey] arguments.length < 1").ThrowAsJavaScriptException();
    return undefined;
  }

  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "[closeKey] typeof arguments[0] != number").ThrowAsJavaScriptException();
    return undefined;
  }

  HKEY hKey = (HKEY)(ULONG_PTR)info[0].As<Napi::Number>().Int32Value();

  wchar_t className[MAX_PATH] = { 0 };
  DWORD classLength = MAX_PATH;
  DWORD subKeys = 0;
  DWORD maxSubKeyLen = 0;
  DWORD maxClassLen = 0;
  DWORD values = 0;
  DWORD maxValueNameLen = 0;
  DWORD maxValueLen = 0;
  DWORD securityDescriptor = 0;

  FILETIME lastWriteTime = { 0, 0 };

  LSTATUS r = RegQueryInfoKeyW(hKey,
    className,
    &classLength,
    nullptr,
    &subKeys,
    &maxSubKeyLen,
    &maxClassLen,
    &values,
    &maxValueNameLen,
    &maxValueLen,
    &securityDescriptor,
    &lastWriteTime);
  if (r != ERROR_SUCCESS) {
    Napi::Error::New(env, errmsg(r)).ThrowAsJavaScriptException();
    return undefined;
  }

  Napi::Object res = Napi::Object::New(env);
  res["className"] = Napi::String::New(env, w2a(className));
  res["classLength"] = Napi::Number::New(env, classLength);
  res["subKeys"] = Napi::Number::New(env, subKeys);
  res["maxSubKeyLen"] = Napi::Number::New(env, maxSubKeyLen);
  res["maxClassLen"] = Napi::Number::New(env, maxClassLen);
  res["values"] = Napi::Number::New(env, values);
  res["maxValueNameLen"] = Napi::Number::New(env, maxValueNameLen);
  res["maxValueLen"] = Napi::Number::New(env, maxValueLen);
  res["securityDescriptor"] = Napi::Number::New(env, securityDescriptor);
  res["lastWriteTime"] = Napi::Date::New(env, get_time(lastWriteTime));

  return res;
}

static Napi::Object _init(Napi::Env env, Napi::Object exports) {
  exports["openKey"] = Napi::Function::New(env, _openKey, "openKey");
  exports["closeKey"] = Napi::Function::New(env, _closeKey, "closeKey");
  exports["queryInfoKey"] = Napi::Function::New(env, _queryInfoKey, "queryInfoKey");
  exports["predefinedKeys"] = _predefinedKeys(env);
  return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, _init)

}
