#include "KeyHandle.hpp"
#include "value.hpp"
#include "api.hpp"
#include <cstddef>

namespace regedit {

Napi::Value _queryValue(const Napi::CallbackInfo& info) {
  size_t len = info.Length();
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  
  Napi::Value undefined = env.Undefined();

  if (len < 1) {
    Napi::TypeError::New(env, "[queryValue] arguments.length < 1").ThrowAsJavaScriptException();
    return undefined;
  }

  HKEY hKey;
  if (info[0].IsNumber()) {
    hKey = (HKEY)(ULONG_PTR)info[0].As<Napi::Number>().Int32Value();
  } else if (info[0].IsObject() && info[0].As<Napi::Object>().InstanceOf(KeyHandle::constructor->Value())) {
    hKey = Napi::ObjectWrap<KeyHandle>::Unwrap(info[0].As<Napi::Object>())->getHandle();
  } else {
    Napi::TypeError::New(env, "[queryInfoKey] typeof arguments[0] != number | KeyHandle").ThrowAsJavaScriptException();
    return undefined;
  }

  std::wstring valueName = L"";
  const wchar_t* lpValueName = nullptr;

  if (len > 1) {
    bool str = info[1].IsString();
    bool nul = info[1].IsNull();
    bool und = info[1].IsUndefined();
    if (str) {
      valueName = a2w(info[1].As<Napi::String>().Utf8Value());
      lpValueName = valueName.c_str();
    } else if (nul || und) {
      lpValueName = nullptr;
    } else {
      Napi::TypeError::New(env, "[queryValue] typeof arguments[1] != string | null | undefined").ThrowAsJavaScriptException();
      return undefined;
    }
  }

  DWORD cbData = 0;
  LSTATUS r = RegQueryValueExW(hKey, lpValueName, nullptr, nullptr, nullptr, &cbData);

  if (r != ERROR_SUCCESS) {
    Napi::Error::New(env, errmsg(r)).ThrowAsJavaScriptException();
    return undefined;
  }

  DWORD type = 0;
  BYTE* data = new BYTE[cbData];
  r = RegQueryValueExW(hKey, lpValueName, nullptr, &type, data, &cbData);

  if (r != ERROR_SUCCESS) {
    delete[] data;
    Napi::Error::New(env, errmsg(r)).ThrowAsJavaScriptException();
    return undefined;
  }

  Napi::Object res = Napi::Object::New(env);
  res["type"] = Napi::Number::New(env, type);
  res["data"] = createData(env, type, data, cbData);

  delete[] data;
  return res;
}

}
