#include "PTAMWrap.h"

using namespace v8;

PTAMWrap::PTAMWrap() {};
PTAMWrap::~PTAMWrap() {};

Persistent<FunctionTemplate> PTAMWrap::constructor;

void PTAMWrap::Init(Handle<Object> target) {
	HandleScope scope;

	// Prepare constructor template
	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
    Local<String> name = String::NewSymbol("PTAM");

	constructor = Persistent<FunctionTemplate>::New(tpl);
	constructor->InstanceTemplate()->SetInternalFieldCount(1);
	tpl->SetClassName(name);

	// Add all prototype methods, getters and setters here.
    NODE_SET_PROTOTYPE_METHOD(constructor, "addFrame", AddFrame);
    NODE_SET_PROTOTYPE_METHOD(constructor, "on", On);

    // This has to be last, otherwise the properties won't show up on the
    // object in JavaScript.
    target->Set(name, constructor->GetFunction());
}

Handle<Value> PTAMWrap::New(const Arguments& args) {
	HandleScope scope;

	PTAMWrap* obj = new PTAMWrap();
	obj->Wrap(args.This());

	int width;
	int height;
	if (args[0]->IsObject()) {
		Local<Object> options = args[0]->ToObject();
		width = options->Get(String::New("width"))->Int32Value();
		height = options->Get(String::New("height"))->Int32Value();
	} else {
		width = 640;
		height = 480;
	}

	obj->videoSize = CVD::ImageRef(width, height);
	obj->ptam.videoSize = obj->videoSize;

	obj->ptam.setup();

	return args.This();
}

Handle<Value> PTAMWrap::AddFrame(const Arguments& args) {
	HandleScope scope;

	PTAMWrap* self = ObjectWrap::Unwrap<PTAMWrap>(args.This());

	if (args.Length() < 1) {
		ThrowException(Exception::TypeError(String::New("AddFrame expects a Uint8Array of RGB or RGBA values and a timestamp (ms) as arguments.")));
		return scope.Close(Undefined());
	}

	Local<Object> maybeUint8Array = args[0]->ToObject();
	if (maybeUint8Array->GetIndexedPropertiesExternalArrayDataType() != kExternalUnsignedShortArray) {
		ThrowException(Exception::TypeError(String::New("AddFrame expects a Uint8Array of RGB values as the first argument.")));
		return scope.Close(Undefined());
	}
	int len = maybeUint8Array->GetIndexedPropertiesExternalArrayDataLength();
	int area = self->videoSize.area();
	unsigned short* rgbData;
	if (len == area * 3) {
		rgbData = static_cast<unsigned short*>(maybeUint8Array->GetIndexedPropertiesExternalArrayData());
	} else if (len == area * 4) {
		unsigned short* rgbaData = static_cast<unsigned short*>(maybeUint8Array->GetIndexedPropertiesExternalArrayData());
		rgbData = new unsigned short[area * 3];
		for(int i = 0; i < area * 4; i++) {
			if (i % 4 == 3) continue;
			rgbData[(i * 3 / 4)] = rgbaData[i];
		}
	} else {
		char errBuffer[100];
		sprintf_s(errBuffer, "Frame size didn't match video size. Expected: %d (RGB) or %d (RGBA), actual: %d", 3*area, 4*area, len);
		ThrowException(Exception::TypeError(String::New(errBuffer)));
		return scope.Close(Undefined());
	}


	/* timestamp is unused for now
	if (!args[1]->IsNumber()) {
		ThrowException(Exception::TypeError(String::New("AddFrame expects a timestamp (ms) as the second argument.")));
		return scope.Close(Undefined());
	}
	double timestamp = args[1]->NumberValue();
	*/

	memcpy(self->ptam.mimFrameRGB.data(), rgbData, len);
	CVD::convert_image(self->ptam.mimFrameRGB, self->ptam.mimFrameBW);
	self->ptam.update();

	return scope.Close(Undefined());
}


Handle<Value> PTAMWrap::On(const Arguments& args) {
	HandleScope scope;
	
	if (args.Length() < 2) {
		ThrowException(Exception::TypeError(String::New("PTAM.on expects a String and Function as arguments.")));
		return scope.Close(Undefined());
	}
	
    v8::String::Utf8Value param1(args[0]->ToString());
    std::string eventName = std::string(*param1);

	if (!strcmp(eventName.c_str(), "update") && args[1]->IsFunction()) {
		PTAMWrap* self = ObjectWrap::Unwrap<PTAMWrap>(args.This());
		Persistent<Function> callback = Persistent<Function>::New(Local<Function>::Cast(args[1]));
		self->updateHandlers.push_back(callback);
	}

	return scope.Close(args.This());
}

void init(Handle<Object> target) {
	PTAMWrap::Init(target);
}
NODE_MODULE(ptam, init)