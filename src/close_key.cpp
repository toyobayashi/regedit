#include "KeyHandle.hpp"
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

  /* if (info[0].IsNumber()) {
    LSTATUS r = RegCloseKey((HKEY)(ULONG_PTR)info[0].As<Napi::Number>().Int32Value());
    if (r != ERROR_SUCCESS) {
      Napi::Error::New(env, errmsg(r)).ThrowAsJavaScriptException();
      return undefined;
    }
  } else  */if (info[0].IsObject() && info[0].As<Napi::Object>().InstanceOf(KeyHandle::constructor->Value())) {
    KeyHandle* h = Napi::ObjectWrap<KeyHandle>::Unwrap(info[0].As<Napi::Object>());

    if (h->isOpen()) {
      LSTATUS r = RegCloseKey(h->getHandle());
      if (r != ERROR_SUCCESS) {
        Napi::Error::New(env, errmsg(r)).ThrowAsJavaScriptException();
        return undefined;
      }
      h->setClosed(true);
    }
  } else {
    Napi::TypeError::New(env, "[closeKey] typeof arguments[0] != KeyHandle").ThrowAsJavaScriptException();
  }

  return undefined;
}

}
