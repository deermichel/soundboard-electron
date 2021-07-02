{
    "targets": [
        {
            "target_name": "backend",
            "sources": [
                "ext/juce/modules/juce_audio_basics/juce_audio_basics.mm",
                "ext/juce/modules/juce_audio_devices/juce_audio_devices.mm",
                "ext/juce/modules/juce_audio_processors/juce_audio_processors.mm",
                "ext/juce/modules/juce_audio_utils/juce_audio_utils.mm",
                "ext/juce/modules/juce_core/juce_core.mm",
                "ext/juce/modules/juce_data_structures/juce_data_structures.mm",
                "ext/juce/modules/juce_events/juce_events.mm",
                "ext/juce/modules/juce_graphics/juce_graphics.mm",
                "ext/juce/modules/juce_gui_basics/juce_gui_basics.mm",
                "src/backend/audio_engine.cc",
                "src/backend/backend.cc",
            ],
            "libraries": [],
            "include_dirs": [
                "<!@(node -p \"require('node-addon-api').include\")",
                "ext/juce/modules",
            ],
            "defines": [
                "DEBUG",
                "JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED",
                "JUCE_MAC",
                "JUCE_MODULE_AVAILABLE_juce_gui_basics",
                "JUCE_STANDALONE_APPLICATION=0",
            ],
            "xcode_settings": {
                "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
                "GCC_ENABLE_CPP_RTTI": "YES",
                "OTHER_LDFLAGS": [ "-framework CoreAudioKit" ],
            },
        },
    ],
}
