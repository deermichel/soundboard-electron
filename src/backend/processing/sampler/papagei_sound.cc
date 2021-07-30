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
    mAdsrParams.attack = 0.01;
    mAdsrParams.release = 0.1;

    // preload sample
    setPreloadSize(PRELOAD_BUFFER_SIZE);
}

// return true if this sound should be played when a given midi note is pressed
bool PapageiSound::appliesToNoteWithVelocity(int midiNoteNumber, int velocity) {
    return mMidiNotes[midiNoteNumber] && mVelocities[velocity];
}

// check if file is mapped and has enough samples
bool PapageiSound::hasEnoughSamplesForBlock(long long maxSampleIndexInFile) const {
    return maxSampleIndexInFile < mAudioFormatReader->getMappedSection().getEnd();
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

// --- private methods ---

// fill the supplied buffer with samples (don't call from audio thread, might read data from disk)
void PapageiSound::fillSampleBuffer(juce::AudioSampleBuffer &sampleBuffer, int samplesToCopy, int startOffset) const {
    if (startOffset + samplesToCopy < mPreloadSize) {
        juce::FloatVectorOperations::copy(sampleBuffer.getWritePointer(0, 0), mPreloadBuffer.getReadPointer(0, startOffset), samplesToCopy);
        juce::FloatVectorOperations::copy(sampleBuffer.getWritePointer(1, 0), mPreloadBuffer.getReadPointer(1, startOffset), samplesToCopy);
    } else {
        mAudioFormatReader->read(&sampleBuffer, 0, samplesToCopy, startOffset, true, true);
    }
}

} // namespace processing
} // namespace soundboard
