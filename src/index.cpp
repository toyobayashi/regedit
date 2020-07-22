#include "KeyHandle.hpp"
#include "api.hpp"

namespace regedit {

static Napi::Object _init(Napi::Env env, Napi::Object exports) {
  KeyHandle::init(env);
  exports["openKey"] = Napi::Function::New(env, _openKey, "openKey"); // RegOpenKeyExW
  exports["closeKey"] = Napi::Function::New(env, _closeKey, "closeKey"); // RegCloseKey
  exports["queryInfoKey"] = Napi::Function::New(env, _queryInfoKey, "queryInfoKey"); // RegQueryInfoKeyW
  exports["queryValue"] = Napi::Function::New(env, _queryValue, "queryValue"); // RegQueryValueExW
  exports["enumKey"] = Napi::Function::New(env, _enumKey, "enumKey"); // RegEnumKeyExW
  exports["enumValue"] = Napi::Function::New(env, _enumValue, "enumValue"); // RegEnumValueW
  exports["createKey"] = Napi::Function::New(env, _createKey, "createKey"); // RegCreateKeyExW
  exports["deleteKey"] = Napi::Function::New(env, _deleteKey, "deleteKey"); // RegDeleteKeyW
  exports["setValue"] = Napi::Function::New(env, _setValue, "setValue"); // RegSetValueExW
  exports["deleteValue"] = Napi::Function::New(env, _deleteValue, "deleteValue"); // RegDeleteValueW
  exports["PredefinedKeys"] = _predefinedKeys(env);
  exports["ValueTypes"] = _valueTypes(env);
  exports["Disposition"] = _disposition(env);
  return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, _init)

}
