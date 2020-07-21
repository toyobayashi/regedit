#ifndef __REGEDIT_KEYHANDLE_HPP__
#define __REGEDIT_KEYHANDLE_HPP__

#include "util.hpp"
#include <napi.h>

namespace regedit {

class KeyHandle : public Napi::ObjectWrap<KeyHandle> {
 public:
  static Napi::FunctionReference* constructor;

  static void init(const Napi::Env& env);

  void Finalize(Napi::Env env);

  Napi::Value getValue(const Napi::CallbackInfo& info);
  Napi::Value isClosed(const Napi::CallbackInfo& info);
  HKEY getHandle();

  void setClosed(bool);
  bool isOpen();

  KeyHandle(const Napi::CallbackInfo& info);
 private:
  HKEY hKey_;
  bool closed_;
};

}

#endif
