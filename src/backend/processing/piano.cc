#include <juce_audio_formats/juce_audio_formats.h>
#include <nlohmann/json.h>
#include "piano.h"
#include "sampler/papagei_sound.h"
#include "sampler/papagei_voice.h"

namespace soundboard {
namespace processing {

struct Sample {
    std::string path;
    unsigned int rootNote;
    unsigned int loNote;
    unsigned int hiNote;
    unsigned int loVel;
    unsigned int hiVel;
    unsigned int start;
    unsigned int end;
};
struct Group {
    std::vector<Sample> samples;
};
struct Metadata {
    std::vector<Group> groups;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Sample, path, rootNote, loNote, hiNote, loVel, hiVel, start, end);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Group, samples);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Metadata, groups);

// construct piano
Piano::Piano() :
    mThreadPool(std::make_unique<juce::ThreadPool>()) {

    // parse metadata
    const std::string rawMetadata = juce::File::getCurrentWorkingDirectory().getChildFile("piano.json").loadFileAsString().toStdString();
    const Metadata metadata = nlohmann::json::parse(rawMetadata).get<Metadata>();

    // fill sample banks
    juce::WavAudioFormat wavFormat;
    for (const auto &group : metadata.groups) {
        // const auto bank = mSampler.addSoundBank();
        for (const auto &sample : group.samples) {
            // open file
            const auto sampleFile = juce::File::getCurrentWorkingDirectory().getChildFile(sample.path);
            // const auto audioReader = wavFormat.createReaderFor(sampleFile.createInputStream().release(), true);

            // read meta
            juce::BigInteger midiNotes, velocities;
            midiNotes.setRange(sample.loNote, sample.hiNote - sample.loNote + 1, true);
            velocities.setRange(sample.loVel, sample.hiVel - sample.loVel + 1, true);
            // const double maxSampleLengthSeconds = sample.end / 44100.0 + 1; // TODO: use actual sample rate

            // add sound
            const auto sound = new PapageiSound(sampleFile, midiNotes, velocities, sample.rootNote);
            sound->loadEntireSample();
            mSampler.addSound(sound);
            printf("added sound %s\n", sampleFile.getFileName().toStdString().c_str());
            // const auto sound = new LayeredSamplerSound(sample.path, *audioReader, midiNotes, velocities, sample.rootNote, 0.001, 0.1, maxSampleLengthSeconds);
            // mSampler.addSoundToBank(sound, bank);
        }
    }

    // init voices
    for (unsigned int i = 0; i < 64; i++) {
        mSampler.addVoice(new PapageiVoice(mThreadPool.get()));
    }
}

// called before playback starts, to let the processor prepare itself
void Piano::prepareToPlay(double sampleRate, int bufferSize) {
    mSampler.setCurrentPlaybackSampleRate(sampleRate);

    // prepare voices (resize buffers)
    for (int i = 0; i < mSampler.getNumVoices(); i++) {
        PapageiVoice *voice = dynamic_cast<PapageiVoice*>(mSampler.getVoice(i));
        voice->prepareToPlay(sampleRate, bufferSize);
    }
}

// renders the next block
void Piano::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages) {
    buffer.clear();
    mSampler.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

// should reset any playing voices
void Piano::reset() {
}

} // namespace processing
} // namespace soundboard
