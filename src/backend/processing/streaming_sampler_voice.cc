#include "streaming_sampler.h"
#include "streaming_sampler_voice.h"

namespace soundboard {
namespace processing {

// construct sampler voice using given thread pool
StreamingSamplerVoice::StreamingSamplerVoice(juce::ThreadPool *backgroundThreadPool) :
    mLoader(backgroundThreadPool) {}

// reset voice
void StreamingSamplerVoice::resetVoice() {
    mVoiceUptime = 0.0;
    mUptimeDelta = 0.0;
    clearCurrentNote();
}

// start streaming the sound
void StreamingSamplerVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition) {
    StreamingSamplerSound *streamingSound = dynamic_cast<StreamingSamplerSound*>(sound);
    mLoader.startNote(streamingSound);

    jassert(streamingSound != nullptr);
    streamingSound->wakeSound();

    mVoiceUptime = 0.0;
    mUptimeDelta = juce::jmin(streamingSound->getPitchFactor(midiNoteNumber), (double)MAX_SAMPLER_PITCH);
}

// --- private methods ---

} // namespace processing
} // namespace soundboard
