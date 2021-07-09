#include "linked_float_parameter.h"
#include "mixer.h"

namespace soundboard {
namespace processing {

// construct mixer
Mixer::Mixer() {
    // init gain
    mGain.setGainLinear(0.0f);
    mGain.setRampDurationSeconds(0.1);

    // link parameters
    addParameter(new LinkedFloatParameter("gain", 0.0f, 1.0f, mGain.getGainLinear(), std::bind(&juce::dsp::Gain<float>::setGainLinear, &mGain, std::placeholders::_1)));
}

// called before playback starts, to let the processor prepare itself
void Mixer::prepareToPlay(double sampleRate, int bufferSize) {
    juce::dsp::ProcessSpec spec { sampleRate, static_cast<juce::uint32>(bufferSize), 2 };
    mGain.prepare(spec);
}

// renders the next block
void Mixer::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages) {
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    mGain.process(context);
}

// should reset any playing voices
void Mixer::reset() {
    mGain.reset();
}

} // namespace processing
} // namespace soundboard
