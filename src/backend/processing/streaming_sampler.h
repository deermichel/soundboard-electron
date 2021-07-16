#ifndef PROCESSING_STREAMING_SAMPLER_H
#define PROCESSING_STREAMING_SAMPLER_H

#include <juce_audio_basics/juce_audio_basics.h>

// maximum value for sample pitch manipulation (8 = 3 octaves)
#define MAX_SAMPLER_PITCH 8

// preload buffer size
#define PRELOAD_BUFFER_SIZE 11000

// load samples in audio thread (useful for debugging)
#define LOAD_SAMPLES_IN_AUDIO_THREAD 0

namespace soundboard {
namespace processing {

// advanced version of the base sampler supporting disk streaming, multiple velocity layers and round-robin banks
// class StreamingSampler : public juce::Synthesiser {
// public:
//     // construct sampler
//     StreamingSampler() {}

//     // add sound bank (returns index)
//     unsigned int addSoundBank();

//     // add sound to bank
//     void addSoundToBank(const juce::ReferenceCountedObjectPtr<StreamingSamplerSound> &sound, unsigned int bank);

//     // clear sound banks
//     void clearSoundBanks();

//     // --- overrides ---

//     // handle note on event
//     void noteOn(int midiChannel, int midiNoteNumber, float velocity) override;

// private:
//     // sound banks
//     std::vector<juce::ReferenceCountedArray<StreamingSamplerSound>> mSoundBanks;

//     // next sound bank iterator
//     unsigned int mNextSoundBank = 0;
// };

} // namespace processing
} // namespace soundboard

#endif // PROCESSING_STREAMING_SAMPLER_H
