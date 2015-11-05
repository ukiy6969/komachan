#include <nan.h>
#include <iostream>
#include "io.h"

namespace komachan {


class Komachan : public Nan::ObjectWrap {
 public:
  static NAN_MODULE_INIT(Init) {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->InstanceTemplate()->SetInternalFieldCount(8);

    Nan::SetPrototypeMethod(tpl, "start", Start);
    Nan::SetPrototypeMethod(tpl, "move", Move);
    Nan::SetPrototypeMethod(tpl, "search", Search);
    Nan::SetPrototypeMethod(tpl, "legal", Legal);
    Nan::SetPrototypeMethod(tpl, "getTpt", GetTpt);
    Nan::SetPrototypeMethod(tpl, "getZobrist", GetZobrist);
    Nan::SetPrototypeMethod(tpl, "end", End);
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
    if (info[0]->IsObject() ) {
      v8::Local<v8::Object> startObj = Nan::To<v8::Object>(info[0]).ToLocalChecked();
      v8::Local<v8::String> useTptStr = Nan::New<v8::String>("useTpt").ToLocalChecked();
      if (Nan::Has(startObj, useTptStr).FromJust() ) {
        int useTpt = Nan::To<int32_t>(Nan::Get(startObj, useTptStr).ToLocalChecked()).FromJust();
        obj->game.get_search()->useTpt = useTpt;
      }
      v8::Local<v8::String> searchDepthStr = Nan::New<v8::String>("searchDepth").ToLocalChecked();
      if (Nan::Has(startObj, searchDepthStr).FromJust() ) {
        int searchDepth = Nan::To<int32_t>(Nan::Get(startObj, searchDepthStr).ToLocalChecked()).FromJust();
        if (searchDepth > 0){
          obj->game.get_search()->searchDepth = searchDepth;
        }
      }

      v8::Local<v8::String> hashStr = Nan::New<v8::String>("hash").ToLocalChecked();
      if ( Nan::Has(startObj, hashStr).FromJust() ) {
        v8::Local<v8::Object> tpt = Nan::To<v8::Object>(
          Nan::Get(startObj, hashStr).ToLocalChecked()
        ).ToLocalChecked();

        v8::Local<v8::String> pieceInfoRandStr = Nan::New<v8::String>("pieceInfoRand").ToLocalChecked();
        if ( Nan::Has(tpt, pieceInfoRandStr).FromJust() ) {
          v8::Local<v8::Array> pieceInfoRand = v8::Local<v8::Array>::Cast(
            Nan::Get(tpt, pieceInfoRandStr).ToLocalChecked()
          );
          for (unsigned int i = 0; i < pieceInfoRand->Length(); i++) {
            v8::Local<v8::Array> pieceInfoRandI = v8::Local<v8::Array>::Cast(pieceInfoRand->Get(i));
            for (unsigned int j = 0; j < pieceInfoRandI->Length(); j++) {
              v8::Local<v8::String> res = Nan::To<v8::String>(pieceInfoRandI->Get(j)).ToLocalChecked();
              Nan::Utf8String hash(res);
              std::string _hash = *hash;
              obj->game.get_board()->PIECE_INFO_RAND[i][j] = std::stoull(_hash);
            }
          }
        }

        v8::Local<v8::String> handInfoRandStr = Nan::New<v8::String>("handInfoRand").ToLocalChecked();
        if ( Nan::Has(tpt, handInfoRandStr).FromJust() ) {
          v8::Local<v8::Array> handInfoRand = v8::Local<v8::Array>::Cast(
            Nan::Get(tpt, handInfoRandStr).ToLocalChecked()
          );
          for (unsigned int i = 0; i < handInfoRand->Length(); i++) {
            v8::Local<v8::Array> handInfoRandI = v8::Local<v8::Array>::Cast(handInfoRand->Get(i));
            for (unsigned int j = 0; j < handInfoRandI->Length(); j++) {
              v8::Local<v8::Array> handInfoRandIJ = v8::Local<v8::Array>::Cast(handInfoRandI->Get(j));
              for (unsigned int k = 0; k < handInfoRandIJ->Length(); k++){
                v8::Local<v8::String> res = Nan::To<v8::String>(handInfoRandIJ->Get(k)).ToLocalChecked();
                Nan::Utf8String hash(res);
                std::string _hash = *hash;
                obj->game.get_board()->HAND_INFO_RAND[i][j][k] = std::stoull(_hash);
              }
            }
          }
        }

        v8::Local<v8::String> turnRandStr = Nan::New<v8::String>("turnRand").ToLocalChecked();
        if ( Nan::Has(tpt, turnRandStr).FromJust() ) {
          v8::Local<v8::Array> turnRand = v8::Local<v8::Array>::Cast(
            Nan::Get(tpt, turnRandStr).ToLocalChecked()
          );
          for (unsigned int i = 0; i < turnRand->Length(); i++) {
            v8::Local<v8::String> res = Nan::To<v8::String>(turnRand->Get(i)).ToLocalChecked();
            Nan::Utf8String hash(res);
            std::string _hash = *hash;
            obj->game.get_board()->TURN_RAND[i] = std::stoull(_hash);
          }
        }
      }

    }

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
    double searchTime = -1;
    if ( (searchTime = obj->game.search(&fromX, &fromY, &toX, &toY, &piece, &color, &promote, &capture, &isAttack)) < 0 ) {
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

    Nan::Set(
      move,
      Nan::New<v8::String>("searchTime").ToLocalChecked(),
      Nan::New<v8::Number>(searchTime)
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

  static NAN_METHOD(End) {
    Komachan* obj = ObjectWrap::Unwrap<Komachan>(info.This());
    obj->game.game_finalize();
    info.GetReturnValue().SetNull();
  }

  static NAN_METHOD(GetTpt) {
    Komachan* obj = ObjectWrap::Unwrap<Komachan>(info.This());
    //obj->game.get_board()->print_tpt();
    auto game_tpt = obj->game.get_board()->tpt;
    auto itr = game_tpt.begin();
    v8::Local<v8::Array> tpt = Nan::New<v8::Array>();
    int idx = 0;
    while ( itr != game_tpt.end() ) {
      unsigned long long hash = (*itr).first;
      std::string hash_str = std::to_string(hash);
      tpt_v tpt_evals = (*itr).second;
      v8::Local<v8::Object> tptValue = Nan::New<v8::Object>();
      v8::Local<v8::Object> evals = Nan::New<v8::Object>();

      Nan::Set(tptValue, Nan::New<v8::String>("hash").ToLocalChecked(), Nan::New<v8::String>(hash_str).ToLocalChecked());

      Nan::Set(evals, Nan::New<v8::String>("depth").ToLocalChecked(), Nan::New<v8::Number>(tpt_evals.depth));
      Nan::Set(evals, Nan::New<v8::String>("eval").ToLocalChecked(), Nan::New<v8::Number>(tpt_evals.eval));

      Nan::Set(tptValue, Nan::New<v8::String>("evals").ToLocalChecked(), evals);

      Nan::Set(tpt, Nan::New<v8::Number>(idx), tptValue);
      idx++;
      itr++;
    }
    info.GetReturnValue().Set(tpt);
  }

  static NAN_METHOD(GetZobrist) {
    Komachan* obj = ObjectWrap::Unwrap<Komachan>(info.This());
    v8::Local<v8::Object> hash = Nan::New<v8::Object>();
    v8::Local<v8::Object> pieceInfoRand = Nan::New<v8::Array>();
    v8::Local<v8::Object> handInfoRand = Nan::New<v8::Array>();
    v8::Local<v8::Object> turnRand = Nan::New<v8::Array>();

    for(int i = 0; i < 32; i++) {
      v8::Local<v8::Array> pieceInfoRandJ = Nan::New<v8::Array>();
      for(int j = 0; j < 32; j++) {
        v8::Local<v8::String> hash = Nan::New<v8::String>(
          std::to_string(obj->game.get_board()->PIECE_INFO_RAND[i][j])
        ).ToLocalChecked();
        Nan::Set(pieceInfoRandJ, Nan::New<v8::Number>(j), hash);
      }
      Nan::Set(pieceInfoRand, Nan::New<v8::Number>(i), pieceInfoRandJ);
    }
    Nan::Set(hash, Nan::New<v8::String>("pieceInfoRand").ToLocalChecked(), pieceInfoRand);

    for(int i = 0; i < 2; i++) {
      v8::Local<v8::Array> handInfoRandJ = Nan::New<v8::Array>();
      for(int j = 0; j < 4; j++) {
        v8::Local<v8::Array> handInfoRandK = Nan::New<v8::Array>();
        for (int k = 0; k < 8; k++) {
          v8::Local<v8::String> hash = Nan::New<v8::String>(
            std::to_string(obj->game.get_board()->HAND_INFO_RAND[i][j][k])
          ).ToLocalChecked();
          Nan::Set(handInfoRandK, Nan::New<v8::Number>(k), hash);
        }
        Nan::Set(handInfoRandJ, Nan::New<v8::Number>(j), handInfoRandK);
      }
      Nan::Set(handInfoRand, Nan::New<v8::Number>(i), handInfoRandJ);
    }
    Nan::Set(hash, Nan::New<v8::String>("handInfoRand").ToLocalChecked(), handInfoRand);

    for( int i = 0; i < 2; i++) {
      v8::Local<v8::String> hash = Nan::New<v8::String>(
        std::to_string(obj->game.get_board()->TURN_RAND[i])
      ).ToLocalChecked();
      Nan::Set(turnRand, Nan::New<v8::Number>(i), hash);
    }
    Nan::Set(hash, Nan::New<v8::String>("turnRand").ToLocalChecked(), turnRand);

    info.GetReturnValue().Set(hash);
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
