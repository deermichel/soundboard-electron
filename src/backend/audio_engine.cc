#include "audio_engine.h"
#include "processing/equalizer.h"
#include "processing/oscillator.h"
#include "processing/output.h"

namespace soundboard {

// add audio unit (returns unique ref)
unsigned int AudioEngine::addAudioUnit(const std::string &id) {
    if (!mInitialized) throw std::logic_error("audio engine is not initialized");

    // create processor instance
    std::unique_ptr<juce::AudioProcessor> instance;
    if (id == processing::Oscillator::ID) instance = std::make_unique<processing::Oscillator>(); // TODO: use map & register instead of hardcoding?
    if (id == processing::Equalizer::ID) instance = std::make_unique<processing::Equalizer>();
    if (id == processing::Output::ID) instance = std::make_unique<processing::Output>();
    if (!instance) throw std::logic_error("invalid audio unit id");

    // add processor
    return addInternalProcessor(std::move(instance));
}

// update audio graph connections via session
void AudioEngine::updateGraph(const Session &session) {
    auto channelStrips = session.channelStrips;
    fprintf(stderr, "updating audio graph of session '%s' (%d channel strips)\n", session.name.c_str(), channelStrips.size());

    // // clear all connections
    for (const auto &node : mAudioGraph.getNodes()) {
        mAudioGraph.disconnectNode(node->nodeID);
    }

    // iterate channel strips
    for (unsigned int index = 0; index < channelStrips.size(); index++) {
        auto audioUnits = channelStrips[index].audioUnits;
        fprintf(stderr, "channel strip %d: %d audio units\n", index, audioUnits.size());

        // connect audio unit chain
        // auto lastNodeId = mAudioGraphAudioInputNode->nodeID;
        // for (const auto &audioUnit : audioUnits) {
        //     auto nodeId = juce::AudioProcessorGraph::NodeID(audioUnit.ref);

        //     // create new connections
        //     for (int channel = 0; channel < 2; channel++) { // TODO: always stereo?
        //         bool result = mAudioGraph.addConnection({ { lastNodeId, channel }, { nodeId, channel } });
        //         fprintf(stderr, "add connection %d to %d: %s\n", lastNodeId.uid, nodeId.uid, result ? "success" : "failed");
        //     }
        //     lastNodeId = nodeId;
        // }

        // // connect to output
        // for (int channel = 0; channel < 2; channel++) { // TODO: always stereo?
        //     bool result = mAudioGraph.addConnection({ { lastNodeId, channel }, { mAudioGraphAudioOutputNode->nodeID, channel } });
        //     fprintf(stderr, "add connection %d to output: %s\n", lastNodeId.uid, result ? "success" : "failed");
        // }
    }

    fprintf(stderr, "there are now %d nodes and %d connections\n", mAudioGraph.getNumNodes(), mAudioGraph.getConnections().size());
}

// initialize engine (required before calling any other method)
void AudioEngine::initialize() {
    if (mInitialized) return; // already initialized
    std::cerr << "initializing audio engine" << std::endl;

    // init audio devices
    mDeviceManager.initialiseWithDefaultDevices(2, 2);
    mDeviceManager.addAudioCallback(&mAudioProcessorPlayer);
    auto deviceSetup = mDeviceManager.getAudioDeviceSetup();
    fprintf(stderr, "using audio output '%s' (%d channels) and input '%s' (%d channels) at sample rate %.2f with buffer size %d\n",
        deviceSetup.outputDeviceName.toUTF8(), deviceSetup.outputChannels.toInteger(),
        deviceSetup.inputDeviceName.toUTF8(), deviceSetup.inputChannels.toInteger(),
        deviceSetup.sampleRate, deviceSetup.bufferSize);

    // setup audio processor player
    mAudioProcessorPlayer.setProcessor(&mAudioGraph);
    mInitialized = true;

    // init audio graph
    reset();
}

// remove processor
void AudioEngine::removeProcessor(unsigned int ref) {
    if (!mInitialized) throw std::logic_error("audio engine is not initialized");
    mAudioGraph.removeNode(juce::AudioProcessorGraph::NodeID(ref));
}

// reset engine to freshly initialized state (e.g. removes all processors)
void AudioEngine::reset() {
    if (!mInitialized) throw std::logic_error("audio engine is not initialized");

    // clear graph
    mAudioGraph.clear();
    mAudioGraph.enableAllBuses();

    // add audio and midi io nodes
    mAudioGraphAudioInputNode = mAudioGraph.addNode(std::make_unique<juce::AudioProcessorGraph::AudioGraphIOProcessor>(juce::AudioProcessorGraph::AudioGraphIOProcessor::audioInputNode));
    mAudioGraphAudioOutputNode = mAudioGraph.addNode(std::make_unique<juce::AudioProcessorGraph::AudioGraphIOProcessor>(juce::AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode));
    // auto midiInputNode = mAudioGraph.addNode(std::make_unique<juce::AudioProcessorGraph::AudioGraphIOProcessor>(juce::AudioProcessorGraph::AudioGraphIOProcessor::midiInputNode));
    // auto midiOutputNode = mAudioGraph.addNode(std::make_unique<juce::AudioProcessorGraph::AudioGraphIOProcessor>(juce::AudioProcessorGraph::AudioGraphIOProcessor::midiOutputNode));
}

// --- private methods ---

AudioEngine::AudioEngine() {}
AudioEngine::~AudioEngine() {
    if (!mInitialized) return;

    // cleanup audio devices
    mDeviceManager.removeAudioCallback(&mAudioProcessorPlayer);
}

// add internal processor (returns unique ref)
unsigned int AudioEngine::addInternalProcessor(std::unique_ptr<juce::AudioProcessor> processor) {
    if (!mInitialized) throw std::logic_error("audio engine is not initialized");
    auto node = mAudioGraph.addNode(std::move(processor));
    node->getProcessor()->enableAllBuses();
    return node->nodeID.uid;
}

} // namespace soundboard
