#include "KeyHandle.hpp"
#include "api.hpp"
#include <cstddef>
#include <cstdlib>
#include <vector>
#include <cstring>

namespace regedit {

Napi::Value _setValue(const Napi::CallbackInfo& info) {
  size_t len = info.Length();
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  
  Napi::Value undefined = env.Undefined();

  if (len < 2) {
    Napi::TypeError::New(env, "[setValue] arguments.length < 2").ThrowAsJavaScriptException();
    return undefined;
  }

  HKEY hKey;
  if (info[0].IsNumber()) {
    hKey = (HKEY)(ULONG_PTR)info[0].As<Napi::Number>().Int32Value();
  } else if (info[0].IsObject() && info[0].As<Napi::Object>().InstanceOf(KeyHandle::constructor->Value())) {
    hKey = Napi::ObjectWrap<KeyHandle>::Unwrap(info[0].As<Napi::Object>())->getHandle();
  } else {
    Napi::TypeError::New(env, "[setValue] typeof arguments[0] != number | KeyHandle").ThrowAsJavaScriptException();
    return undefined;
  }

  if (!info[1].IsString()) {
    Napi::TypeError::New(env, "[setValue] typeof arguments[1] != string").ThrowAsJavaScriptException();
    return undefined;
  }

  std::wstring valueName = a2w(info[1].As<Napi::String>().Utf8Value());
  const wchar_t* lpValueName = valueName.c_str();

  DWORD dwType = REG_SZ;
  BYTE* lpData = nullptr;
  DWORD cbData = 0;

  if (len >= 3) {
    Napi::Value data = info[2];
    if (data.IsUndefined() || data.IsNull()) {
      lpData = nullptr;
      cbData = 0;
    } else if (data.IsString()) {
      std::wstring strData = a2w(data.As<Napi::String>().Utf8Value());
      size_t l = (strData.length() + 1) * 2;
      lpData = new BYTE[l];
      memcpy(lpData, strData.c_str(), l);
      cbData = l;
    } else if (data.IsNumber()) {
      int64_t numberData = data.As<Napi::Number>().Int64Value();
      if (numberData > 4294967295/*  || numberData < 0 */) {
        dwType = REG_QWORD;
        size_t l = 8;
        lpData = new BYTE[l];
        memcpy(lpData, &numberData, l);
        cbData = l;
      } else {
        uint32_t num = data.As<Napi::Number>().Uint32Value();
        dwType = REG_DWORD;
        size_t l = 4;
        lpData = new BYTE[l];
        memcpy(lpData, &num, l);
        cbData = l;
      }
    } else if (data.IsArray()) {
      size_t totalLength = 0;
      std::vector<std::wstring> wstrArr;
      Napi::Array arr = data.As<Napi::Array>();
      for (uint32_t i = 0; i < arr.Length(); i++) {
        Napi::Value item = arr.Get(i);
        if (!item.IsString()) {
          Napi::TypeError::New(env, "[setValue] typeof arguments[2] != string[]").ThrowAsJavaScriptException();
          return undefined;
        }
        std::wstring wstr = a2w(item.As<Napi::String>().Utf8Value());
        wstrArr.push_back(wstr);
        totalLength += wstr.length() + 1;
      }
      totalLength++;

      dwType = REG_MULTI_SZ;
      size_t l = totalLength * 2;
      lpData = new BYTE[l];
      memset(lpData, 0, l);
      size_t pos = 0;
      size_t len = 0;
      for (uint32_t i = 0; i < wstrArr.size(); i++) {
        len = (wstrArr[i].length() + 1) * 2;
        memcpy(lpData + pos, wstrArr[i].c_str(), len);
        pos += len;
      }
      cbData = l;
    }
#if NAPI_VERSION > 5   
    else if (data.IsBigInt()) {
      dwType = REG_QWORD;
      bool lossless = true;
      uint64_t numberData = data.As<Napi::BigInt>().Uint64Value(&lossless);
      if (!lossless) {
        Napi::RangeError::New(env, "[setValue] BigInt range error.").ThrowAsJavaScriptException();
        return undefined;
      }
      size_t l = sizeof(uint64_t);
      lpData = new BYTE[l];
      memcpy(lpData, &numberData, l);
      cbData = l;
    }
#else
    else if (data.Type() == 9) {
      std::string str = data.As<Napi::Object>().Get("toString").As<Napi::Function>().Call(data, {}).As<Napi::String>().Utf8Value();
      uint64_t numberData = std::strtoull(str.c_str(), nullptr, 10);
      dwType = REG_QWORD;
      size_t l = 8;
      lpData = new BYTE[l];
      memcpy(lpData, &numberData, l);
      cbData = l;
    }
#endif
    else if (data.IsBuffer()) {
      dwType = REG_BINARY;
      Napi::Buffer<BYTE> bufferData = data.As<Napi::Buffer<BYTE>>();
      size_t l = bufferData.ByteLength();
      lpData = new BYTE[l];
      memcpy(lpData, bufferData.Data(), l);
      cbData = l;
    } else {
      Napi::TypeError::New(env, "[setValue] typeof arguments[2] != number | string | string[] | Buffer").ThrowAsJavaScriptException();
      return undefined;
    }
  }

  if (len >= 4) {
    if (!info[3].IsNumber()) {
      if (lpData) delete[] lpData;
      Napi::TypeError::New(env, "[setValue] typeof arguments[3] != number").ThrowAsJavaScriptException();
      return undefined;
    }
    DWORD customType = info[3].As<Napi::Number>().Uint32Value();
    if (info[2].IsString()) {
      if (customType != REG_SZ && customType != REG_EXPAND_SZ && customType != REG_LINK && customType != REG_BINARY) {
        if (lpData) delete[] lpData;
        Napi::Error::New(env, "[setValue] Error value type.").ThrowAsJavaScriptException();
        return undefined;
      }
      dwType = customType;
    } else if (info[2].IsNumber()) {
      if (customType == REG_DWORD) {
        if (dwType != REG_DWORD) {
          dwType = customType;
          uint32_t numberData = (uint64_t) info[2].As<Napi::Number>().Uint32Value();
          size_t l = sizeof(uint32_t);
          if (lpData) delete[] lpData;
          lpData = new BYTE[l];
          memcpy(lpData, &numberData, l);
          cbData = l;
        }
      } else if (customType == REG_DWORD_BIG_ENDIAN) {
        if (dwType != REG_DWORD) {
          dwType = customType;
          uint32_t numberData = (uint64_t) info[2].As<Napi::Number>().Uint32Value();
          size_t l = sizeof(uint32_t);
          if (lpData) delete[] lpData;
          lpData = new BYTE[l];
          memcpy(lpData, &numberData, l);
          cbData = l;
        }
        BYTE tmp = *lpData;
        *lpData = *(lpData + 3);
        *(lpData + 3) = tmp;
        tmp = *(lpData + 1);
        *(lpData + 1) = *(lpData + 2);
        *(lpData + 2) = tmp;
        dwType = customType;
      } else if (customType == REG_QWORD) {
        if (dwType != REG_QWORD) {
          dwType = customType;
          uint64_t numberData = (uint64_t) info[2].As<Napi::Number>().Int64Value();
          size_t l = sizeof(uint64_t);
          if (lpData) delete[] lpData;
          lpData = new BYTE[l];
          memcpy(lpData, &numberData, l);
          cbData = l;
        }
      } else {
        if (lpData) delete[] lpData;
        Napi::Error::New(env, "[setValue] Error value type.").ThrowAsJavaScriptException();
        return undefined;
      }
    } else if (info[2].IsBuffer()) { 
      size_t bytelen = info[2].As<Napi::Buffer<BYTE>>().ByteLength();
      if (
        ((customType == REG_DWORD || customType == REG_DWORD_BIG_ENDIAN) && bytelen != 4) ||
        ((customType == REG_QWORD) && bytelen != 8)
      ) {
        if (lpData) delete[] lpData;
        Napi::Error::New(env, "[setValue] Error value type.").ThrowAsJavaScriptException();
        return undefined;
      }

      dwType = customType;
    }
#if NAPI_VERSION > 5 
    else if (info[2].IsBigInt()) {
#else
    else if (info[2].Type() == 9) {
#endif
      if (customType != REG_QWORD) {
        if (lpData) delete[] lpData;
        Napi::Error::New(env, "[setValue] Error value type.").ThrowAsJavaScriptException();
        return undefined;
      }

      dwType = customType;
    } else {
      if (customType != REG_BINARY) {
        if (lpData) delete[] lpData;
        Napi::Error::New(env, "[setValue] Error value type.").ThrowAsJavaScriptException();
        return undefined;
      }
    }
  }

  LSTATUS r = RegSetValueExW(hKey, lpValueName, 0, dwType, lpData, cbData);
  if (lpData) delete[] lpData;
  if (r != ERROR_SUCCESS) {
    Napi::Error::New(env, errmsg(r)).ThrowAsJavaScriptException();
    return undefined;
  }

  return undefined;
}

}
