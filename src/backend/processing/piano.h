#ifndef PROCESSING_PIANO_H
#define PROCESSING_PIANO_H

#include <juce_audio_basics/juce_audio_basics.h>
#include "base_processor.h"

namespace soundboard {
namespace processing {

// [instrument] piano - piano sampler
class Piano : public BaseProcessor {
public:
    // audio unit id
    static constexpr const char *ID = "piano";

    // construct piano
    Piano();

    // --- overrides ---

    // called before playback starts, to let the processor prepare itself
    void prepareToPlay(double sampleRate, int bufferSize) override;

    // renders the next block
    void processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages) override;

private:
    // synthesizer
    juce::Synthesiser mSynth;

    // thread pool for disk streaming
    std::unique_ptr<juce::ThreadPool> mThreadPool;
};

} // namespace processing
} // namespace soundboard

#endif // PROCESSING_PIANO_H
