#ifndef BACKEND_AUDIO_ENGINE_H
#define BACKEND_AUDIO_ENGINE_H

#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_utils/juce_audio_utils.h>

namespace soundboard {
namespace backend {

// juce-powered audio engine, derives audio processor graph from session
class AudioEngine {
public:
    // initialize engine (required before calling any other method)
    void initialize();

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
};

} // namespace backend
} // namespace soundboard

#endif // BACKEND_AUDIO_ENGINE_H
