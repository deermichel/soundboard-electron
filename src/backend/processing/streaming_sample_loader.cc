#include "streaming_sampler.h" // TODO: move defines to better place
#include "streaming_sample_loader.h"

namespace soundboard {
namespace processing {

// construct sample loader
StreamingSampleLoader::StreamingSampleLoader(juce::ThreadPool *threadPool) :
    ThreadPoolJob("StreamingSampleLoader"),
    mDiskUsage(0.0),
    mLastCallToRequestData(0.0),
    mPositionInSampleFile(0),
    mSound(nullptr),
    mThreadPool(threadPool),
    mWriteBufferIsBeingFilled(false) {

    // set buffer size
    setBufferSize(PRELOAD_BUFFER_SIZE);
}

// reset loader (unloads sound)
void StreamingSampleLoader::reset() {
    juce::ScopedLock lock(mLock);
    mSound = nullptr;
    mDiskUsage = 0.0;
}

// set buffer size
void StreamingSampleLoader::setBufferSize(unsigned int bufferSize) {
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
void StreamingSampleLoader::startNote(StreamingSamplerSound const *sound) {
    juce::ScopedLock lock(mLock);

    // reset vars
    mDiskUsage = 0.0;
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
juce::ThreadPoolJob::JobStatus StreamingSampleLoader::runJob() {
    const double readStart = juce::Time::highResolutionTicksToSeconds(juce::Time::getHighResolutionTicks());

    // fill buffer
    fillInactiveBuffer();
    mWriteBufferIsBeingFilled = false;

    // measure disk usage
    const double readStop = juce::Time::highResolutionTicksToSeconds(juce::Time::getHighResolutionTicks());
    const double readTime = readStop - readStart;
    const double timeSinceLastCall = readStop - mLastCallToRequestData;
    const double diskUsageThisTime = readTime / timeSinceLastCall;
    mDiskUsage = juce::jmax(mDiskUsage, diskUsageThisTime);
    mLastCallToRequestData = readStart;

    return JobStatus::jobHasFinished;
}

// --- private methods ---

// fill inactive buffer with samples from the sampler sound
void StreamingSampleLoader::fillInactiveBuffer() {
    if (mSound != nullptr && mSound->hasEnoughSamplesForBlock(mBufferSize + mPositionInSampleFile)) {
        mSound->fillSampleBuffer(*mWriteBuffer, mBufferSize, (int)mPositionInSampleFile);
    }
}

// kick off background job
void StreamingSampleLoader::requestNewData() {
    mWriteBufferIsBeingFilled = true; // mutex

#if LOAD_SAMPLES_IN_AUDIO_THREAD
    runJob(); // run synchronously
#else
    jassert(!mThreadPool->contains(this)); // check if the background thread is already loading this sound
    mThreadPool->addJob(this, false);
#endif
}

} // namespace processing
} // namespace soundboard
