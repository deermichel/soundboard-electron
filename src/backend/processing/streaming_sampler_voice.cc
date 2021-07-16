#include "streaming_sampler.h"
#include "streaming_sampler_voice.h"

namespace soundboard {
namespace processing {

// construct sampler voice using given thread pool
StreamingSamplerVoice::StreamingSamplerVoice(juce::ThreadPool *backgroundThreadPool) :
    mLoader(backgroundThreadPool) {}

// prepare internal sample buffer
void StreamingSamplerVoice::prepareToPlay(double sampleRate, int samplesPerBlock) {
    if (sampleRate != -1.0) {
        mSamplesForThisBlock = juce::AudioSampleBuffer(2, samplesPerBlock * MAX_SAMPLER_PITCH);
        mSamplesForThisBlock.clear();
    }
}

// reset voice
void StreamingSamplerVoice::resetVoice() {
    mVoiceUptime = 0.0;
    mUptimeDelta = 0.0;
    clearCurrentNote();
}

// renders the next block of data for this voice
void StreamingSamplerVoice::renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples) {
    const StreamingSamplerSound *sound = mLoader.getLoadedSound();
    if (sound == nullptr) return;

    const int pos = (int)mVoiceUptime;
    double numSamplesUsed = mVoiceUptime - pos;

    // samples calculation
    if (mPitchData != nullptr) {
        for (int i = startSample; i < startSample + numSamples; i++) {
            numSamplesUsed += juce::jmin(mUptimeDelta * mPitchData[i], (double)MAX_SAMPLER_PITCH);
        }
    } else {
        for (int i = startSample; i < startSample + numSamples; i++) {
            numSamplesUsed += mUptimeDelta;
        }
    }
    const int samplesToCopy = (int)numSamplesUsed + 2; // get a few more for linear interpolating

    // check if enough samples are available
    if (!sound->hasEnoughSamplesForBlock(pos + samplesToCopy)) {
        resetVoice();
        return;
    }

    // copy samples and interpolate as needed
    mLoader.fillSampleBlockBuffer(mSamplesForThisBlock, samplesToCopy, pos);
    const float *inL = mSamplesForThisBlock.getReadPointer(0);
    const float *inR = mSamplesForThisBlock.getReadPointer(1);
    float *outL = outputBuffer.getWritePointer(0, startSample);
    float *outR = outputBuffer.getWritePointer(1, startSample);

    while (--numSamples >= 0) {
        const float indexFloat = (float)(mVoiceUptime - pos);
        const int index = (int)indexFloat;

        jassert(index + 1 <= samplesToCopy);

        const float alpha = indexFloat - index;
        const float invAlpha = 1.0f / alpha;

        float l = inL[index] * invAlpha + inL[index+1] * alpha;
        float r = inR[index] * invAlpha + inR[index+1] * alpha;

        *outL++ += l;
        *outR++ += r;

        mVoiceUptime += mUptimeDelta * (mPitchData == nullptr ? 1.0 : (double)mPitchData[startSample]);
        startSample++;
    }
}

// start streaming the sound
void StreamingSamplerVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition) {
    StreamingSamplerSound *streamingSound = dynamic_cast<StreamingSamplerSound*>(sound);
    mLoader.startNote(streamingSound);

    jassert(streamingSound != nullptr);
    streamingSound->wakeSound();

    mVoiceUptime = 0.0;
    mUptimeDelta = juce::jmin(streamingSound->getPitchFactor(midiNoteNumber), (double)MAX_SAMPLER_PITCH);
}

// stop the note
void StreamingSamplerVoice::stopNote(float velocity, bool allowTailOff) {
    clearCurrentNote();
    mLoader.reset();
}

// --- private methods ---

} // namespace processing
} // namespace soundboard
