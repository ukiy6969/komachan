#include <nan.h>
#include <iostream>
#include "io.h"

namespace komachan {


class Komachan : public Nan::ObjectWrap {
 public:
  static NAN_MODULE_INIT(Init) {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->InstanceTemplate()->SetInternalFieldCount(5);

    Nan::SetPrototypeMethod(tpl, "start", Start);
    Nan::SetPrototypeMethod(tpl, "move", Move);
    Nan::SetPrototypeMethod(tpl, "search", Search);
    Nan::SetPrototypeMethod(tpl, "legal", Legal);
    Nan::SetPrototypeMethod(tpl, "print", Print);

    constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
  }

  static NAN_METHOD(NewInstance) {
    v8::Local<v8::Function> cons = Nan::New(constructor());
    //double value = info[0]->IsNumber() ? Nan::To<double>(info[0]).FromJust() : 0;
    const int argc = 1;
    std::string binPath;
    v8::Local<v8::Object> opt = Nan::To<v8::Object>(info[0]).ToLocalChecked();
    if (Nan::Has(opt, Nan::New<v8::String>("binPath").ToLocalChecked()).FromJust()) {
      v8::Local<v8::String> _binPath = Nan::To<v8::String>(Nan::Get(opt, Nan::New<v8::String>("binPath").ToLocalChecked()).ToLocalChecked()).ToLocalChecked();
      Nan::Utf8String _bp(_binPath);
      binPath = *_bp;
    }
    v8::Local<v8::Value> argv[1] = {info[0]};
    info.GetReturnValue().Set(Nan::NewInstance(cons, argc, argv).ToLocalChecked());
  }


 private:
  explicit Komachan(std::string binPath = "./setting") : game(binPath), io(&game) {
  }
  ~Komachan() {
    std::cout << "delete komachan" << std::endl;
  }

  static NAN_METHOD(New) {
    if (info.IsConstructCall()) {
      std::string binPath;
      v8::Local<v8::Object> opt = Nan::To<v8::Object>(info[0]).ToLocalChecked();
      if (Nan::Has(opt, Nan::New<v8::String>("binPath").ToLocalChecked()).FromJust()) {
        v8::Local<v8::String> _binPath = Nan::To<v8::String>(Nan::Get(opt, Nan::New<v8::String>("binPath").ToLocalChecked()).ToLocalChecked()).ToLocalChecked();
        Nan::Utf8String _bp(_binPath);
        binPath = *_bp;
      }
      Komachan * obj = new Komachan(binPath);
      obj->Wrap(info.This());
      info.GetReturnValue().Set(info.This());
    } else {
      const int argc = 1;
      v8::Local<v8::Value> argv[argc] = {info[0]};
      v8::Local<v8::Function> cons = Nan::New(constructor());
      info.GetReturnValue().Set(Nan::NewInstance(cons, argc, argv).ToLocalChecked());
    }
  }

  static NAN_METHOD(Start) {
    Komachan* obj = ObjectWrap::Unwrap<Komachan>(info.This());
    obj->game.game_initialize();
    info.GetReturnValue().SetNull();
  }

  static NAN_METHOD(Move) {
    Komachan* obj = ObjectWrap::Unwrap<Komachan>(info.This());

    if (info.Length() > 1 || info.Length() == 0) {
      Nan::ThrowTypeError("Wrong number of arguments");
    }

    if (!info[0]->IsObject()) {
      Nan::ThrowTypeError("Wrong arguments");
    }

    v8::Local<v8::Object> move = Nan::To<v8::Object>(info[0]).ToLocalChecked();

    if (!Nan::Has(move, Nan::New<v8::String>("from").ToLocalChecked()).FromJust()  ||
        !Nan::Has(move, Nan::New<v8::String>("to").ToLocalChecked()).FromJust()    ||
        !Nan::Has(move, Nan::New<v8::String>("piece").ToLocalChecked()).FromJust()     ) {
      Nan::ThrowTypeError("Wrong Object");
    }

    v8::Local<v8::Object> from = Nan::To<v8::Object>(Nan::Get(move, Nan::New<v8::String>("from").ToLocalChecked()).ToLocalChecked()).ToLocalChecked();
    v8::Local<v8::Object> to = Nan::To<v8::Object>(Nan::Get(move, Nan::New<v8::String>("to").ToLocalChecked()).ToLocalChecked()).ToLocalChecked();
    v8::Local<v8::String> piece = Nan::To<v8::String>(Nan::Get(move, Nan::New<v8::String>("piece").ToLocalChecked()).ToLocalChecked()).ToLocalChecked();

    if (!Nan::Has(from, Nan::New<v8::String>("x").ToLocalChecked()).FromJust()    ||
        !Nan::Has(from, Nan::New<v8::String>("y").ToLocalChecked()).FromJust()    ||
        !Nan::Has(to, Nan::New<v8::String>("x").ToLocalChecked()).FromJust()      ||
        !Nan::Has(to, Nan::New<v8::String>("y").ToLocalChecked()).FromJust()        ){
      Nan::ThrowTypeError("Wrong Object");
    }

    double fromX = Nan::To<double>(Nan::Get(from, Nan::New<v8::String>("x").ToLocalChecked()).ToLocalChecked()).FromJust();
    double fromY = Nan::To<double>(Nan::Get(from, Nan::New<v8::String>("y").ToLocalChecked()).ToLocalChecked()).FromJust();
    double toX = Nan::To<double>(Nan::Get(to, Nan::New<v8::String>("x").ToLocalChecked()).ToLocalChecked()).FromJust();
    double toY = Nan::To<double>(Nan::Get(to, Nan::New<v8::String>("y").ToLocalChecked()).ToLocalChecked()).FromJust();

    Nan::Utf8String piece_(piece);

    double color = 0;
    double promote = 0;
    std::string cap;
    double isAttack;

    if (! obj->game.move(&fromX, &fromY, &toX, &toY, *piece_, &color, &promote, &cap, &isAttack) ){
      info.GetReturnValue().SetUndefined();
      return;
    }

    Nan::Set( move,
              Nan::New<v8::String>("color").ToLocalChecked(),
              Nan::New<v8::Number>((double)(color))
    );
    Nan::Set( move,
              Nan::New<v8::String>("promote").ToLocalChecked(),
              Nan::New<v8::Boolean>(promote)
    );
    Nan::Set( move,
              Nan::New<v8::String>("capture").ToLocalChecked(),
              Nan::New<v8::String>(cap).ToLocalChecked()
    );
    Nan::Set( move,
              Nan::New<v8::String>("isAttack").ToLocalChecked(),
              Nan::New<v8::Boolean>(isAttack)
    );
    info.GetReturnValue().Set(move);
  }

