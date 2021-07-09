#include "linked_float_parameter.h"
#include "mixer.h"

namespace soundboard {
namespace processing {

// construct mixer
Mixer::Mixer() {
    // add parameters
    addParameter(new LinkedFloatParameter("gain", 0.0f, 1.0f, 0.0f, std::bind(&juce::dsp::Gain<float>::setGainLinear, &mGain, std::placeholders::_1)));
    addParameter(new LinkedFloatParameter("pan", -1.0f, 1.0f, 0.0f, std::bind(&juce::dsp::Panner<float>::setPan, &mPanner, std::placeholders::_1)));

    // init gain
    mGain.setGainLinear(0.0f);
    mGain.setRampDurationSeconds(0.05);

    // init panner
    mPanner.setRule(juce::dsp::PannerRule::squareRoot3dB);
    mPanner.setPan(0.0f);
}

// called before playback starts, to let the processor prepare itself
void Mixer::prepareToPlay(double sampleRate, int bufferSize) {
    juce::dsp::ProcessSpec spec { sampleRate, static_cast<juce::uint32>(bufferSize), 2 };
    mGain.prepare(spec);
    mPanner.prepare(spec);
}

// renders the next block
void Mixer::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages) {
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    mPanner.process(context); // before gain
    mGain.process(context);
}

// should reset any playing voices
void Mixer::reset() {
    mGain.reset();
    mPanner.reset();
}

} // namespace processing
} // namespace soundboard
