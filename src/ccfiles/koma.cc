#include "koma.h"

namespace komachan {

void Komachan::NAN_MODULE_INIT(Init) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "getValue", GetValue);

  constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
}

Komachan::NAN_METHOD(NewInstance) {
  v8::Local<v8::Function> cons = Nan::New(constructor());
  double value = info[0]->IsNumber() ? Nan::To<double>(info[0]).FromJust() : 0;
  const int argc = 1;
  v8::Local<v8::Value> argv[1] = {Nan::New(value)};
  info.GetReturnValue().Set(Nan::NewInstance(cons, argc, argv).ToLocalChecked());
}

// Needed for the next example:
Komachan::inline double value() const {
  return value_;
}

explicit Komachan(double value = 0) : value_(value) {}
~Komachan() {}

Komachan::NAN_METHOD(New) {
  if (info.IsConstructCall()) {
    double value = info[0]->IsNumber() ? Nan::To<double>(info[0]).FromJust() : 0;
    Komachan * obj = new Komachan(value);
    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  } else {
    const int argc = 1;
    v8::Local<v8::Value> argv[argc] = {info[0]};
    v8::Local<v8::Function> cons = Nan::New(constructor());
    info.GetReturnValue().Set(Nan::NewInstance(cons, argc, argv).ToLocalChecked());
  }
}

Komachan::NAN_METHOD(GetValue) {
  Komachan* obj = ObjectWrap::Unwrap<Komachan>(info.This());
  info.GetReturnValue().Set(obj->value_);
}

Komachan::inline Nan::Persistent<v8::Function> & constructor() {
  static Nan::Persistent<v8::Function> my_constructor;
  return my_constructor;
}


}  // namespace komachan
