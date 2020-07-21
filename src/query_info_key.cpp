#include "util.hpp"
#include "api.hpp"
#include <cstddef>

namespace regedit {

Napi::Value _queryInfoKey(const Napi::CallbackInfo& info) {
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
#if (NAPI_VERSION > 4)
  res["lastWriteTime"] = Napi::Date::New(env, get_time(lastWriteTime));
#else
  Napi::Function DateContructor = env.Global().Get("Date").As<Napi::Function>();
  res["lastWriteTime"] = DateContructor.New({ Napi::Number::New(env, (double)get_time(lastWriteTime)) });
#endif
  return res;
}

}
