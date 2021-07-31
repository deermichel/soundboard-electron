#include "papagei_sound.h"
#include "papagei_voice.h"

namespace soundboard {
namespace processing {

// construct voice using given thread pool
PapageiVoice::PapageiVoice(juce::ThreadPool *backgroundThreadPool) :
    mLoader(backgroundThreadPool) {}

// prepare internal sample buffer
void PapageiVoice::prepareToPlay(double sampleRate, int samplesPerBlock) {
    if (sampleRate > 0) {
        mSamplesForThisBlock = juce::AudioSampleBuffer(2, samplesPerBlock * MAX_SAMPLER_PITCH + 2);
        mSamplesForThisBlock.clear();
    }
}

// return true if this voice object is capable of playing the given sound
bool PapageiVoice::canPlaySound(juce::SynthesiserSound *sound) {
    return dynamic_cast<PapageiSound*>(sound) != nullptr;
}

// renders the next block of data for this voice
void PapageiVoice::renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples) {
    auto *sound = mLoader.getLoadedSound();
    if (sound == nullptr) return;

    // samples calculation
    int offset = (int)mSourceSamplePosition;
    int samplesToCopy = numSamples * mPitchRatio + 2; // get a few more for linear interpolation

    // check if enough samples are available
    if (!sound->hasEnoughSamplesForBlock(offset + samplesToCopy)) {
        stopNote(0.0f, false);
        return;
    }

    // get buffers
    mLoader.fillSampleBlockBuffer(mSamplesForThisBlock, samplesToCopy, (mSourceSamplePosition + numSamples * mPitchRatio) - offset, offset);
    const float *inL = mSamplesForThisBlock.getReadPointer(0);
    const float *inR = mSamplesForThisBlock.getNumChannels() > 1 ? mSamplesForThisBlock.getReadPointer(1) : nullptr;
    float *outL = outputBuffer.getWritePointer(0, startSample);
    float *outR = outputBuffer.getNumChannels() > 1 ? outputBuffer.getWritePointer(1, startSample) : nullptr;

    while (--numSamples >= 0) {
        int pos = (int)mSourceSamplePosition;
        float alpha = (float)(mSourceSamplePosition - pos);
        float invAlpha = 1.0f - alpha;
        int index = pos - offset;

        float l = (inL[index] * invAlpha + inL[index + 1] * alpha);
        float r = (inR != nullptr) ? (inR[index] * invAlpha + inR[index + 1] * alpha) : l;

        auto envelopeValue = mAdsr.getNextSample();

        l *= mGain * envelopeValue;
        r *= mGain * envelopeValue;

        if (outR != nullptr) {
            *outL++ += l;
            *outR++ += r;
        } else {
            *outL++ += (l + r) * 0.5f;
        }

        mSourceSamplePosition += mPitchRatio;

        // if (mSourceSamplePosition > sound->getLength()) {
        //     stopNote(0.0f, false);
        //     break;
        // }
    }
}

// start streaming the sound
void PapageiVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound *s, int currentPitchWheelPosition) {
    if (auto *sound = dynamic_cast<PapageiSound*>(s)) {
        mLoader.startNote(sound);
        sound->wakeSound();
        mSourceSamplePosition = 0.0;

        // pitch & velocity
        mPitchRatio = juce::jmin(sound->getPitchRatio(midiNoteNumber), (double)MAX_SAMPLER_PITCH) * sound->getSourceSampleRate() / getSampleRate();
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
        mLoader.reset();
    }
}

} // namespace processing
} // namespace soundboard
