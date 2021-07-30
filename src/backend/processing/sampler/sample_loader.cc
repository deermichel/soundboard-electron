#include "sample_loader.h"

namespace soundboard {
namespace processing {

// construct sample loader
SampleLoader::SampleLoader(juce::ThreadPool *threadPool) :
    ThreadPoolJob("SampleLoader"),
    mPositionInSampleFile(0),
    mReadIndex(0),
    mSound(nullptr),
    mThreadPool(threadPool),
    mWriteBufferIsBeingFilled(false) {

    // set buffer size
    setBufferSize(STREAMING_BUFFER_SIZE);
}

// fill buffer with samples from the current read buffer (works with buffer swapping)
void SampleLoader::fillSampleBlockBuffer(juce::AudioSampleBuffer &sampleBlockBuffer, int numSamples, int sampleIndex) {
    // since the numSamples is only a estimate for buffering, the sampleIndex is used for the exact clock
    mReadIndex = sampleIndex % mBufferSize;

    // fill buffer
    jassert(mSound != nullptr);
    if (mReadIndex + numSamples < mBufferSize) {
        // copy all samples from the current read buffer
        juce::FloatVectorOperations::copy(sampleBlockBuffer.getWritePointer(0, 0), mReadBuffer->getReadPointer(0, mReadIndex), numSamples);
        juce::FloatVectorOperations::copy(sampleBlockBuffer.getWritePointer(1, 0), mReadBuffer->getReadPointer(1, mReadIndex), numSamples);
    } else {
        // copy remaining samples from current read buffer
        const int remainingSamples = mBufferSize - mReadIndex;
        jassert(remainingSamples <= numSamples);
        juce::FloatVectorOperations::copy(sampleBlockBuffer.getWritePointer(0, 0), mReadBuffer->getReadPointer(0, mReadIndex), remainingSamples);
        juce::FloatVectorOperations::copy(sampleBlockBuffer.getWritePointer(1, 0), mReadBuffer->getReadPointer(1, mReadIndex), remainingSamples);

        // swap buffers and copy remaining samples from the new read buffer
        if (swapBuffers()) {
            mReadIndex = 0;
            const int numSamplesInNewReadBuffer = numSamples - remainingSamples;
            juce::FloatVectorOperations::copy(sampleBlockBuffer.getWritePointer(0, remainingSamples), mReadBuffer->getReadPointer(0, mReadIndex), numSamplesInNewReadBuffer);
            juce::FloatVectorOperations::copy(sampleBlockBuffer.getWritePointer(1, remainingSamples), mReadBuffer->getReadPointer(1, mReadIndex), numSamplesInNewReadBuffer);
            mPositionInSampleFile += mBufferSize;
            requestNewData();
        } else {
            jassertfalse; // ERROR: background thread was not quick enough -> increase preload / buffer size
        }
    }
}

// reset loader (unloads sound)
void SampleLoader::reset() {
    juce::ScopedLock lock(mLock);
    mSound = nullptr;
}

// set buffer size
void SampleLoader::setBufferSize(unsigned int bufferSize) {
    mBufferSize = bufferSize;

    // allocate and clear buffers
    mBuffer1 = juce::AudioSampleBuffer(2, bufferSize);
    mBuffer2 = juce::AudioSampleBuffer(2, bufferSize);
    mBuffer1.clear();
    mBuffer2.clear();

    // reset pointers and loader
    mReadBuffer = &mBuffer1;
    mWriteBuffer = &mBuffer2;
    reset();
}

// call this when sound was started (sets the read pointer to the preload buffer and starts the background reading)
void SampleLoader::startNote(PapageiSound const *sound) {
    juce::ScopedLock lock(mLock);

    // reset vars
    mSound = sound;
    mReadIndex = 0;

    // set read pointer point to preload buffer
    mReadBuffer = &sound->getPreloadBuffer();

    // ERROR: preload buffer must be at least as big as the buffer size
    jassert(mReadBuffer->getNumSamples() >= mBufferSize);

    // prepare writing
    mWriteBuffer = &mBuffer1;
    mPositionInSampleFile = mBufferSize;
    if (!mWriteBufferIsBeingFilled) requestNewData();
}

// fill currently inactive buffer with samples from the sampler sound
juce::ThreadPoolJob::JobStatus SampleLoader::runJob() {
    // fill buffer
    fillInactiveBuffer();
    mWriteBufferIsBeingFilled = false;

    return JobStatus::jobHasFinished;
}

// --- private methods ---

// fill inactive buffer with samples from the sampler sound
void SampleLoader::fillInactiveBuffer() {
    if (mSound != nullptr && mSound->hasEnoughSamplesForBlock(mBufferSize + mPositionInSampleFile)) {
        mSound->fillSampleBuffer(*mWriteBuffer, mBufferSize, (int)mPositionInSampleFile);
    }
}

// kick off background job
void SampleLoader::requestNewData() {
    mWriteBufferIsBeingFilled = true; // mutex

// #if LOAD_SAMPLES_IN_AUDIO_THREAD
//     runJob(); // run synchronously
// #else
    jassert(!mThreadPool->contains(this)); // check if the background thread is already loading this sound
    mThreadPool->addJob(this, false);
// #endif
}

// swap buffers
bool SampleLoader::swapBuffers() {
    if (mReadBuffer == &mBuffer1) {
        mReadBuffer = &mBuffer2;
        mWriteBuffer = &mBuffer1;
    } else { // will also be true if read pointer points at preload buffer
        mReadBuffer = &mBuffer1;
        mWriteBuffer = &mBuffer2;
    }
    return mWriteBufferIsBeingFilled == false;
}

} // namespace processing
} // namespace soundboard