  static NAN_METHOD(Search) {
    Komachan* obj = ObjectWrap::Unwrap<Komachan>(info.This());
    double fromX, fromY, toX, toY, color, promote, isAttack;
    std::string piece, capture;
    if (obj->game.search(&fromX, &fromY, &toX, &toY, &piece, &color, &promote, &capture, &isAttack) < 0 ) {
      info.GetReturnValue().SetUndefined();
      return;
    }
    v8::Local<v8::Object> move = Nan::New<v8::Object>();
    v8::Local<v8::Object> from = Nan::New<v8::Object>();
    v8::Local<v8::Object> to = Nan::New<v8::Object>();

    Nan::Set(
      from,
      Nan::New<v8::String>("x").ToLocalChecked(),
      Nan::New<v8::Number>(fromX)
    );

    Nan::Set(
      from,
      Nan::New<v8::String>("y").ToLocalChecked(),
      Nan::New<v8::Number>(fromY)
    );

    Nan::Set(
      to,
      Nan::New<v8::String>("x").ToLocalChecked(),
      Nan::New<v8::Number>(toX)
    );

    Nan::Set(
      to,
      Nan::New<v8::String>("y").ToLocalChecked(),
      Nan::New<v8::Number>(toY)
    );

    Nan::Set(
      move,
      Nan::New<v8::String>("from").ToLocalChecked(),
      from
    );

    Nan::Set(
      move,
      Nan::New<v8::String>("to").ToLocalChecked(),
      to
    );

    Nan::Set(
      move,
      Nan::New<v8::String>("piece").ToLocalChecked(),
      Nan::New<v8::String>(piece).ToLocalChecked()
    );

    Nan::Set(
      move,
      Nan::New<v8::String>("promote").ToLocalChecked(),
      Nan::New<v8::Boolean>(promote)
    );

    Nan::Set(
      move,
      Nan::New<v8::String>("capture").ToLocalChecked(),
      Nan::New<v8::String>(capture).ToLocalChecked()
    );

    Nan::Set(
      move,
      Nan::New<v8::String>("isAttack").ToLocalChecked(),
      Nan::New<v8::Boolean>(isAttack)
    );
    info.GetReturnValue().Set(move);
  }

  static NAN_METHOD(Legal) {
    Komachan* obj = ObjectWrap::Unwrap<Komachan>(info.This());
    unsigned int lmoves[ SIZE_LEGALMOVES ];
    double fromX, fromY, toX, toY;
    std::string piece;
    int nmove = obj->game.get_board()->gen_legalmoves(lmoves);
    v8::Local<v8::Array> lmovesArray = Nan::New<v8::Array>();
    for ( int i=0; i<nmove; i++) {
      obj->game.legal(lmoves[i], &fromX, &fromY, &toX, &toY, &piece);
      v8::Local<v8::Object> lmove = Nan::New<v8::Object>();
      v8::Local<v8::Object> from = Nan::New<v8::Object>();
      v8::Local<v8::Object> to = Nan::New<v8::Object>();
      Nan::Set(from, Nan::New<v8::String>("x").ToLocalChecked(), Nan::New<v8::Number>(fromX));
      Nan::Set(from, Nan::New<v8::String>("y").ToLocalChecked(), Nan::New<v8::Number>(fromY));
      Nan::Set(to, Nan::New<v8::String>("x").ToLocalChecked(), Nan::New<v8::Number>(toX));
      Nan::Set(to, Nan::New<v8::String>("y").ToLocalChecked(), Nan::New<v8::Number>(toY));
      Nan::Set(lmove, Nan::New<v8::String>("from").ToLocalChecked(), from);
      Nan::Set(lmove, Nan::New<v8::String>("to").ToLocalChecked(), to);
      Nan::Set(lmove, Nan::New<v8::String>("piece").ToLocalChecked(), Nan::New<v8::String>(piece).ToLocalChecked());
      Nan::Set(lmovesArray, Nan::New<v8::Number>(i), lmove);
    }
    info.GetReturnValue().Set(lmovesArray);
  }

  static NAN_METHOD(Print) {
    Komachan* obj = ObjectWrap::Unwrap<Komachan>(info.This());
    obj->io.out_position();
    info.GetReturnValue().SetNull();
  }

  static inline Nan::Persistent<v8::Function> & constructor() {
    static Nan::Persistent<v8::Function> my_constructor;
    return my_constructor;
  }

  Game game;
  Io io;
};

NAN_MODULE_INIT(Init) {
  Komachan::Init(target);
  Nan::Set(target,
    Nan::New<v8::String>("newKomachan").ToLocalChecked(),
    Nan::GetFunction(
      Nan::New<v8::FunctionTemplate>(Komachan::NewInstance)).ToLocalChecked()
  );
}

NODE_MODULE(komachan, Init)

}  // namespace koma
