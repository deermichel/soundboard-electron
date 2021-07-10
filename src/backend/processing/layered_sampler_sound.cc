#include "layered_sampler_sound.h"

namespace soundboard {
namespace processing {

// construct sampler sound
LayeredSamplerSound::LayeredSamplerSound(const juce::String &name, juce::AudioFormatReader &source, const juce::BigInteger &midiNotes, const juce::BigInteger &velocities,
    int midiNoteForNormalPitch, double attackTimeSecs, double releaseTimeSecs, double maxSampleLengthSeconds) :
    SamplerSound(name, source, midiNotes, midiNoteForNormalPitch, attackTimeSecs, releaseTimeSecs, maxSampleLengthSeconds),
    mVelocities(velocities) {}

// return true if this sound should be played when a given midi note is pressed
bool LayeredSamplerSound::appliesToNoteWithVelocity(int midiNoteNumber, int velocity) {
    return appliesToNote(midiNoteNumber) && mVelocities[velocity];
}

} // namespace processing
} // namespace soundboard
