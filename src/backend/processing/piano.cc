#include <juce_audio_formats/juce_audio_formats.h>
#include <nlohmann/json.h>
#include "layered_sampler_sound.h"
#include "piano.h"

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
Piano::Piano() {
    // parse metadata
    const std::string rawMetadata = juce::File::getCurrentWorkingDirectory().getChildFile("piano.json").loadFileAsString().toStdString();
    const Metadata metadata = nlohmann::json::parse(rawMetadata).get<Metadata>();

    // fill sample banks
    juce::WavAudioFormat wavFormat;
    for (const auto &group : metadata.groups) {
        const auto bank = mSampler.addSoundBank();
        for (const auto &sample : group.samples) {
            // open file
            const auto sampleFile = juce::File::getCurrentWorkingDirectory().getChildFile(sample.path);
            const auto audioReader = wavFormat.createReaderFor(sampleFile.createInputStream().release(), true);

            // read meta
            juce::BigInteger midiNotes;
            midiNotes.setRange(sample.loNote, sample.hiNote, true);
            const auto velocityRange = juce::Range<float>(sample.loVel / 127.0f, sample.hiVel / 127.0f);
            const double maxSampleLengthSeconds = sample.end / 44100.0; // TODO: use actual sample rate

            // add sound
            const auto sound = new LayeredSamplerSound(sample.path, *audioReader, midiNotes, velocityRange, sample.rootNote, 0.001, 0.1, maxSampleLengthSeconds);
            mSampler.addSoundToBank(sound, bank);
        }
    }

    // init voices
    reset();
}

// called before playback starts, to let the processor prepare itself
void Piano::prepareToPlay(double sampleRate, int bufferSize) {
    mSampler.setCurrentPlaybackSampleRate(sampleRate);
}

// renders the next block
void Piano::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages) {
    buffer.clear();
    mSampler.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

// should reset any playing voices
void Piano::reset() {
    mSampler.clearVoices();
    for (unsigned int i = 0; i < 8; i++) {
        mSampler.addVoice(new juce::SamplerVoice());
    }
}

} // namespace processing
} // namespace soundboard
