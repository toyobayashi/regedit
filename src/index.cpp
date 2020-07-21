#include "util.hpp"
#include "api.hpp"

namespace regedit {

static Napi::Object _init(Napi::Env env, Napi::Object exports) {
  exports["openKey"] = Napi::Function::New(env, _openKey, "openKey");
  exports["closeKey"] = Napi::Function::New(env, _closeKey, "closeKey");
  exports["queryInfoKey"] = Napi::Function::New(env, _queryInfoKey, "queryInfoKey");
  exports["queryValue"] = Napi::Function::New(env, _queryValue, "queryValue");
  exports["enumKey"] = Napi::Function::New(env, _enumKey, "enumKey");
  exports["enumValue"] = Napi::Function::New(env, _enumValue, "enumValue");
  exports["PredefinedKeys"] = _predefinedKeys(env);
  exports["ValueTypes"] = _valueTypes(env);
  return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, _init)

}
