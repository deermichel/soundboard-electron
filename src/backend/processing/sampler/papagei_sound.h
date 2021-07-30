#ifndef PROCESSING_SAMPLER_PAPAGEI_SOUND_H
#define PROCESSING_SAMPLER_PAPAGEI_SOUND_H

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_formats/juce_audio_formats.h>

// preload buffer size
#define PRELOAD_BUFFER_SIZE 11000

namespace soundboard {
namespace processing {

// synthesizer sampler sound which provides velocity support and buffered disk streaming using memory mapped file access and a preloaded sample start
class PapageiSound : public juce::SynthesiserSound {
public:
    // construct sampler sound
    PapageiSound(const juce::File &file, const juce::BigInteger &midiNotes, const juce::BigInteger &velocities, int midiNoteForNormalPitch);

    // return true if this sound should be played when a given midi note is pressed
    bool appliesToNoteWithVelocity(int midiNoteNumber, int velocity);

    // return adsr parameters
    juce::ADSR::Parameters getEnvelopeParameters() const { return mAdsrParams; }

    // return length in samples
    unsigned int getLength() const { return mLength; }

	// return pitch ratio for the note number
	double getPitchRatio(int noteNumberToPitch) const { return pow(2.0, (noteNumberToPitch - mRootNote) / 12.0); }

    // return read-only access to the preload buffer
    const juce::AudioSampleBuffer& getPreloadBuffer() const { return mPreloadBuffer; }

    // return source sample rate
    double getSourceSampleRate() const { return mSourceSampleRate; }

    // check if file is mapped and has enough samples
    bool hasEnoughSamplesForBlock(long long maxSampleIndexInFile) const;

    // load entire sample in memory
    void loadEntireSample() { setPreloadSize(-1); }

    // set adsr parameters
    void setEnvelopeParameters(const juce::ADSR::Parameters &params) { mAdsrParams = params; }

    // set the preload size (-1 for entire sample)
    void setPreloadSize(int sizeInSamples);

    // get sound into active memory
    void wakeSound();

    // --- overrides ---

    // return true if this sound should be played for a given midi channel
    bool appliesToChannel(int midiChannel) override;

    // return true if this sound should be played when a given midi note is pressed
    bool appliesToNote(int midiNoteNumber) override { return mMidiNotes[midiNoteNumber]; } // TODO: currently ignore velocity for noteOff

private:
    // adsr parameters
    juce::ADSR::Parameters mAdsrParams;

    // memory reader
    std::unique_ptr<juce::MemoryMappedAudioFormatReader> mAudioFormatReader;

    // file name
    const std::string mFileName;

    // sample length
    unsigned int mLength;

    // midi note mapping
    const juce::BigInteger mMidiNotes;

    // preload buffer
    juce::AudioSampleBuffer mPreloadBuffer;

    // current preload size
    int mPreloadSize;

    // midi root note
    const int mRootNote;

    // source sample rate
    double mSourceSampleRate;

    // velocity range
    const juce::BigInteger mVelocities;

    // you got a friend in me
    friend class SampleLoader;

    // fill the supplied buffer with samples (don't call from audio thread, might read data from disk)
	void fillSampleBuffer(juce::AudioSampleBuffer &sampleBuffer, int samplesToCopy, int startOffset) const;

    // non copy, leak detection
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PapageiSound)
};

} // namespace processing
} // namespace soundboard

#endif // PROCESSING_SAMPLER_PAPAGEI_SOUND_H
