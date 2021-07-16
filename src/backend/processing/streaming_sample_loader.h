#ifndef PROCESSING_STREAMING_SAMPLE_LOADER_H
#define PROCESSING_STREAMING_SAMPLE_LOADER_H

#include <juce_audio_basics/juce_audio_basics.h>
#include "streaming_sampler_sound.h"

namespace soundboard {
namespace processing {

// utility class handling buffered sample streaming in a background thread
class StreamingSampleLoader : public juce::ThreadPoolJob {
public:
    // construct sample loader
    StreamingSampleLoader(juce::ThreadPool *threadPool);

    // reset loader (unloads sound)
    void reset();

    // set buffer size
    void setBufferSize(unsigned int bufferSize);

    // --- overrides ---

    // fill currently inactive buffer with samples from the sampler sound
    juce::ThreadPoolJob::JobStatus runJob() override;

private:
    // internal buffers
    juce::AudioSampleBuffer mBuffer1, mBuffer2;

    // current buffer size
    unsigned int mBufferSize;

    // disk usage
    double mDiskUsage;

    // critical section lock
    juce::CriticalSection mLock;

    // current read buffer pointer
    juce::AudioSampleBuffer const *mReadBuffer;

    // the sound being loaded
    StreamingSamplerSound const *mSound;

    // thread pool used for this job
    juce::ThreadPool *mThreadPool;

    // current write buffer pointer
    juce::AudioSampleBuffer *mWriteBuffer;
};

} // namespace processing
} // namespace soundboard

#endif // PROCESSING_STREAMING_SAMPLE_LOADER_H
