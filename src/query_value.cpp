#include "util.hpp"
#include "api.hpp"
#include <cstddef>

namespace regedit {

static inline Napi::Array createStringArray(const Napi::Env& env, const BYTE* data, DWORD cbData) {
  Napi::Array arr = Napi::Array::New(env);
  const wchar_t* p = (const wchar_t*)data;
  const wchar_t* c = p;
  uint32_t index = 0;
  for (DWORD i = 0; i < cbData / 2 - 1; i++) {
    if (*(p + i) == L'\0') {
      arr.Set(index, Napi::String::New(env, w2a((wchar_t*)c)));
      index++;
      c = p + i + 1;
    }
  }
  return arr;
}

template<typename T32>
static inline T32 swap32(const T32& v) {
  return (v >> 24 & 0x000000ff)
    | ((v & 0x00ff0000) >> 8 & 0x00ffffff)
    | ((v & 0x0000ff00) << 8)
    | (v << 24);
}

Napi::Value _queryValue(const Napi::CallbackInfo& info) {
  size_t len = info.Length();
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  
  Napi::Value undefined = env.Undefined();

  if (len < 1) {
    Napi::TypeError::New(env, "[queryValue] arguments.length < 1").ThrowAsJavaScriptException();
    return undefined;
  }

  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "[queryValue] typeof arguments[0] != number").ThrowAsJavaScriptException();
    return undefined;
  }

  HKEY hKey = (HKEY)(ULONG_PTR)info[0].As<Napi::Number>().Int32Value();
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

  switch (type) {
    case REG_SZ:
    case REG_EXPAND_SZ:
    case REG_LINK:
      res["data"] = Napi::String::New(env, w2a((wchar_t*)data));
      break;
    case REG_DWORD:
      res["data"] = Napi::Number::New(env, (double)(*((uint32_t*)data)));
      break;
    case REG_DWORD_BIG_ENDIAN:
      res["data"] = Napi::Number::New(env, (double)swap32(*((uint32_t*)data)));
      break;
    case REG_QWORD:
      res["data"] = Napi::BigInt::New(env, *((uint64_t*)data));
      break;
    case REG_MULTI_SZ:
      res["data"] = createStringArray(env, data, cbData);
      break;
    default:
      res["data"] = Napi::Buffer<BYTE>::Copy(env, data, cbData);
      break;
  }

  delete[] data;
  return res;
}

}
