#ifndef PROCESSING_SAMPLER_PAPAGEI_SAMPLER_H
#define PROCESSING_SAMPLER_PAPAGEI_SAMPLER_H

#include <juce_audio_basics/juce_audio_basics.h>

namespace soundboard {
namespace processing {

// advanced sampler supporting disk streaming, multiple velocity layers and round-robin banks
class PapageiSampler : public juce::Synthesiser {
public:
    // construct sampler
    PapageiSampler() {}

    // --- overrides ---

    // handle note on event
    void noteOn(int midiChannel, int midiNoteNumber, float velocity) override;

private:
};

} // namespace processing
} // namespace soundboard

#endif // PROCESSING_SAMPLER_PAPAGEI_SAMPLER_H
