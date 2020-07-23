#ifndef __REGEDIT_ENUM_HPP__
#define __REGEDIT_ENUM_HPP__

#define REGEDIT_MAKE_ENUM(obj, value) \
  do { \
    (obj)[#value] = Napi::Number::New(env, value); \
    (obj).Set(Napi::Number::New(env, value), Napi::String::New(env, #value)); \
  } while(0)

#define REGEDIT_MAKE_ENUM_EX(obj, value, type) \
  do { \
    (obj)[#value] = Napi::Number::New(env, type value); \
    (obj).Set(Napi::Number::New(env, type value), Napi::String::New(env, #value)); \
  } while(0)

#endif
