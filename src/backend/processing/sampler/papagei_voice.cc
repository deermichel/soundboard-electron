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
        mSamplesForThisBlock = juce::AudioSampleBuffer(2, samplesPerBlock * MAX_SAMPLER_PITCH);
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

    // get buffers
    const juce::AudioSampleBuffer &preloadBuffer = sound->getPreloadBuffer();
    const float *inL = preloadBuffer.getReadPointer(0);
    const float *inR = preloadBuffer.getNumChannels() > 1 ? preloadBuffer.getReadPointer(1) : nullptr;
    float *outL = outputBuffer.getWritePointer(0, startSample);
    float *outR = outputBuffer.getNumChannels() > 1 ? outputBuffer.getWritePointer(1, startSample) : nullptr;

    // copy samples and interpolate as needed
    while (--numSamples >= 0) {
        int index = (int)mSourceSamplePosition;
        float alpha = (float)(mSourceSamplePosition - index);
        float invAlpha = 1.0f - alpha;

        // simple linear interpolation
        float l = (inL[index] * invAlpha + inL[index + 1] * alpha);
        float r = (inR != nullptr) ? (inR[index] * invAlpha + inR[index + 1] * alpha) : l;

        // envelope & gain
        auto envelopeValue = mAdsr.getNextSample();
        l *= mGain * envelopeValue;
        r *= mGain * envelopeValue;

        if (outR != nullptr) {
            *outL++ += l;
            *outR++ += r;
        } else {
            *outL++ += (l + r) * 0.5f;
        }

        // adjust pos
        mSourceSamplePosition += mPitchRatio;
        if (mSourceSamplePosition > sound->getLength()) {
            stopNote(0.0f, false);
            break;
        }
    }

    // auto *sound = mLoader.getLoadedSound();
    // if (sound == nullptr) return;

    // // samples calculation
    // int pos = (int)mSourceSamplePosition;
    // double numSamplesUsed = mSourceSamplePosition - pos;
    // numSamplesUsed += mPitchRatio * numSamples;
    // int samplesToCopy = (int)numSamplesUsed + 2; // get a few more for linear interpolating

    // // check if enough samples are available
    // if (!sound->hasEnoughSamplesForBlock(pos + samplesToCopy)) {
    //     stopNote(0.0f, false);
    //     return;
    // }

    // // get buffers
    // mLoader.fillSampleBlockBuffer(mSamplesForThisBlock, samplesToCopy, pos);
    // const float *inL = mSamplesForThisBlock.getReadPointer(0);
    // const float *inR = mSamplesForThisBlock.getNumChannels() > 1 ? mSamplesForThisBlock.getReadPointer(1) : nullptr;
    // float *outL = outputBuffer.getWritePointer(0, startSample);
    // float *outR = outputBuffer.getNumChannels() > 1 ? outputBuffer.getWritePointer(1, startSample) : nullptr;

    // // copy samples and interpolate as needed
    // while (--numSamples >= 0) {
    //     float indexFloat = (float)(mSourceSamplePosition - pos);
    //     int index = (int)indexFloat;
    //     jassert(index + 1 <= samplesToCopy);
    //     float alpha = indexFloat - index;
    //     float invAlpha = 1.0f - alpha;

    //     // simple linear interpolation
    //     float l = (inL[index] * invAlpha + inL[index + 1] * alpha);
    //     float r = (inR != nullptr) ? (inR[index] * invAlpha + inR[index + 1] * alpha) : l;

    //     // envelope & gain
    //     auto envelopeValue = mAdsr.getNextSample() * 2; // TODO: remove 2 !!!
    //     l *= mGain * envelopeValue;
    //     r *= mGain * envelopeValue;

    //     if (outR != nullptr) {
    //         *outL++ += l;
    //         *outR++ += r;
    //     } else {
    //         *outL++ += (l + r) * 0.5f;
    //     }

    //     // increment pos
    //     mSourceSamplePosition += mPitchRatio;
    // }
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
