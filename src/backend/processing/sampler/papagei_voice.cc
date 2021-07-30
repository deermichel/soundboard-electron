#include "papagei_sound.h"
#include "papagei_voice.h"

namespace soundboard {
namespace processing {

// construct voice using given thread pool
PapageiVoice::PapageiVoice(juce::ThreadPool *backgroundThreadPool) {

}

// return true if this voice object is capable of playing the given sound
bool PapageiVoice::canPlaySound(juce::SynthesiserSound *sound) {
    return dynamic_cast<PapageiSound*>(sound) != nullptr;
}

// renders the next block of data for this voice
void PapageiVoice::renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples) {

}

// start streaming the sound
void PapageiVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound *s, int currentPitchWheelPosition) {
    if (auto *sound = dynamic_cast<PapageiSound*>(s)) {
        sound->wakeSound();
        mSourceSamplePosition = 0.0;

        // pitch & velocity
        mPitchRatio = sound->getPitchRatio(midiNoteNumber) * sound->getSourceSampleRate() / getSampleRate();
        mGain = velocity;

        // adsr
        mAdsr.setSampleRate(sound->getSourceSampleRate());
        mAdsr.setParameters(sound->getEnvelopeParameters());
        mAdsr.noteOn();
    } else {
        jassertfalse; // ERROR: PapageiVoice can only play PapageiSounds
    }
}

// stop the note
void PapageiVoice::stopNote(float velocity, bool allowTailOff) {
    if (allowTailOff) {
        mAdsr.noteOff();
    } else {
        clearCurrentNote();
        mAdsr.reset();
    }
}

} // namespace processing
} // namespace soundboard
