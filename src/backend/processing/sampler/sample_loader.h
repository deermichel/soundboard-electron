#ifndef PROCESSING_STREAMING_SAMPLER_SAMPLE_LOADER_H
#define PROCESSING_STREAMING_SAMPLER_SAMPLE_LOADER_H

#include <juce_audio_basics/juce_audio_basics.h>
#include "papagei_sound.h"

// streaming buffer size
#define STREAMING_BUFFER_SIZE 512

namespace soundboard {
namespace processing {

// utility class handling buffered sample streaming in a background thread
class SampleLoader : public juce::ThreadPoolJob {
public:
    // construct sample loader
    SampleLoader(juce::ThreadPool *threadPool);

    // fill buffer with samples from the current read buffer (works with buffer swapping)
    void fillSampleBlockBuffer(juce::AudioSampleBuffer &sampleBlockBuffer, int numSamples, int numActualSamples, int sampleIndex);

    // return loaded sound
    const PapageiSound* getLoadedSound() const { return mSound; }

    // reset loader (unloads sound)
    void reset();

    // set buffer size
    void setBufferSize(unsigned int bufferSize);

    // call this when sound was started (sets the read pointer to the preload buffer and starts the background reading)
    void startNote(PapageiSound const *sound);

    // --- overrides ---

    // fill currently inactive buffer with samples from the sampler sound
    juce::ThreadPoolJob::JobStatus runJob() override;

private:
    // internal buffers
    juce::AudioSampleBuffer mBuffer1, mBuffer2;

    // current buffer size
    unsigned int mBufferSize;

    // critical section lock
    juce::CriticalSection mLock;

    // position in sample file
    long long mPositionInSampleFile;

    // current read buffer pointer
    juce::AudioSampleBuffer const *mReadBuffer;

    // current read index
    unsigned int mReadIndex;

    // the sound being loaded
    PapageiSound const *mSound;

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

    // swap buffers
    bool swapBuffers();
};

} // namespace processing
} // namespace soundboard

#endif // PROCESSING_STREAMING_SAMPLER_SAMPLE_LOADER_H
