#include "papagei_sampler.h"
#include "papagei_sound.h"

namespace soundboard {
namespace processing {

// handle note on event
void PapageiSampler::noteOn(int midiChannel, int midiNoteNumber, float velocity) {
    const juce::ScopedLock sl(lock);

    // iterate sounds in current bank
    for (unsigned int i = 0; i < sounds.size(); i++) {
        PapageiSound *sound = static_cast<PapageiSound*>(sounds.getUnchecked(i).get());
        if (sound->appliesToNoteWithVelocity(midiNoteNumber, velocity * 127.0f) && sound->appliesToChannel(midiChannel)) {
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
}

} // namespace processing
} // namespace soundboard
