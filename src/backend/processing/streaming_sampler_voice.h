#ifndef PROCESSING_STREAMING_SAMPLER_VOICE_H
#define PROCESSING_STREAMING_SAMPLER_VOICE_H

#include <juce_audio_basics/juce_audio_basics.h>
#include "streaming_sample_loader.h"

namespace soundboard {
namespace processing {

// synthesizer sampler voice to stream data from a StreamingSamplerSound
class StreamingSamplerVoice : public juce::SynthesiserVoice {
public:
    // construct sampler voice using given thread pool
    StreamingSamplerVoice(juce::ThreadPool *backgroundThreadPool);

    // --- overrides ---

    // return true if this voice object is capable of playing the given sound (always)
    bool canPlaySound(juce::SynthesiserSound *sound) override { return true; }

    // midi controller moved
    void controllerMoved (int controllerNumber, int newControllerValue) override {}

    // pitch wheel moved
    void pitchWheelMoved (int newPitchWheelValue) override {}

    // renders the next block of data for this voice
    void renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples) override;

    // start streaming the sound
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition) override;

    // stop the note
    void stopNote(float velocity, bool allowTailOff) override;

private:
    // sample loader
    StreamingSampleLoader mLoader;
};

} // namespace processing
} // namespace soundboard

#endif // PROCESSING_STREAMING_SAMPLER_VOICE_H
