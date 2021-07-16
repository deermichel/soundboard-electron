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

    // call this when sound was started (sets the read pointer to the preload buffer and starts the background reading)
    void startNote(StreamingSamplerSound const *sound);

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

    // disk usage utility var
    double mLastCallToRequestData;

    // critical section lock
    juce::CriticalSection mLock;

    // position in sample file
    long long mPositionInSampleFile;

    // current read buffer pointer
    juce::AudioSampleBuffer const *mReadBuffer;

    // current read index
    unsigned int mReadIndex;

    // the sound being loaded
    StreamingSamplerSound const *mSound;

    // thread pool used for this job
    juce::ThreadPool *mThreadPool;

    // current write buffer pointer
    juce::AudioSampleBuffer *mWriteBuffer;

    // simple mutex for the buffer that is being used for loading
    bool mWriteBufferIsBeingFilled;

    // fill inactive buffer with samples from the sampler sound
    void fillInactiveBuffer();

    // kick off background job
    void requestNewData();
};

} // namespace processing
} // namespace soundboard

#endif // PROCESSING_STREAMING_SAMPLE_LOADER_H
