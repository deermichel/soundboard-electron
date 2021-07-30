#include "papagei_sound.h"

namespace soundboard {
namespace processing {

// construct sampler sound
PapageiSound::PapageiSound(const juce::File &file, const juce::BigInteger &midiNotes, const juce::BigInteger &velocities, int midiNoteForNormalPitch) :
    mFileName(file.getFullPathName().toStdString()), mMidiNotes(midiNotes), mRootNote(midiNoteForNormalPitch), mVelocities(velocities) {

    // map file to memory
    juce::WavAudioFormat waf;
    mAudioFormatReader.reset(waf.createMemoryMappedReader(file));
    if (mAudioFormatReader == nullptr) throw std::runtime_error("file does not exist: " + mFileName);
    mAudioFormatReader->mapEntireFile();
    if (mAudioFormatReader->getMappedSection().isEmpty()) throw std::runtime_error("failed mapping file to memory: " + mFileName);
    mSourceSampleRate = mAudioFormatReader->sampleRate;
    mLength = mAudioFormatReader->lengthInSamples;
    if (mSourceSampleRate == 0 || mLength == 0) throw std::runtime_error("invalid sample rate or empty audio file: " + mFileName);

    // adsr
    mAdsrParams.attack = 0.001;
    mAdsrParams.release = 0.1;

    // preload sample
    setPreloadSize(PRELOAD_BUFFER_SIZE);
}

// return true if this sound should be played when a given midi note is pressed
bool PapageiSound::appliesToNoteWithVelocity(int midiNoteNumber, int velocity) {
    return mMidiNotes[midiNoteNumber] && mVelocities[velocity];
}

// set the preload size (-1 for entire sample)
void PapageiSound::setPreloadSize(int sizeInSamples) {
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

// get sound into active memory
void PapageiSound::wakeSound() {
    mAudioFormatReader->touchSample(0);
}

// return true if this sound should be played for a given midi channel
bool PapageiSound::appliesToChannel(int midiChannel) {
    return true;
}

} // namespace processing
} // namespace soundboard
