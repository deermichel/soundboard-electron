// id types
export type AudioUnitId = string; // TODO: derive allowed enum dynamically
export type AudioUnitControlId = string;
export type AudioUnitRef = number;

// audio unit control types
export enum AudioUnitControlType {
    Button = "button",
    Fader = "fader",
    Knob = "knob",
}

// audio unit control (e.g. fader, knob, button)
export interface AudioUnitControl {
    id: AudioUnitControlId,
    label: string,
    type: AudioUnitControlType,
    x: number,
    y: number,
}

// audio unit description (e.g. effect, instrument)
export interface AudioUnitDescription {
    controls: AudioUnitControl[],
    id: AudioUnitId,
    name: string,
    type: AudioUnitType,
}

// audio unit types
export enum AudioUnitType {
    Effect = "effect",
    Instrument = "instrument",
    Internal = "internal",
}

// audio unit (instance)
export interface AudioUnit {
    id: AudioUnitId,
    ref: AudioUnitRef,
}

// channel strip
export interface ChannelStrip {
    audioUnits: AudioUnit[],
}

// session
export interface Session {
    channelStrips: ChannelStrip[],
    name: string,
}

// backend interface
export interface Backend {
    addAudioUnit: (id: AudioUnitId) => AudioUnitRef,
    updateGraph: (session: Session) => void,
}
