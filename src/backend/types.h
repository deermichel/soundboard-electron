#ifndef TYPES_H
#define TYPES_H

namespace soundboard {

// audio unit
struct AudioUnit {
    unsigned int ref;
};

// channel strip
struct ChannelStrip {
    std::vector<AudioUnit> audioUnits;
};

// session
struct Session {
    std::string name;
    std::vector<ChannelStrip> channelStrips;
};

} // namespace soundboard

#endif // TYPES_H
