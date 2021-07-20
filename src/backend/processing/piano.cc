#include <juce_audio_formats/juce_audio_formats.h>
#include <nlohmann/json.h>
#include "streaming_sampler_voice.h"
#include "streaming_sampler.h"
#include "piano.h"

#define DEBUG_DISK_USAGE 0

namespace soundboard {
namespace processing {

// construct piano
Piano::Piano() :
    mThreadPool(std::make_unique<juce::ThreadPool>()) {

    // load samples
    juce::BigInteger midiNotes;
    midiNotes.setRange(0, 127, true);
    mSynth.addSound(new StreamingSamplerSound(juce::File::getCurrentWorkingDirectory().getChildFile("samples/RR1 C3 359.wav"), midiNotes, 48));

    // add voices
    for (int i = 0; i < 128; i++) {
        mSynth.addVoice(new StreamingSamplerVoice(mThreadPool.get()));
    }
}

// deconstructor -> clearSounds, clearVoices

// called before playback starts, to let the processor prepare itself
void Piano::prepareToPlay(double sampleRate, int bufferSize) {
    mSynth.setCurrentPlaybackSampleRate(sampleRate);

    // prepare voices (resize buffers)
    for (int i = 0; i < mSynth.getNumVoices(); i++) {
        StreamingSamplerVoice *voice = dynamic_cast<StreamingSamplerVoice*>(mSynth.getVoice(i));
        voice->prepareToPlay(sampleRate, bufferSize);
        voice->setLoaderBufferSize(bufferSize * 32); // load new data every 32 blocks
    }

    // update preload buffers
    for (int i = 0; i < mSynth.getNumSounds(); i++) {
        StreamingSamplerSound *sound = dynamic_cast<StreamingSamplerSound*>(mSynth.getSound(i).get());
        sound->setPreloadSize(juce::jmax(PRELOAD_BUFFER_SIZE, bufferSize * 32));
    }
}

// renders the next block
void Piano::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages) {
    buffer.clear();
    mSynth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    // disk usage
#if DEBUG_DISK_USAGE
    double usage = 0.0;
    for (int i = 0; i < mSynth.getNumVoices(); i++) {
        usage += dynamic_cast<StreamingSamplerVoice*>(mSynth.getVoice(i))->getDiskUsage();
    }
    DBG("Disk usage: " + juce::String(usage, 3));
#endif
}

} // namespace processing
} // namespace soundboard
