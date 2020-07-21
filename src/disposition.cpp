#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

#include "api.hpp"

namespace regedit {

Napi::Object _disposition(const Napi::Env& env) {
  Napi::Object disposition = Napi::Object::New(env);
  disposition["REG_CREATED_NEW_KEY"] = Napi::Number::New(env, REG_CREATED_NEW_KEY);
  disposition["REG_OPENED_EXISTING_KEY"] = Napi::Number::New(env, REG_OPENED_EXISTING_KEY);

  return disposition;
}

}
