import { createSlice } from "@reduxjs/toolkit";

// app state
interface AppState {}

// initial state
const initialState: AppState = {};

// slice
const appSlice = createSlice({
    name: "app",
    initialState,
    reducers: {},
});

export default appSlice.reducer;
