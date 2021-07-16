#include "streaming_sampler.h" // TODO: move defines to better place
#include "streaming_sampler_sound.h"

namespace soundboard {
namespace processing {

// construct sampler sound
StreamingSamplerSound::StreamingSamplerSound(const juce::File &file, const juce::BigInteger &midiNotes, int midiNoteForNormalPitch) :
    mFileName(file.getFullPathName().toStdString()), mMidiNotes(midiNotes), mRootNote(midiNoteForNormalPitch) {

    // map file to memory
    juce::WavAudioFormat waf;
    mAudioFormatReader.reset(waf.createMemoryMappedReader(file));
    if (mAudioFormatReader == nullptr) throw std::runtime_error("file does not exist: " + mFileName);
    mAudioFormatReader->mapEntireFile();
    if (mAudioFormatReader->getMappedSection().isEmpty()) throw std::runtime_error("failed mapping file to memory: " + mFileName);
    mSampleRate = mAudioFormatReader->sampleRate;

    // preload sample
    setPreloadSize(PRELOAD_BUFFER_SIZE);
}

// set the preload size (-1 for entire sample)
void StreamingSamplerSound::setPreloadSize(int sizeInSamples) {
    mPreloadSize = sizeInSamples;
    auto maxSize = mAudioFormatReader->getMappedSection().getLength();

     // cap size to file maximum
    if (sizeInSamples == -1 || sizeInSamples > maxSize) {
        mPreloadSize = (int)maxSize;
    }

    // create and fill preload buffer
    try {
        mPreloadBuffer = juce::AudioSampleBuffer(2, mPreloadSize);
    } catch (const std::exception &e) {
        throw std::runtime_error("out of memory: " + mFileName);
    }
    mAudioFormatReader->read(&mPreloadBuffer, 0, mPreloadSize, 0, true, true);
}

// return true if this sound should be played for a given midi channel
bool StreamingSamplerSound::appliesToChannel(int midiChannel) {
    return true;
}

// return true if this sound should be played when a given midi note is pressed
bool StreamingSamplerSound::appliesToNote(int midiNoteNumber) {
    return mMidiNotes[midiNoteNumber];
}

// --- private methods ---

} // namespace processing
} // namespace soundboard
