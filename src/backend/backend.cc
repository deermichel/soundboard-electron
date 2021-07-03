#include <napi.h>
#include "audio_engine.h"
#include "types.h"

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

// update audio graph connections via session
Napi::Value UpdateGraph(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();

    // validate arguments
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "invalid number of arguments").ThrowAsJavaScriptException();
        return env.Null();
    }
    if (!info[0].IsObject()) {
        Napi::TypeError::New(env, "first argument is not an object").ThrowAsJavaScriptException();
        return env.Null();
    }

    // parse name
    auto _session = info[0].As<Napi::Object>();
    soundboard::Session session;
    session.name = _session.Get("name").As<Napi::String>().Utf8Value();

    // parse channel strips
    auto _channelStrips = _session.Get("channelStrips").As<Napi::Array>();
    for (unsigned int c = 0; c < _channelStrips.Length(); c++) {
        auto _channelStrip = _channelStrips.Get(c).As<Napi::Object>();
        fprintf(stderr, "%d channel strip\n", c);

        // soundboard::ChannelStrip& channelStrip = session.channelStrips.emplace_back();

        // parse audio units
        auto _audioUnits = _channelStrip.Get("audioUnits").As<Napi::Array>();
        for (unsigned int a = 0; a < _audioUnits.Length(); a++) {
            auto _audioUnit = _audioUnits.Get(a).As<Napi::Object>();
            auto ref = _audioUnit.Get("ref").As<Napi::Number>().Uint32Value();
            fprintf(stderr, "%d audio unit - ref: %d\n", a, ref);
        }
    }

    // update graph
    soundboard::AudioEngine::instance().updateGraph(session);
}

// init js interface
Napi::Object Init(Napi::Env env, Napi::Object exports) {
    // init engine
    soundboard::AudioEngine::instance().initialize();

    // exports
    exports.Set(Napi::String::New(env, "addAudioUnit"), Napi::Function::New(env, AddAudioUnit));
    exports.Set(Napi::String::New(env, "updateGraph"), Napi::Function::New(env, UpdateGraph));
    return exports;
}

NODE_API_MODULE(backend, Init)
