#include "linked_float_parameter.h"
#include "oscillator.h"

namespace soundboard {
namespace processing {

// construct oscillator
Oscillator::Oscillator() {
    // add parameters
    addParameter(new LinkedFloatParameter("frequency", 20, 20000, 440, std::bind(&juce::dsp::Oscillator<float>::setFrequency, &mOscillator, std::placeholders::_1, false)));

    // init oscillator
    mOscillator.setFrequency(440.0f);
    mOscillator.initialise([](float x) { return std::sin(x); });
}

// called before playback starts, to let the processor prepare itself
void Oscillator::prepareToPlay(double sampleRate, int bufferSize) {
    juce::dsp::ProcessSpec spec { sampleRate, static_cast<juce::uint32>(bufferSize), 2 };
    mOscillator.prepare(spec);
}

// renders the next block
void Oscillator::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages) {
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    mOscillator.process(context);
}

// should reset any playing voices
void Oscillator::reset() {
    mOscillator.reset();
}

} // namespace processing
} // namespace soundboard
