#ifndef AUDIO_ENGINE_H
#define AUDIO_ENGINE_H

#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_utils/juce_audio_utils.h>

namespace soundboard {

// juce-powered audio engine, derives audio processor graph from session
class AudioEngine {
public:
    // add audio unit (returns unique ref)
    unsigned int addAudioUnit(const std::string &id);

    // initialize engine (required before calling any other method)
    void initialize();

    // remove processor
    void removeProcessor(unsigned int ref);

    // reset engine (to freshly initialized state, e.g. removes all processors)
    void reset();

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
