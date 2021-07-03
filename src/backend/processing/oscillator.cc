#include "oscillator.h"

namespace soundboard {
namespace processing {

// construct oscillator
Oscillator::Oscillator() {
    // add parameters
    addParameter(mFrequency = new juce::AudioParameterFloat("freq", "Frequency", 20.0f, 20000.0f, 440.0f));
    addParameter(mGain = new juce::AudioParameterFloat("gain", "Gain", 0.0f, 1.0f, 0.0f));
    addParameter(mSaw = new juce::AudioParameterBool("saw", "Sain / Saw", false));

    // init oscillator
    mOscillator.setFrequency(440.0f);
    mOscillator.initialise([](float x) { return std::sin(x); });

    // parameter change listener
    mFrequency->addListener(this);
}

// the host will call this method when it wants to save the processor internal state
void Oscillator::getStateInformation (juce::MemoryBlock &destData) {
    juce::MemoryOutputStream(destData, true).writeFloat(*mFrequency);
}

// parameter value changed callback
void Oscillator::parameterValueChanged(int parameterIndex, float newValue) {
    mOscillator.setFrequency(mFrequency->get());
}

// called before playback starts, to let the processor prepare itself
void Oscillator::prepareToPlay(double sampleRate, int bufferSize) {
    mOscillator.prepare({ sampleRate, static_cast<juce::uint32>(bufferSize) });
}

// renders the next block
void Oscillator::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages) {
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    mOscillator.process(context);
    buffer.applyGain(*mGain);
}

// should reset any playing voices
void Oscillator::reset() {
    mOscillator.reset();
}

// this must restore the processor state from a block of data previously created using getStateInformation()
void Oscillator::setStateInformation (const void *data, int sizeInBytes) {
    *mFrequency = juce::MemoryInputStream(data, sizeInBytes, false).readFloat();
}

} // namespace processing
} // namespace soundboard
