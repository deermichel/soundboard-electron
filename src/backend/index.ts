import { AudioUnitId, AudioUnitControlType, AudioUnitDescription, AudioUnitType } from "./types";

// audio unit descriptions
export const availableAudioUnits: { [id in AudioUnitId]: AudioUnitDescription } = {
    // [effect] equalizer
    eq: {
        controls: [
            { id: "low", label: "Lows", type: AudioUnitControlType.Knob, x: 0, y: 0 },
            { id: "mid", label: "Mids", type: AudioUnitControlType.Knob, x: 1, y: 0 },
            { id: "high", label: "Highs", type: AudioUnitControlType.Knob, x: 2, y: 0 },
        ],
        id: "eq",
        name: "Equalizer",
        type: AudioUnitType.Effect,
    },

    // [instrument] oscillator
    osc: {
        controls: [
            { id: "freq", label: "Frequency", type: AudioUnitControlType.Knob, x: 0, y: 0 },
            { id: "saw", label: "Sine / Saw", type: AudioUnitControlType.Button, x: 1, y: 0 },
            { id: "gain", label: "Gain", type: AudioUnitControlType.Knob, x: 2, y: 0 },
        ],
        id: "osc",
        name: "Oscillator",
        type: AudioUnitType.Instrument,
    },

    // [internal] mixer
    mixer: {
        controls: [
            { id: "gain", label: "Gain", type: AudioUnitControlType.Knob, x: 2, y: 0 },
            { id: "pan", label: "Pan", type: AudioUnitControlType.Knob, x: 1, y: 0 },
        ],
        id: "mixer",
        name: "Mixer",
        type: AudioUnitType.Internal,
    },
};
