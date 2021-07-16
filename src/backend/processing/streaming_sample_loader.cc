#include "streaming_sampler.h" // TODO: move defines to better place
#include "streaming_sample_loader.h"

namespace soundboard {
namespace processing {

// construct sample loader
StreamingSampleLoader::StreamingSampleLoader(juce::ThreadPool *threadPool) :
    ThreadPoolJob("StreamingSampleLoader"),
    mThreadPool(threadPool) {}

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

// fill currently inactive buffer with samples from the sampler sound
juce::ThreadPoolJob::JobStatus StreamingSampleLoader::runJob() {
    return JobStatus::jobHasFinished;
}

// --- private methods ---

} // namespace processing
} // namespace soundboard
