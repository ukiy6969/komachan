// hello.cc
#include <node.h>
extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "header.h"
}

int is_log_output; // is output logfile flag
int server_mode;  // server_mode

namespace demo {

using v8::Exception;
using v8::FunctionCallbackInfo;
using v8::HandleScope;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

void Start(const FunctionCallbackInfo<Value>& args){
  Isolate* isolate = args.GetIsolate();
  if( starting_initialize() ){
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Cant starting_initialize")));
    return;
  }
  game_initialize();
  args.GetReturnValue().Set(String::NewFromUtf8(isolate, "ok"));
}

void Search(const FunctionCallbackInfo<Value>& args){
  char str[8];
  Isolate* isolate = args.GetIsolate();
  search_root();
  out_move_string(str);
  args.GetReturnValue().Set(String::NewFromUtf8(isolate, str));
}

void Move(const FunctionCallbackInfo<Value>& args){
  Isolate* isolate = args.GetIsolate();
  char *c_arg, str1[8];
  if (!args[0]->IsString()){
    isolate->ThrowException(Exception::TypeError(
          String::NewFromUtf8(isolate, "Wrong arguments")));
      return;
  }
  String::Utf8Value str(args[0]->ToString());
  c_arg = *str;
  if( manual_move_str(c_arg)) {
    isolate->ThrowException(Exception::TypeError(
          String::NewFromUtf8(isolate, "Invalid move")));
      return;
  }
  out_move_string(str1);
  args.GetReturnValue().Set(String::NewFromUtf8(isolate, str1));
}

void Method(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  args.GetReturnValue().Set(String::NewFromUtf8(isolate, "world"));
}

void init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "hello", Method);
  NODE_SET_METHOD(exports, "start", Start);
  NODE_SET_METHOD(exports, "search", Search);
  NODE_SET_METHOD(exports, "move", Move);
}

NODE_MODULE(addon, init)

}  // namespace demo
