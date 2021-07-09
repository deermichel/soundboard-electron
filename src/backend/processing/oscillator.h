#ifndef PROCESSING_OSCILLATOR_H
#define PROCESSING_OSCILLATOR_H

#include <juce_dsp/juce_dsp.h>
#include "base_processor.h"

namespace soundboard {
namespace processing {

// [instrument] oscillator - basic midi-controlled oscillator for testing purposes
class Oscillator : public BaseProcessor {
public:
    // audio unit id
    static constexpr const char *ID = "oscillator";

    // construct oscillator
    Oscillator();

    // --- overrides ---

    // called before playback starts, to let the processor prepare itself
    void prepareToPlay(double sampleRate, int bufferSize) override;

    // renders the next block
    void processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages) override;

    // should reset any playing voices
    void reset() override;

private:
    // oscillator dsp
    juce::dsp::Oscillator<float> mOscillator;
};

} // namespace processing
} // namespace soundboard

#endif // PROCESSING_OSCILLATOR_H
