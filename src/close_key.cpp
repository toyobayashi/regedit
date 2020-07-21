#include "util.hpp"
#include "api.hpp"
#include <cstddef>

namespace regedit {

Napi::Value _closeKey(const Napi::CallbackInfo& info) {
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

}
