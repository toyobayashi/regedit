#ifndef __REGEDIT_API_HPP__
#define __REGEDIT_API_HPP__

#include <napi.h>

namespace regedit {

Napi::Object _predefinedKeys(const Napi::Env& env);
Napi::Object _valueTypes(const Napi::Env& env);
Napi::Object _disposition(const Napi::Env& env);
Napi::Value _openKey(const Napi::CallbackInfo&);
Napi::Value _closeKey(const Napi::CallbackInfo&);
Napi::Value _queryInfoKey(const Napi::CallbackInfo&);
Napi::Value _queryValue(const Napi::CallbackInfo&);
Napi::Value _enumKey(const Napi::CallbackInfo&);
Napi::Value _enumValue(const Napi::CallbackInfo&);
Napi::Value _createKey(const Napi::CallbackInfo&);

}

#endif
