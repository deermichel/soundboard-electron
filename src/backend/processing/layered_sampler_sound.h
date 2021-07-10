#ifndef PROCESSING_LAYERED_SAMPLER_SOUND_H
#define PROCESSING_LAYERED_SAMPLER_SOUND_H

#include <juce_audio_formats/juce_audio_formats.h>

namespace soundboard {
namespace processing {

// advanced version of the base sampler sound supporting velocity layers
class LayeredSamplerSound : public juce::SamplerSound {
public:
    // construct sampler sound
    LayeredSamplerSound(const juce::String &name, juce::AudioFormatReader &source, const juce::BigInteger &midiNotes, const juce::BigInteger &velocities,
        int midiNoteForNormalPitch, double attackTimeSecs, double releaseTimeSecs, double maxSampleLengthSeconds);

    // return true if this sound should be played when a given midi note is pressed
    bool appliesToNoteWithVelocity(int midiNoteNumber, int velocity);

private:
    // velocity range
    const juce::BigInteger mVelocities;
};

} // namespace processing
} // namespace soundboard

#endif // PROCESSING_LAYERED_SAMPLER_SOUND_H
