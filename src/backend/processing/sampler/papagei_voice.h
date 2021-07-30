#ifndef PROCESSING_SAMPLER_PAPAGEI_VOICE_H
#define PROCESSING_SAMPLER_PAPAGEI_VOICE_H

#include <juce_audio_basics/juce_audio_basics.h>

namespace soundboard {
namespace processing {

// synthesizer sampler voice to stream data from a PapageiSound
class PapageiVoice : public juce::SynthesiserVoice {
public:
    // construct voice using given thread pool
    PapageiVoice(juce::ThreadPool *backgroundThreadPool);

    // prepare internal sample buffer
    // void prepareToPlay(double sampleRate, int samplesPerBlock);

    // reset voice
    // void resetVoice();

    // set streaming loader buffer size
    // void setLoaderBufferSize(int newBufferSize) { mLoader.setBufferSize(newBufferSize); }

    // set pitch information for each sample of the next block (array size must match block size)
	// void setPitchValues(const float *pitchDataForBlock)	{ mPitchData = pitchDataForBlock; }

    // --- overrides ---

    // return true if this voice object is capable of playing the given sound
    bool canPlaySound(juce::SynthesiserSound *sound) override;

    // midi controller moved
    void controllerMoved (int controllerNumber, int newControllerValue) override {}

    // pitch wheel moved
    void pitchWheelMoved (int newPitchWheelValue) override {}

    // renders the next block of data for this voice
    void renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples) override;

    // start streaming the sound
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition) override;

    // stop the note
    void stopNote(float velocity, bool allowTailOff) override;

private:
    // adsr
    juce::ADSR mAdsr;

    // velocity gain
    float mGain;

    // pitch ratio
    double mPitchRatio;

    // source sample position
    double mSourceSamplePosition;
};

} // namespace processing
} // namespace soundboard

#endif // PROCESSING_SAMPLER_PAPAGEI_VOICE_H
