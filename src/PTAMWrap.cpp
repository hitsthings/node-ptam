#include "PTAMWrap.h"

using namespace v8;

PTAMWrap::PTAMWrap() {};
PTAMWrap::~PTAMWrap() {};

Persistent<Function> PTAMWrap::constructor;

void PTAMWrap::Init() {
	// Prepare constructor template
	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	tpl->SetClassName(String::NewSymbol("PTAM"));
	tpl->InstanceTemplate()->SetInternalFieldCount(0);
	// Prototype
	tpl->PrototypeTemplate()->Set(String::NewSymbol("addFrame"), FunctionTemplate::New(AddFrame)->GetFunction());
	tpl->PrototypeTemplate()->Set(String::NewSymbol("on"), FunctionTemplate::New(On)->GetFunction());

	constructor = Persistent<Function>::New(tpl->GetFunction());

	//Local<Value> size = Integer::NewFromUnsigned(256);
	//Local<Object> array = uint32_array_constructor->NewInstance(1, &size);
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

Handle<Value> PTAMWrap::NewInstance(const Arguments& args) {
	HandleScope scope;

	const unsigned argc = 1;
	Handle<Value> argv[argc] = { args[0] };
	Local<Object> instance = constructor->NewInstance(argc, argv);

	return scope.Close(instance);
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
