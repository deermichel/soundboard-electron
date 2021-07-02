// id types
export type AudioUnitId = string;
export type AudioUnitControlId = string;

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

// channel strip
export interface ChannelStrip {
    // audioUnits: int[],
}

// session
export interface Session {
    channelStrips: ChannelStrip[],
    name: string,
}
