#ifndef PROCESSING_MIXER_H
#define PROCESSING_MIXER_H

#include <juce_dsp/juce_dsp.h>
#include "base_processor.h"

namespace soundboard {
namespace processing {

// [internal] mixer - provides gain, balance controls at the of each channel strip
class Mixer : public BaseProcessor {
public:
    // audio unit id
    static constexpr const char *ID = "mixer";

    // construct mixer
    Mixer();

    // --- overrides ---

    // called before playback starts, to let the processor prepare itself
    void prepareToPlay(double sampleRate, int bufferSize) override;

    // renders the next block
    void processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages) override;

    // should reset any playing voices
    void reset() override;

private:
    // gain dsp
    juce::dsp::Gain<float> mGain;

    // panner dsp
    juce::dsp::Panner<float> mPanner;
};

} // namespace processing
} // namespace soundboard

#endif // PROCESSING_MIXER_H
