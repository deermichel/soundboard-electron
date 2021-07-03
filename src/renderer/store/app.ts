import { createSlice, PayloadAction } from "@reduxjs/toolkit";
import { AudioUnit, AudioUnitId, Session } from "backend/types";
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
    const channelStripIndex = (getState().session?.channelStrips.length || 0) - 1;
    dispatch(insertAudioUnit("output", channelStripIndex, 0)); // add output, will update graph as well
};

// insert audio unit
export const insertAudioUnit = (id: AudioUnitId, channelStrip: number, index: number): AppThunk => (dispatch) => {
    const ref = window.backend.addAudioUnit(id);
    const audioUnit: AudioUnit = { id, ref };
    dispatch(appSlice.actions.insertAudioUnit({ audioUnit, channelStrip, index }));
    // TODO: update graph
};

export default appSlice.reducer;
