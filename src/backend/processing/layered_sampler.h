#ifndef PROCESSING_LAYERED_SAMPLER_H
#define PROCESSING_LAYERED_SAMPLER_H

#include <juce_audio_basics/juce_audio_basics.h>

namespace soundboard {
namespace processing {

// advanced version of the base sampler supporting multiple velocity and round-robin layers
class LayeredSampler : public juce::Synthesiser {
public:
    // construct sampler
    LayeredSampler() {}

    // --- overrides ---

    // handle note on event
    void noteOn(int midiChannel, int midiNoteNumber, float velocity) override;

private:
};

} // namespace processing
} // namespace soundboard

#endif // PROCESSING_LAYERED_SAMPLER_H
