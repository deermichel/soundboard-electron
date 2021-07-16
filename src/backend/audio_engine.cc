#include "audio_engine.h"
#include "processing/mixer.h"
#include "processing/oscillator.h"
#include "processing/piano.h"

namespace soundboard {

// add audio unit (returns unique ref)
unsigned int AudioEngine::addAudioUnit(const std::string &id) {
    if (!mInitialized) throw std::logic_error("audio engine is not initialized");

    // create processor instance
    std::unique_ptr<juce::AudioProcessor> instance; // TODO: use map & register instead of hardcoding?
    if (id == processing::Mixer::ID) instance = std::make_unique<processing::Mixer>();
    if (id == processing::Oscillator::ID) instance = std::make_unique<processing::Oscillator>();
    if (id == processing::Piano::ID) instance = std::make_unique<processing::Piano>();
    if (!instance) throw std::logic_error("invalid audio unit id");

    // add processor
    return addInternalProcessor(std::move(instance));
}

// return parameter values mapped by audio units refs
std::map<unsigned int, std::vector<model::ParameterValue>> AudioEngine::getParameterValues() const {
    if (!mInitialized) throw std::logic_error("audio engine is not initialized");

    // iterate audio graph
    std::map<unsigned int, std::vector<model::ParameterValue>> valuesByRef;
    for (const auto &node : mAudioGraph.getNodes()) {
        // cast and convert parameters
        std::vector<model::ParameterValue> values;
        for (const auto &param : node->getProcessor()->getParameters()) {
            // TODO: move cast to a ParameterValue constructor
            auto paramWithId = static_cast<juce::AudioProcessorParameterWithID*>(param);
            values.push_back({ .id = paramWithId->paramID.toStdString(), .value = paramWithId->getValue() });
        }
        valuesByRef[node->nodeID.uid] = values;
    }
    return valuesByRef;
}

// initialize engine (required before calling any other method)
void AudioEngine::initialize() {
    if (mInitialized) return; // already initialized
    std::cerr << "initializing audio engine" << std::endl;

    // init audio devices
    juce::AudioDeviceManager::AudioDeviceSetup deviceSetup = {
        .sampleRate = 44100,
        .bufferSize = 64,
    };
    mDeviceManager.initialise(2, 2, nullptr, true, "", &deviceSetup);
    mDeviceManager.addAudioCallback(&mAudioProcessorPlayer);
    deviceSetup = mDeviceManager.getAudioDeviceSetup();
    fprintf(stderr, "using audio output '%s' (%d channels) and input '%s' (%d channels) at sample rate %.2f with buffer size %d\n",
        deviceSetup.outputDeviceName.toUTF8(), deviceSetup.outputChannels.toInteger(),
        deviceSetup.inputDeviceName.toUTF8(), deviceSetup.inputChannels.toInteger(),
        deviceSetup.sampleRate, deviceSetup.bufferSize);

    // init midi devices
    for (const auto &device : juce::MidiInput::getAvailableDevices()) { // TODO: must be called on message thread
        mDeviceManager.setMidiInputDeviceEnabled(device.identifier, true);
    }
    mDeviceManager.addMidiInputDeviceCallback("", &mAudioProcessorPlayer);

    // setup audio processor player
    mAudioProcessorPlayer.setProcessor(&mAudioGraph);
    mInitialized = true;
    fprintf(stderr, "initialization done\n");

    // init audio graph
    reset();
}

// remove audio unit
void AudioEngine::removeAudioUnit(unsigned int ref) {
    if (!mInitialized) throw std::logic_error("audio engine is not initialized");
    mAudioGraph.removeNode(juce::AudioProcessorGraph::NodeID(ref));
}

// reset engine to freshly initialized state (e.g. removes all audio units)
void AudioEngine::reset() {
    if (!mInitialized) throw std::logic_error("audio engine is not initialized");

    // clear graph
    mAudioGraph.clear();
    mAudioGraph.enableAllBuses();

    // add audio and midi io nodes
    mAudioGraphAudioInputNode = mAudioGraph.addNode(std::make_unique<juce::AudioProcessorGraph::AudioGraphIOProcessor>(juce::AudioProcessorGraph::AudioGraphIOProcessor::audioInputNode));
    mAudioGraphAudioOutputNode = mAudioGraph.addNode(std::make_unique<juce::AudioProcessorGraph::AudioGraphIOProcessor>(juce::AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode));
    mAudioGraphMidiInputNode = mAudioGraph.addNode(std::make_unique<juce::AudioProcessorGraph::AudioGraphIOProcessor>(juce::AudioProcessorGraph::AudioGraphIOProcessor::midiInputNode));
    mAudioGraphMidiOutputNode = mAudioGraph.addNode(std::make_unique<juce::AudioProcessorGraph::AudioGraphIOProcessor>(juce::AudioProcessorGraph::AudioGraphIOProcessor::midiOutputNode));
}

// set audio unit parameter value
std::vector<model::ParameterValue> AudioEngine::setParameterValue(unsigned int ref, const std::string &paramId, float value) {
    if (!mInitialized) throw std::logic_error("audio engine is not initialized");
    auto node = mAudioGraph.getNodeForId(juce::AudioProcessorGraph::NodeID(ref));
    if (!node) throw std::logic_error("invalid node id");
    auto parameters = node->getProcessor()->getParameters();

    // iterate and convert params
    std::vector<model::ParameterValue> values;
    bool paramSet = false;
    for (const auto &param : parameters) {
        auto paramWithId = static_cast<juce::AudioProcessorParameterWithID*>(param);

        // set param
        if (paramWithId->paramID.toStdString() == paramId) {
            paramWithId->setValue(value);
            paramSet = true;
        }

        // store (updated) params
        values.push_back({ .id = paramWithId->paramID.toStdString(), .value = paramWithId->getValue() });
    }
    if (!paramSet) throw std::logic_error("invalid parameter id"); // param with given id not found
    return values;
}

// update audio graph connections via session
void AudioEngine::updateGraph(const model::Session &session) {
    auto channelStrips = session.channelStrips;
    fprintf(stderr, "updating audio graph of session '%s' (%d channel strips)\n", session.name.c_str(), channelStrips.size());

    // clear all connections
    for (const auto &node : mAudioGraph.getNodes()) {
        mAudioGraph.disconnectNode(node->nodeID);
    }

    // iterate channel strips
    for (unsigned int index = 0; index < channelStrips.size(); index++) {
        auto audioUnits = channelStrips[index].audioUnits;
        fprintf(stderr, "channel strip %d: %d audio units\n", index, audioUnits.size());

        // connect midi input
        if (audioUnits.size() > 0) {
            const auto firstNodeId = juce::AudioProcessorGraph::NodeID(audioUnits.front().ref);
            bool result = mAudioGraph.addConnection({ { mAudioGraphMidiInputNode->nodeID, juce::AudioProcessorGraph::midiChannelIndex }, { firstNodeId, juce::AudioProcessorGraph::midiChannelIndex } });
            fprintf(stderr, "add midi input connection to %d: %s\n", firstNodeId.uid, result ? "success" : "failed");
        }

        // connect audio unit chain
        auto lastNodeId = juce::AudioProcessorGraph::NodeID(0); // mAudioGraphAudioInputNode->nodeID; // TODO: handle audio input
        for (const auto &audioUnit : audioUnits) {
            auto nodeId = juce::AudioProcessorGraph::NodeID(audioUnit.ref);

            // create new connections
            for (int channel = 0; channel < 2; channel++) { // TODO: always stereo?
                bool result = mAudioGraph.addConnection({ { lastNodeId, channel }, { nodeId, channel } });
                fprintf(stderr, "add connection %d to %d: %s\n", lastNodeId.uid, nodeId.uid, result ? "success" : "failed");
            }
            lastNodeId = nodeId;
        }

        // connect to output
        for (int channel = 0; channel < 2; channel++) { // TODO: always stereo?
            bool result = mAudioGraph.addConnection({ { lastNodeId, channel }, { mAudioGraphAudioOutputNode->nodeID, channel } });
            fprintf(stderr, "add connection %d to output: %s\n", lastNodeId.uid, result ? "success" : "failed");
        }
    }

    fprintf(stderr, "there are now %d nodes and %d connections\n", mAudioGraph.getNumNodes(), mAudioGraph.getConnections().size());
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
    processor->enableAllBuses();
    auto node = mAudioGraph.addNode(std::move(processor));
    return node->nodeID.uid;
}

} // namespace soundboard
