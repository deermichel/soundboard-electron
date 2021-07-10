#include "layered_sampler.h"
#include "layered_sampler_sound.h"

namespace soundboard {
namespace processing {

// handle note on event
void LayeredSampler::noteOn(int midiChannel, int midiNoteNumber, float velocity) {
    const juce::ScopedLock sl(lock);

    // iterate sounds
    for (int i = 0; i < sounds.size(); i++) {
        LayeredSamplerSound *sound = static_cast<LayeredSamplerSound*>(sounds.getUnchecked(i).get());
        if (sound->appliesToNoteWithVelocity(midiNoteNumber, velocity) && sound->appliesToChannel(midiChannel)) {
            // stop note if still ringing
            for (auto *voice : voices) {
                if (voice->getCurrentlyPlayingNote() == midiNoteNumber && voice->isPlayingChannel(midiChannel)) {
                    stopVoice(voice, 1.0f, true);
                }
            }

            // start note
            auto *freeVoice = findFreeVoice(sound, midiChannel, midiNoteNumber, isNoteStealingEnabled());
            startVoice(freeVoice, sound, midiChannel, midiNoteNumber, velocity);
        }
    }
}

} // namespace processing
} // namespace soundboard
