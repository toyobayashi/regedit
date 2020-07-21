#include "util.hpp"
#include "value.hpp"
#include "api.hpp"
#include <cstddef>

namespace regedit {

Napi::Value _enumValue(const Napi::CallbackInfo& info) {
  size_t len = info.Length();
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  
  Napi::Value undefined = env.Undefined();

  if (len < 2) {
    Napi::TypeError::New(env, "[enumValue] arguments.length < 2").ThrowAsJavaScriptException();
    return undefined;
  }

  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "[enumValue] typeof arguments[0] != number").ThrowAsJavaScriptException();
    return undefined;
  }

  if (!info[1].IsNumber()) {
    Napi::TypeError::New(env, "[enumValue] typeof arguments[1] != number").ThrowAsJavaScriptException();
    return undefined;
  }

  HKEY hKey = (HKEY)(ULONG_PTR)info[0].As<Napi::Number>().Int32Value();
  DWORD dwIndex = info[1].As<Napi::Number>().Uint32Value();

  DWORD dataLen = 0;
  DWORD nameLen = 0;
  LSTATUS r = RegQueryInfoKeyW(hKey, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, &nameLen, &dataLen, nullptr, nullptr);
  if (r != ERROR_SUCCESS) {
    Napi::Error::New(env, errmsg(r)).ThrowAsJavaScriptException();
    return undefined;
  }
  nameLen++;

  wchar_t* name = new wchar_t[nameLen];
  BYTE* data = new BYTE[dataLen];
  DWORD type = 0;
  r = RegEnumValueW(hKey, dwIndex, name, &nameLen, nullptr, &type, data, &dataLen);

  if (r != ERROR_SUCCESS) {
    delete[] name;
    delete[] data;
    Napi::Error::New(env, errmsg(r)).ThrowAsJavaScriptException();
    return undefined;
  }

  Napi::Object res = Napi::Object::New(env);
  res["name"] = Napi::String::New(env, w2a(name));
  res["type"] = Napi::Number::New(env, type);
  res["data"] = createData(env, type, data, dataLen);

  delete[] name;
  delete[] data;
  return res;
}

}
