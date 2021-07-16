{
    "targets": [
        {
            "target_name": "backend",
            "sources": [
                "ext/juce/modules/juce_audio_basics/juce_audio_basics.mm",
                "ext/juce/modules/juce_audio_devices/juce_audio_devices.mm",
                "ext/juce/modules/juce_audio_formats/juce_audio_formats.mm"
                "ext/juce/modules/juce_audio_processors/juce_audio_processors.mm",
                "ext/juce/modules/juce_audio_utils/juce_audio_utils.mm",
                "ext/juce/modules/juce_core/juce_core.mm",
                "ext/juce/modules/juce_data_structures/juce_data_structures.mm",
                "ext/juce/modules/juce_dsp/juce_dsp.mm",
                "ext/juce/modules/juce_events/juce_events.mm",
                "ext/juce/modules/juce_graphics/juce_graphics.mm",
                "ext/juce/modules/juce_gui_basics/juce_gui_basics.mm",
                "src/backend/audio_engine.cc",
                "src/backend/backend.cc",
                "src/backend/processing/layered_sampler.cc",
                "src/backend/processing/layered_sampler_sound.cc",
                "src/backend/processing/mixer.cc",
                "src/backend/processing/oscillator.cc",
                "src/backend/processing/piano.cc",
            ],
            "libraries": [
                # "<(module_root_dir)/ext/juce/prebuilt.a",
            ],
            "include_dirs": [
                "<!@(node -p \"require('node-addon-api').include\")",
                "ext/juce/modules",
                "ext",
            ],
            "defines": [
                "DEBUG",
                "JSON_USE_IMPLICIT_CONVERSIONS=0",
                "JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED",
                "JUCE_MAC",
                "JUCE_MODULE_AVAILABLE_juce_gui_basics",
                "JUCE_STANDALONE_APPLICATION=0",
            ],
            "xcode_settings": {
                "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
                "GCC_ENABLE_CPP_RTTI": "YES",
                "OTHER_CFLAGS": [ "-std=c++17" ],
                "OTHER_LDFLAGS": [ "-framework CoreAudioKit" ],
            },
        },
    ],
}
