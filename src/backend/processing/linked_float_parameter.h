#ifndef PROCESSING_LINKED_FLOAT_PARAMETER_H
#define PROCESSING_LINKED_FLOAT_PARAMETER_H

#include <juce_audio_processors/juce_audio_processors.h>

namespace soundboard {
namespace processing {

// float parameter wrapper with value changed callback
class LinkedFloatParameter : public juce::AudioParameterFloat {
public:
    // value changed callback function type
    typedef std::function<void(float)> ValueChangedCallback;

    // constructor
    LinkedFloatParameter(const std::string &paramId, float minValue, float maxValue, float defaultValue, const ValueChangedCallback &valueChanged) :
        AudioParameterFloat(paramId, paramId, minValue, maxValue, defaultValue),
        mValueChanged(valueChanged) {}

    // --- overrides ---

    // called when parameter value changes
    void valueChanged(float newValue) override { mValueChanged(newValue); };

private:
    // value changed callback
    const ValueChangedCallback mValueChanged;
};

} // namespace processing
} // namespace soundboard

#endif // PROCESSING_LINKED_FLOAT_PARAMETER_H
