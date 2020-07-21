#include "util.hpp"
#include "api.hpp"
#include <cstddef>

namespace regedit {

Napi::Value _enumKey(const Napi::CallbackInfo& info) {
  size_t len = info.Length();
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  
  Napi::Value undefined = env.Undefined();

  if (len < 2) {
    Napi::TypeError::New(env, "[enumKey] arguments.length < 2").ThrowAsJavaScriptException();
    return undefined;
  }

  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "[enumKey] typeof arguments[0] != number").ThrowAsJavaScriptException();
    return undefined;
  }

  if (!info[1].IsNumber()) {
    Napi::TypeError::New(env, "[enumKey] typeof arguments[1] != number").ThrowAsJavaScriptException();
    return undefined;
  }

  HKEY hKey = (HKEY)(ULONG_PTR)info[0].As<Napi::Number>().Int32Value();
  DWORD dwIndex = info[1].As<Napi::Number>().Uint32Value();

  DWORD maxSubKeyLen = 0;
  LSTATUS r = RegQueryInfoKeyW(hKey, nullptr, nullptr, nullptr, nullptr, &maxSubKeyLen, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
  if (r != ERROR_SUCCESS) {
    Napi::Error::New(env, errmsg(r)).ThrowAsJavaScriptException();
    return undefined;
  }

  DWORD nameLen = maxSubKeyLen + 1;
  wchar_t* name = new wchar_t[nameLen];
  wchar_t className[MAX_PATH] = { 0 };
  DWORD classLength = MAX_PATH;
  FILETIME lastWriteTime = { 0, 0 };
  r = RegEnumKeyExW(hKey, dwIndex, name, &nameLen, nullptr, className, &classLength, &lastWriteTime);

  if (r != ERROR_SUCCESS) {
    delete[] name;
    Napi::Error::New(env, errmsg(r)).ThrowAsJavaScriptException();
    return undefined;
  }

  Napi::Object res = Napi::Object::New(env);
  res["name"] = Napi::String::New(env, w2a(name));
  // res["nameLength"] = Napi::Number::New(env, nameLen);
  res["className"] = Napi::String::New(env, w2a(className));
  // res["classLength"] = Napi::Number::New(env, classLength);
#if (NAPI_VERSION > 4)
  res["lastWriteTime"] = Napi::Date::New(env, get_time(lastWriteTime));
#else
  Napi::Function DateContructor = env.Global().Get("Date").As<Napi::Function>();
  res["lastWriteTime"] = DateContructor.New({ Napi::Number::New(env, (double)get_time(lastWriteTime)) });
#endif

  delete[] name;
  return res;
}

}
