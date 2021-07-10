#ifndef PROCESSING_LAYERED_SAMPLER_H
#define PROCESSING_LAYERED_SAMPLER_H

#include <juce_audio_basics/juce_audio_basics.h>
#include "layered_sampler_sound.h"

namespace soundboard {
namespace processing {

// advanced version of the base sampler supporting multiple velocity layers and round-robin banks
class LayeredSampler : public juce::Synthesiser {
public:
    // construct sampler
    LayeredSampler() {}

    // add sound bank
    void addSoundBank();

    // add sound to bank
    void addSoundToBank(const juce::ReferenceCountedObjectPtr<LayeredSamplerSound> &sound, unsigned int bank);

    // clear sound banks
    void clearSoundBanks();

    // --- overrides ---

    // handle note on event
    void noteOn(int midiChannel, int midiNoteNumber, float velocity) override;

private:
    // sound banks
    std::vector<juce::ReferenceCountedArray<LayeredSamplerSound>> mSoundBanks;

    // next sound bank iterator
    unsigned int mNextSoundBank = 0;
};

} // namespace processing
} // namespace soundboard

#endif // PROCESSING_LAYERED_SAMPLER_H
