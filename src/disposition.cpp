#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

#include "api.hpp"
#include "enum.hpp"

namespace regedit {

Napi::Object _disposition(const Napi::Env& env) {
  Napi::Object disposition = Napi::Object::New(env);

  REGEDIT_MAKE_ENUM(disposition, REG_CREATED_NEW_KEY);
  REGEDIT_MAKE_ENUM(disposition, REG_OPENED_EXISTING_KEY);

  return disposition;
}

}
