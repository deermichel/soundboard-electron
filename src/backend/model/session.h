#ifndef MODEL_SESSION_H
#define MODEL_SESSION_H

#ifndef STANDALONE
#include <napi.h>
#endif
#include "channel_strip.h"

namespace soundboard {
namespace model {

// session containing a single project (channel strips, audio units, mappings, etc.)
struct Session {
    // channel strips
    std::vector<ChannelStrip> channelStrips;

    // session name
    std::string name;

#ifndef STANDALONE
    // construct session from js object
    Session(const Napi::Object &object) {
        name = object.Get("name").As<Napi::String>().Utf8Value();
        auto channelStripsObject = object.Get("channelStrips").As<Napi::Array>();
        for (unsigned int i = 0; i < channelStripsObject.Length(); i++) {
            channelStrips.emplace_back(channelStripsObject.Get(i).As<Napi::Object>());
        }
    }
#endif
};

} // namespace model
} // namespace soundboard

#endif // MODEL_SESSION_H
