#include <napi.h>
#include "audio_engine.h"

// add audio unit (returns unique ref)
Napi::Value AddAudioUnit(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();

    // validate arguments
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "invalid number of arguments").ThrowAsJavaScriptException();
        return env.Null();
    }
    if (!info[0].IsString()) {
        Napi::TypeError::New(env, "first argument is not a string").ThrowAsJavaScriptException();
        return env.Null();
    }
    std::string id = info[0].As<Napi::String>().Utf8Value();

    // add audio unit
    try {
        unsigned int ref = soundboard::AudioEngine::instance().addAudioUnit(id);
        return Napi::Number::New(env, ref);
    } catch (const std::exception &e) {
        throw Napi::Error::New(env, e.what());
    }
}

// init module
Napi::Object Init(Napi::Env env, Napi::Object exports) {
    // init engine
    soundboard::AudioEngine::instance().initialize();

    // exports
    exports.Set(Napi::String::New(env, "addAudioUnit"), Napi::Function::New(env, AddAudioUnit));
    return exports;
}

NODE_API_MODULE(backend, Init)
