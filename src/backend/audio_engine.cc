#include "audio_engine.h"

namespace soundboard {
namespace backend {

// initialize engine (required before calling any other method)
void AudioEngine::initialize() {
    std::cout << "initializing audio engine";

    // init audio devices
    mDeviceManager.initialiseWithDefaultDevices(2, 2);
    mDeviceManager.addAudioCallback(&mAudioProcessorPlayer);
    auto deviceSetup = mDeviceManager.getAudioDeviceSetup();
    // Logger::debug("using audio output '%s' (%d channels) and input '%s' (%d channels) at sample rate %.2f with buffer size %d",
    //     deviceSetup.outputDeviceName.toUTF8(), deviceSetup.outputChannels.toInteger(),
    //     deviceSetup.inputDeviceName.toUTF8(), deviceSetup.inputChannels.toInteger(),
    //     deviceSetup.sampleRate, deviceSetup.bufferSize);

    // init audio graph
    mAudioGraph.enableAllBuses();
    mAudioGraph.clear();

    // add audio and midi io nodes
    mAudioGraphAudioInputNode = mAudioGraph.addNode(std::make_unique<juce::AudioProcessorGraph::AudioGraphIOProcessor>(juce::AudioProcessorGraph::AudioGraphIOProcessor::audioInputNode));
    mAudioGraphAudioOutputNode = mAudioGraph.addNode(std::make_unique<juce::AudioProcessorGraph::AudioGraphIOProcessor>(juce::AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode));
    // auto midiInputNode = mAudioGraph.addNode(std::make_unique<juce::AudioProcessorGraph::AudioGraphIOProcessor>(juce::AudioProcessorGraph::AudioGraphIOProcessor::midiInputNode));
    // auto midiOutputNode = mAudioGraph.addNode(std::make_unique<juce::AudioProcessorGraph::AudioGraphIOProcessor>(juce::AudioProcessorGraph::AudioGraphIOProcessor::midiOutputNode));

    // setup audio processor player
    mAudioProcessorPlayer.setProcessor(&mAudioGraph);
    mInitialized = true;
}

// --- private methods ---

AudioEngine::AudioEngine() {}
AudioEngine::~AudioEngine() {
    if (!mInitialized) return;

    // cleanup audio devices
    mDeviceManager.removeAudioCallback(&mAudioProcessorPlayer);
}

} // namespace backend
} // namespace soundboard
