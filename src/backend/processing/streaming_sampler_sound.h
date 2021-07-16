#ifndef PROCESSING_STREAMING_SAMPLER_SOUND_H
#define PROCESSING_STREAMING_SAMPLER_SOUND_H

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_formats/juce_audio_formats.h>

namespace soundboard {
namespace processing {

// synthesizer sampler sound which provides buffered disk streaming using memory mapped file access and a preloaded sample start
class StreamingSamplerSound : public juce::SynthesiserSound {
public:
    // construct sampler sound
    StreamingSamplerSound(const juce::File &file, const juce::BigInteger &midiNotes, int midiNoteForNormalPitch);

    // set the preload size (-1 for entire sample)
    void setPreloadSize(int sizeInSamples);

    // --- overrides ---

    // return true if this sound should be played for a given midi channel
    bool appliesToChannel(int midiChannel) override;

    // return true if this sound should be played when a given midi note is pressed
    bool appliesToNote(int midiNoteNumber) override;

private:
    // memory reader
    std::unique_ptr<juce::MemoryMappedAudioFormatReader> mAudioFormatReader;

    // file name
    const std::string mFileName;

    // midi note mapping
    const juce::BigInteger mMidiNotes;

    // preload buffer
    juce::AudioSampleBuffer mPreloadBuffer;

    // current preload size
    int mPreloadSize;

    // midi root note
    const int mRootNote;

    // sample rate
    double mSampleRate;

    // non copy, leak detection
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StreamingSamplerSound)
};

} // namespace processing
} // namespace soundboard

#endif // PROCESSING_STREAMING_SAMPLER_SOUND_H
