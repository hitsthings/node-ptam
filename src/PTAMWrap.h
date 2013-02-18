#ifndef PTAMWRAP_H
#define PTAMWRAP_H

#include <list>

#include <node.h>

#include <cvd/image_convert.h>

#include <gvars3/instances.h>
#include <stdlib.h>
#include "ATANCamera.h"
#include "MapMaker.h"
#include "Tracker.h"
#include "ARDriver.h"
#include "MapViewer.h"

#include <System.h>
typedef System PTAMthread;

using namespace std;

class PTAMWrap : public node::ObjectWrap {
 public:
  static void Init();
  static v8::Handle<v8::Value> NewInstance(const v8::Arguments& args);

 private:
  PTAMWrap();
  ~PTAMWrap();

  static v8::Persistent<v8::Function> constructor;
  static v8::Handle<v8::Value> New(const v8::Arguments& args);

  static v8::Handle<v8::Value> AddFrame(const v8::Arguments& args);
  static v8::Handle<v8::Value> On(const v8::Arguments& args);

  PTAMthread ptam;
  CVD::ImageRef videoSize;
  list<v8::Persistent<v8::Function>> updateHandlers;
};

#endif
