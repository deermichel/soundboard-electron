#include <napi.h>
#include "audio_engine.h"

void Initialize(const Napi::CallbackInfo &info) {
    soundboard::backend::AudioEngine::instance().initialize();
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "initialize"), Napi::Function::New(env, Initialize));
    return exports;
}

NODE_API_MODULE(backend, Init)
