#include <node.h>
#include <v8.h>

#include "PTAMWrap.h"

using namespace v8;

Handle<Value> CreatePTAM(const Arguments& args) {
  HandleScope scope;
  return scope.Close(PTAMWrap::NewInstance(args));
}

void init(Handle<Object> target) {
  target->Set(String::NewSymbol("createPTAM"),
      FunctionTemplate::New(CreatePTAM)->GetFunction());
}
NODE_MODULE(ptam, init)