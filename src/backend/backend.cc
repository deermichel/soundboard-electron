#include <napi.h>
#include "audio_engine.h"

// add audio unit (returns unique ref)
Napi::Value AddAudioUnit(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();

    // validate arguments
    if (info.Length() < 1) throw Napi::TypeError::New(env, "invalid number of arguments");
    if (!info[0].IsString()) throw Napi::TypeError::New(env, "first argument is not a string");
    std::string id = info[0].As<Napi::String>().Utf8Value();

    // add audio unit
    try {
        unsigned int ref = soundboard::AudioEngine::instance().addAudioUnit(id);
        return Napi::Number::New(env, ref);
    } catch (const std::exception &e) {
        throw Napi::Error::New(env, e.what());
    }
}

// return parameter values mapped by audio units refs
Napi::Value GetParameterValues(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    // TODO: refactor using clearer variable names (js vs c++ types)
    Napi::Object valuesByRefs = Napi::Object::New(env);
    for (auto const &[ref, paramValues] : soundboard::AudioEngine::instance().getParameterValues()) {
        Napi::Object values = Napi::Object::New(env); // TODO: duplicated code from SetParameterValue()
        for (const auto &paramValue : paramValues) {
            values.Set(Napi::String::New(env, paramValue.id), paramValue.CreateObject(env));
        }
        valuesByRefs.Set(Napi::String::New(env, std::to_string(ref)), values);
    }
    return valuesByRefs;
}

// remove audio unit
void RemoveAudioUnit(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();

    // validate arguments
    if (info.Length() < 1) throw Napi::TypeError::New(env, "invalid number of arguments");
    if (!info[0].IsNumber()) throw Napi::TypeError::New(env, "first argument is not a number");
    unsigned int ref = info[0].As<Napi::Number>().Uint32Value();

    // remove audio unit
    try {
        soundboard::AudioEngine::instance().removeAudioUnit(ref);
    } catch (const std::exception &e) {
        throw Napi::Error::New(env, e.what());
    }
}

// set audio unit parameter value
Napi::Value SetParameterValue(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();

    // validate arguments
    if (info.Length() < 3) throw Napi::TypeError::New(env, "invalid number of arguments");
    if (!info[0].IsNumber()) throw Napi::TypeError::New(env, "first argument is not a number");
    if (!info[1].IsString()) throw Napi::TypeError::New(env, "second argument is not a string");
    if (!info[2].IsNumber()) throw Napi::TypeError::New(env, "third argument is not a number");
    unsigned int ref = info[0].As<Napi::Number>().Uint32Value();
    std::string paramId = info[1].As<Napi::String>().Utf8Value();
    float value = info[2].As<Napi::Number>().FloatValue();

    // set parameter value
    try {
        auto paramValues = soundboard::AudioEngine::instance().setParameterValue(ref, paramId, value);

        // return updated parameter values
        Napi::Object values = Napi::Object::New(env);
        for (const auto &paramValue : paramValues) {
            values.Set(Napi::String::New(env, paramValue.id), paramValue.CreateObject(env));
        }
        return values;
    } catch (const std::exception &e) {
        throw Napi::Error::New(env, e.what());
    }
}

// update audio graph connections via session
void UpdateGraph(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();

    // validate arguments
    if (info.Length() < 1) throw Napi::TypeError::New(env, "invalid number of arguments");
    if (!info[0].IsObject()) throw Napi::TypeError::New(env, "first argument is not an object");
    soundboard::model::Session session(info[0].As<Napi::Object>());

    // update graph
    try {
        soundboard::AudioEngine::instance().updateGraph(session);
    } catch (const std::exception &e) {
        throw Napi::Error::New(env, e.what());
    }
}

// init js interface
Napi::Object Init(Napi::Env env, Napi::Object exports) {
    // init engine
    soundboard::AudioEngine::instance().initialize();

    // exports
    exports.Set(Napi::String::New(env, "addAudioUnit"), Napi::Function::New(env, AddAudioUnit));
    exports.Set(Napi::String::New(env, "getParameterValues"), Napi::Function::New(env, GetParameterValues));
    exports.Set(Napi::String::New(env, "removeAudioUnit"), Napi::Function::New(env, RemoveAudioUnit));
    exports.Set(Napi::String::New(env, "setParameterValue"), Napi::Function::New(env, SetParameterValue));
    exports.Set(Napi::String::New(env, "updateGraph"), Napi::Function::New(env, UpdateGraph));
    return exports;
}

NODE_API_MODULE(backend, Init)
