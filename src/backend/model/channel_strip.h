#ifndef MODEL_CHANNEL_STRIP_H
#define MODEL_CHANNEL_STRIP_H

#ifndef STANDALONE
#include <napi.h>
#endif
#include "audio_unit.h"

namespace soundboard {
namespace model {

// channel strip containing audio units
struct ChannelStrip {
    // audio units
    std::vector<AudioUnit> audioUnits;

#ifndef STANDALONE
    // construct channel strip from js object
    ChannelStrip(const Napi::Object &object) {
        auto audioUnitsObject = object.Get("audioUnits").As<Napi::Array>();
        for (unsigned int i = 0; i < audioUnitsObject.Length(); i++) {
            audioUnits.emplace_back(audioUnitsObject.Get(i).As<Napi::Object>());
        }
    }
#endif
};

} // namespace model
} // namespace soundboard

#endif // MODEL_CHANNEL_STRIP_H
