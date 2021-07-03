#ifndef MODEL_AUDIO_UNIT_H
#define MODEL_AUDIO_UNIT_H

#include <napi.h>

namespace soundboard {
namespace model {

// audio unit is a single instance of an effect, instrument, internal
struct AudioUnit {
    // unique processor reference
    unsigned int ref;

    // construct audio unit from js object
    AudioUnit(const Napi::Object &object) {
        ref = object.Get("ref").As<Napi::Number>().Uint32Value();
    }
};

} // namespace model
} // namespace soundboard

#endif // MODEL_AUDIO_UNIT_H
