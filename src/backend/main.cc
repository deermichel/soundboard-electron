#include "audio_engine.h"
using namespace soundboard;

// standalone repl for development
int main(int argc, char *argv[]) {
    // init juce
    juce::ScopedJuceInitialiser_GUI juce;

    // init engine
    AudioEngine::instance().initialize();

    // add audio unit
    AudioEngine::instance().addAudioUnit("oscillator");

    // update session
    model::Session session = { .name = "New Session" };
    auto &channelStrip = session.channelStrips.emplace_back();
    AudioEngine::instance().updateGraph(session);

    std::cin.get();
    return 0;
}
