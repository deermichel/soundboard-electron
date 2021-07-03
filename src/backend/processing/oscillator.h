#ifndef PROCESSING_OSCILLATOR_H
#define PROCESSING_OSCILLATOR_H

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

namespace soundboard {
namespace processing {

// oscillator instrument processor
class Oscillator : public juce::AudioProcessor, juce::AudioProcessorParameter::Listener {
public:
    // audio unit id
    static constexpr const char *ID = "osc";

    // construct oscillator
    Oscillator();

    // --- overrides ---

    // returns true if the processor wants midi messages
    bool acceptsMidi() const override { return false; }

    // called by the host to rename a program
    void changeProgramName(int index, const juce::String &newName) override {}

    // creates the processor gui
    juce::AudioProcessorEditor* createEditor() override { return nullptr; }

    // returns the number of the currently active program
    int getCurrentProgram() override { return 0; }

    // returns the name of this processor
    const juce::String getName() const override { return "Oscillator"; };

    // returns the number of preset programs the processor supports
    int getNumPrograms() override { return 0; }

    // return the name of a given program
    const juce::String getProgramName(int index) override { return {}; }

    // the host will call this method when it wants to save the processor internal state
    void getStateInformation (juce::MemoryBlock &destData) override;

    // returns the length of the processor's tail, in seconds
    double getTailLengthSeconds() const override { return 0; }

    // return true if it can create an editor component
    bool hasEditor() const override { return false; }

    // parameter gesture changed callback
    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override {};

    // parameter value changed callback
    void parameterValueChanged(int parameterIndex, float newValue) override;

    // called before playback starts, to let the processor prepare itself
    void prepareToPlay(double sampleRate, int bufferSize) override;

    // returns true if the processor produces midi messages
    bool producesMidi() const override { return false; }

    // renders the next block
    void processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages) override;

    // called after playback has stopped, to let the object free up any resources it no longer needs
    void releaseResources() override {}

    // should reset any playing voices
    void reset() override;

    // called by the host to change the current program
    void setCurrentProgram(int index) override {}

    // this must restore the processor state from a block of data previously created using getStateInformation()
    void setStateInformation (const void *data, int sizeInBytes) override;

private:
    // frequency parameter
    juce::AudioParameterFloat *mFrequency;

    // gain parameter
    juce::AudioParameterFloat *mGain;

    // oscillator
    juce::dsp::Oscillator<float> mOscillator;

    // sine or saw parameter
    juce::AudioParameterBool *mSaw;

    // non copy, leak detection
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Oscillator)
};

} // namespace processing
} // namespace soundboard

#endif // PROCESSING_OSCILLATOR_H
