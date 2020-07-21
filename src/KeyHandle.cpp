#include "KeyHandle.hpp"

namespace regedit {

Napi::FunctionReference* KeyHandle::constructor = nullptr;

void KeyHandle::init(const Napi::Env& env) {
  Napi::Function func = DefineClass(env, "KeyHandle", {
    InstanceMethod<&KeyHandle::getValue>("getValue"),
    InstanceMethod<&KeyHandle::isClosed>("isClosed")
  });

  if (constructor == nullptr) {
    constructor = new Napi::FunctionReference();
  }

  *constructor = Napi::Persistent(func);
}

Napi::Value KeyHandle::getValue(const Napi::CallbackInfo& info) {
  return Napi::Number::New(info.Env(), (LONG)(ULONG_PTR)hKey_);
}

Napi::Value KeyHandle::isClosed(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(info.Env(), closed_);
}

HKEY KeyHandle::getHandle() {
  return this->hKey_;
}

void KeyHandle::setClosed(bool v) {
  this->closed_ = v;
}

bool KeyHandle::isOpen() {
  return !this->closed_;
}

void KeyHandle::Finalize(Napi::Env env) {
  if (!this->closed_) {
    LSTATUS r = RegCloseKey(hKey_);
    if (r != ERROR_SUCCESS) {
      Napi::Error::New(env, errmsg(r)).ThrowAsJavaScriptException();
      return;
    }
    this->closed_ = true;
  }
}

KeyHandle::KeyHandle(const Napi::CallbackInfo& info): Napi::ObjectWrap<KeyHandle>(info) {
  this->hKey_ = (HKEY)(ULONG_PTR)info[0].As<Napi::Number>().Int32Value();
  this->closed_ = false;
}

}
