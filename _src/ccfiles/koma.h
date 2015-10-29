#ifndef KOMA_H
#define KOMA_H
#include "game.h"
#include <nan.h>

namespace komachan {

class Komachan : public Nan::ObjectWrap {
 public:
  static NAN_MODULE_INIT(Init);
  static NAN_METHOD(NewInstance);
  inline double value() const;

 private:
  explicit Komachan(double value = 0);
  ~Komachan();

  static NAN_METHOD(New);
  static NAN_METHOD(GetValue);
  static inline Nan::Persistent<v8::Function> & constructor();
  double value_;
};

}  // namespace komachan

#endif
