import { createSlice, PayloadAction } from "@reduxjs/toolkit";
import { AudioUnit, AudioUnitControlId, AudioUnitId, Session } from "backend/types";
import { AppThunk } from "./index";

// app state
interface AppState {
    editMode: boolean,
    session: Session | null,
}

// initial state
const initialState: AppState = {
    editMode: false,
    session: {
        channelStrips: [],
        name: "New Session",
    },
};

// slice
const appSlice = createSlice({
    name: "app",
    initialState,
    reducers: {
        // add channel strip
        addChannelStrip: (state) => {
            state.session?.channelStrips.push({ audioUnits: [] });
        },

        // insert audio unit
        insertAudioUnit: (state, { payload }: PayloadAction<{ audioUnit: AudioUnit, channelStrip: number, index: number }>) => {
            state.session?.channelStrips[payload.channelStrip].audioUnits.splice(payload.index, 0, payload.audioUnit);
        },

        // remove audio unit
        removeAudioUnit: (state, { payload }: PayloadAction<{ channelStrip: number, index: number }>) => {
            state.session?.channelStrips[payload.channelStrip].audioUnits.splice(payload.index, 1);
        },

        // remove channel strip
        removeChannelStrip: (state, { payload }: PayloadAction<{ index: number }>) => {
            state.session?.channelStrips.splice(payload.index, 1);
        },

        // set edit mode
        setEditMode: (state, { payload }: PayloadAction<boolean>) => {
            state.editMode = payload;
        },
    },
});

// inferred actions
export const { setEditMode } = appSlice.actions;

// TODO: action payload validation + error handling

// add channel strip
export const addChannelStrip = (): AppThunk => (dispatch, getState) => {
    dispatch(appSlice.actions.addChannelStrip());
    const channelStripIndex = getState().session!.channelStrips.length - 1;
    dispatch(insertAudioUnit("output", channelStripIndex, 0)); // add output, will update graph as well
};

// insert audio unit
export const insertAudioUnit = (id: AudioUnitId, channelStrip: number, index: number): AppThunk => (dispatch, getState) => {
    const ref = window.backend.addAudioUnit(id);
    const audioUnit: AudioUnit = { id, ref };
    dispatch(appSlice.actions.insertAudioUnit({ audioUnit, channelStrip, index }));
    window.backend.updateGraph(getState().session!);
};

// remove audio unit
export const removeAudioUnit = (channelStrip: number, index: number): AppThunk => (dispatch, getState) => {
    const { ref } = getState().session!.channelStrips[channelStrip].audioUnits[index];
    window.backend.removeAudioUnit(ref);
    dispatch(appSlice.actions.removeAudioUnit({ channelStrip, index }));
    window.backend.updateGraph(getState().session!);
};

// remove channel strip
export const removeChannelStrip = (index: number): AppThunk => (dispatch, getState) => {
    getState().session!.channelStrips[index].audioUnits.forEach((audioUnit) => {
        window.backend.removeAudioUnit(audioUnit.ref); // remove all audio units
    });
    dispatch(appSlice.actions.removeChannelStrip({ index }));
    window.backend.updateGraph(getState().session!);
};

// set parameter value
export const setParameterValue = (channelStrip: number, index: number, paramId: AudioUnitControlId, value: number): AppThunk => (dispatch, getState) => {
    const { ref } = getState().session!.channelStrips[channelStrip].audioUnits[index];
    window.backend.setParameterValue(ref, paramId, value);
};

export default appSlice.reducer;
