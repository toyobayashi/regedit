#include "KeyHandle.hpp"
#include "api.hpp"
#include <cstddef>

namespace regedit {

Napi::Value _createKey(const Napi::CallbackInfo& info) {
  size_t len = info.Length();
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  
  Napi::Value undefined = env.Undefined();

  if (len < 2) {
    Napi::TypeError::New(env, "[createKey] arguments.length < 2").ThrowAsJavaScriptException();
    return undefined;
  }

  HKEY hKey;
  if (info[0].IsNumber()) {
    hKey = (HKEY)(ULONG_PTR)info[0].As<Napi::Number>().Int32Value();
  } else if (info[0].IsObject() && info[0].As<Napi::Object>().InstanceOf(KeyHandle::constructor->Value())) {
    hKey = Napi::ObjectWrap<KeyHandle>::Unwrap(info[0].As<Napi::Object>())->getHandle();
  } else {
    Napi::TypeError::New(env, "[createKey] typeof arguments[0] != number | KeyHandle").ThrowAsJavaScriptException();
    return undefined;
  }

  if (!info[1].IsString()) {
    Napi::TypeError::New(env, "[createKey] typeof arguments[1] != string").ThrowAsJavaScriptException();
    return undefined;
  }

  std::wstring subKey = a2w(info[1].As<Napi::String>().Utf8Value());
  const wchar_t* lpSubKey = subKey.c_str();
  
  std::wstring className = L"";
  const wchar_t* lpClass = nullptr;
  if (len < 4) {
    bool str = info[2].IsString();
    bool nul = info[2].IsNull();
    bool und = info[2].IsUndefined();
    if (str) {
      className = a2w(info[2].As<Napi::String>().Utf8Value());
      lpClass = className.c_str();
    } else if (nul || und) {
      lpClass = nullptr;
    } else {
      Napi::TypeError::New(env, "[createKey] typeof arguments[2] != string | null | undefined").ThrowAsJavaScriptException();
      return undefined;
    }
  }

  HKEY result = 0;
  DWORD dwDisposition = 0;
  LSTATUS r = RegCreateKeyExW(hKey, lpSubKey, 0, (LPWSTR)lpClass, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, nullptr, &result, &dwDisposition);
  if (r != ERROR_SUCCESS) {
    Napi::Error::New(env, errmsg(r)).ThrowAsJavaScriptException();
    return undefined;
  }

  Napi::Object res = Napi::Object::New(env);
  res["result"] = KeyHandle::constructor->New({ Napi::Number::New(env, (LONG)(ULONG_PTR)result) });
  res["disposition"] = Napi::Number::New(env, dwDisposition);
  return res;
  // return Napi::Number::New(env, (LONG)(ULONG_PTR)result);
}

}
