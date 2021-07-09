#ifndef AUDIO_ENGINE_H
#define AUDIO_ENGINE_H

#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include "model/parameter_value.h"
#include "model/session.h"

namespace soundboard {

// juce-powered audio engine, derives audio processor graph from session
class AudioEngine {
public:
    // add audio unit (returns unique ref)
    unsigned int addAudioUnit(const std::string &id);

    // return parameter values mapped by audio units refs
    std::map<unsigned int, std::vector<model::ParameterValue>> getParameterValues() const;

    // initialize engine (required before calling any other method)
    void initialize();

    // remove audio unit
    void removeAudioUnit(unsigned int ref);

    // reset engine (to freshly initialized state, e.g. removes all audio units)
    void reset();

    // set audio unit parameter value
    std::vector<model::ParameterValue> setParameterValue(unsigned int ref, const std::string &paramId, float value);

    // update audio graph connections via session
    void updateGraph(const model::Session &session);

    // return singleton instance
    static AudioEngine& instance() {
        static AudioEngine instance;
        return instance;
    }

private:
    AudioEngine();
    ~AudioEngine();

    // audio graph (= main audio processor)
    juce::AudioProcessorGraph mAudioGraph;

    // audio graph audio input node
    juce::AudioProcessorGraph::Node::Ptr mAudioGraphAudioInputNode;

    // audio graph audio output node
    juce::AudioProcessorGraph::Node::Ptr mAudioGraphAudioOutputNode;

    // audio graph midi input node
    juce::AudioProcessorGraph::Node::Ptr mAudioGraphMidiInputNode;

    // audio graph midi output node
    juce::AudioProcessorGraph::Node::Ptr mAudioGraphMidiOutputNode;

    // audio processor player
    juce::AudioProcessorPlayer mAudioProcessorPlayer;

    // audio device manager
    juce::AudioDeviceManager mDeviceManager;

    // whether the engine was initialized
    bool mInitialized = false;

    // add internal processor (returns unique ref)
    unsigned int addInternalProcessor(std::unique_ptr<juce::AudioProcessor> processor);
};

} // namespace soundboard

#endif // AUDIO_ENGINE_H
