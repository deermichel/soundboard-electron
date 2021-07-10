#include "layered_sampler_sound.h"

namespace soundboard {
namespace processing {

// construct sampler sound
LayeredSamplerSound::LayeredSamplerSound(const juce::String &name, juce::AudioFormatReader &source, const juce::BigInteger &midiNotes, const juce::Range<float> &velocityRange,
    int midiNoteForNormalPitch, double attackTimeSecs, double releaseTimeSecs, double maxSampleLengthSeconds) :
    SamplerSound(name, source, midiNotes, midiNoteForNormalPitch, attackTimeSecs, releaseTimeSecs, maxSampleLengthSeconds),
    mVelocityRange(velocityRange) {}

// return true if this sound should be played when a given midi note is pressed
bool LayeredSamplerSound::appliesToNoteWithVelocity(int midiNoteNumber, float velocity) {
    return appliesToNote(midiNoteNumber) && mVelocityRange.contains(velocity);
}

} // namespace processing
} // namespace soundboard
