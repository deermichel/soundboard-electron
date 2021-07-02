import { createSlice, PayloadAction } from "@reduxjs/toolkit";

// app state
interface AppState {
    editMode: boolean,
}

// initial state
const initialState: AppState = {
    editMode: false,
};

// slice
const appSlice = createSlice({
    name: "app",
    initialState,
    reducers: {
        // set edit mode
        setEditMode: (state, { payload }: PayloadAction<boolean>) => {
            state.editMode = payload;
        },
    },
});

export const { setEditMode } = appSlice.actions;
export default appSlice.reducer;
