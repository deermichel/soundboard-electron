#include "streaming_sampler_voice.h"

namespace soundboard {
namespace processing {

// construct sampler voice using given thread pool
StreamingSamplerVoice::StreamingSamplerVoice(juce::ThreadPool *backgroundThreadPool) :
    mLoader(backgroundThreadPool) {}

// --- private methods ---

} // namespace processing
} // namespace soundboard
