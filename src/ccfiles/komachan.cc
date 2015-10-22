#include <node.h>
#include "koma.h"

namespace koma {

using v8::Local;
using v8::Object;

void InitAll(Local<Object> exports) {
  Koma::Init(exports);
}

NODE_MODULE(komachan, InitAll)

}  // namespace koma
