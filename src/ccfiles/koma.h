#ifndef KOMA_H
#define KOMA_H
#include "game.h"
#include <node.h>
#include <node_object_wrap.h>

namespace komachan {

class Koma : public node::ObjectWrap {
 public:
  static void Init(v8::Local<v8::Object> exports);

 private:
  explicit Koma(double value = 0);
  ~Koma();

  static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void PlusOne(const v8::FunctionCallbackInfo<v8::Value>& args);
  static v8::Persistent<v8::Function> constructor;
  double value_;
};

}  // namespace komachan

#endif
