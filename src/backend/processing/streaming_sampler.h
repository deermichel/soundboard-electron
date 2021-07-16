#ifndef PROCESSING_STREAMING_SAMPLER_H
#define PROCESSING_STREAMING_SAMPLER_H

#include <juce_audio_basics/juce_audio_basics.h>

// maximum value for sample pitch manipulation (8 = 3 octaves)
#define MAX_SAMPLER_PITCH 8

// preload buffer size // TODO: split into two defines
#define PRELOAD_BUFFER_SIZE 11000

// load samples in audio thread (useful for debugging)
#define LOAD_SAMPLES_IN_AUDIO_THREAD 0

#endif // PROCESSING_STREAMING_SAMPLER_H
