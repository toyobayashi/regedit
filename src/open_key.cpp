#include "util.hpp"
#include "api.hpp"
#include <cstddef>

namespace regedit {

Napi::Value _openKey(const Napi::CallbackInfo& info) {
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

}
