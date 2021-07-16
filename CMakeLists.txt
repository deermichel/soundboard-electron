# project
cmake_minimum_required(VERSION 3.15)
project(soundboard VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 17)

# includes
include_directories(ext/juce/modules)
include_directories(ext)

# target
add_executable(soundboard)

# source
target_include_directories(soundboard PRIVATE src)
target_sources(soundboard
    PRIVATE
        "ext/juce/modules/juce_audio_basics/juce_audio_basics.mm"
        "ext/juce/modules/juce_audio_devices/juce_audio_devices.mm"
        "ext/juce/modules/juce_audio_formats/juce_audio_formats.mm"
        "ext/juce/modules/juce_audio_processors/juce_audio_processors.mm"
        "ext/juce/modules/juce_audio_utils/juce_audio_utils.mm"
        "ext/juce/modules/juce_core/juce_core.mm"
        "ext/juce/modules/juce_data_structures/juce_data_structures.mm"
        "ext/juce/modules/juce_dsp/juce_dsp.mm"
        "ext/juce/modules/juce_events/juce_events.mm"
        "ext/juce/modules/juce_graphics/juce_graphics.mm"
        "ext/juce/modules/juce_gui_basics/juce_gui_basics.mm"
        "src/backend/audio_engine.cc"
        "src/backend/main.cc"
        "src/backend/processing/layered_sampler.cc"
        "src/backend/processing/layered_sampler_sound.cc"
        "src/backend/processing/mixer.cc"
        "src/backend/processing/oscillator.cc"
        "src/backend/processing/piano.cc"
        "src/backend/processing/streaming_sample_loader.cc"
        "src/backend/processing/streaming_sampler_sound.cc"
        "src/backend/processing/streaming_sampler_voice.cc"
)

# defs
target_compile_definitions(soundboard
    PRIVATE
        DEBUG
        JSON_USE_IMPLICIT_CONVERSIONS=0
        JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED
        JUCE_MAC
        JUCE_MODULE_AVAILABLE_juce_gui_basics
        JUCE_STANDALONE_APPLICATION=0
        STANDALONE
)

# link libs
target_link_libraries(soundboard
    PRIVATE
    "-framework Accelerate"
    "-framework AppKit"
    "-framework AudioToolbox"
    "-framework CoreAudio"
    "-framework CoreAudioKit"
    "-framework CoreMIDI"
    "-framework IOKit"
    "-framework QuartzCore"
)
