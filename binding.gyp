{
    "targets": [
        {
            "target_name": "backend",
            "sources": [
                "ext/juce/modules/juce_core/juce_core.mm",
                "ext/juce/modules/juce_events/juce_events.mm",
                "ext/juce/modules/juce_graphics/juce_graphics.mm",
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
                "JUCE_STANDALONE_APPLICATION=0",
            ],
            "xcode_settings": {
                "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
                "GCC_ENABLE_CPP_RTTI": "YES",
            },
        },
    ],
}
