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
        mInputBuffer = juce::AudioSampleBuffer(2, samplesPerBlock * MAX_SAMPLER_PITCH);
        mOutputBuffer = juce::AudioSampleBuffer(2, samplesPerBlock);
        mInputBuffer.clear();
        mOutputBuffer.clear();
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
    int pos = (int)mSourceSamplePosition;
    int samplesToCopy = mPitchRatio * numSamples + 2; // get a few more for interpolating

    // check if enough samples are available
    if (!sound->hasEnoughSamplesForBlock(pos + samplesToCopy)) {
        stopNote(0.0f, false);
        return;
    }

    // get buffers
    // mLoader.fillSampleBlockBuffer(mInputBuffer, samplesToCopy, pos);
    auto &buffer = sound->getPreloadBuffer();
    const float *inL = buffer.getReadPointer(0);
    const float *inR = buffer.getNumChannels() > 1 ? buffer.getReadPointer(1) : nullptr;
    float *outL = mOutputBuffer.getWritePointer(0);
    float *outR = mOutputBuffer.getNumChannels() > 1 ? mOutputBuffer.getWritePointer(1) : nullptr;

    // interpolate
    mLeftInterpolator.process(mPitchRatio, inL+(int)mSourceSamplePosition, outL, numSamples);
    if (outR != nullptr) mRightInterpolator.process(mPitchRatio, inR+(int)mSourceSamplePosition, outR, numSamples);
    // mOutputBuffer.clear();
    // mOutputBuffer.addFrom(0, 0, inL+(int)mSourceSamplePosition, numSamples);
    // mOutputBuffer.addFrom(1, 0, inR+(int)mSourceSamplePosition, numSamples);

    // envelope
    mAdsr.applyEnvelopeToBuffer(mOutputBuffer, 0, numSamples);

    // write output
    outputBuffer.addFrom(0, startSample, outL, numSamples, mGain);
    outputBuffer.addFrom(1, startSample, outR, numSamples, mGain);

    // increment pos
    mSourceSamplePosition += numSamples * mPitchRatio;
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
