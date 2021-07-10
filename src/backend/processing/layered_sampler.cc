#include "layered_sampler.h"

namespace soundboard {
namespace processing {

// add sound bank (returns index)
unsigned int LayeredSampler::addSoundBank() {
    const juce::ScopedLock sl(lock);
    mSoundBanks.emplace_back();
    return mSoundBanks.size() - 1;
}

// add sound to bank
void LayeredSampler::addSoundToBank(const juce::ReferenceCountedObjectPtr<LayeredSamplerSound> &sound, unsigned int bank) {
    const juce::ScopedLock sl(lock);
    mSoundBanks.at(bank).add(sound);
}

// clear sound banks
void LayeredSampler::clearSoundBanks() {
    const juce::ScopedLock sl(lock);
    mSoundBanks.clear();
    mNextSoundBank = 0;
}

// handle note on event
void LayeredSampler::noteOn(int midiChannel, int midiNoteNumber, float velocity) {
    const juce::ScopedLock sl(lock);
    if (mSoundBanks.empty()) return;

    // iterate sounds in current bank
    auto soundBank = mSoundBanks[mNextSoundBank];
    for (int i = 0; i < soundBank.size(); i++) {
        LayeredSamplerSound *sound = static_cast<LayeredSamplerSound*>(soundBank.getUnchecked(i).get());
        if (sound->appliesToNoteWithVelocity(midiNoteNumber, velocity) && sound->appliesToChannel(midiChannel)) {
            // TODO: that's probably not want we want
            // stop note if still ringing
            // for (auto *voice : voices) {
            //     if (voice->getCurrentlyPlayingNote() == midiNoteNumber && voice->isPlayingChannel(midiChannel)) {
            //         stopVoice(voice, 1.0f, true);
            //     }
            // }

            // start note
            auto *freeVoice = findFreeVoice(sound, midiChannel, midiNoteNumber, isNoteStealingEnabled());
            startVoice(freeVoice, sound, midiChannel, midiNoteNumber, velocity);
        }
    }
    mNextSoundBank = (mNextSoundBank + 1) % mSoundBanks.size();
}

} // namespace processing
} // namespace soundboard
