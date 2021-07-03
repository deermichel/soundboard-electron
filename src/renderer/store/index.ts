import { AnyAction, configureStore, getDefaultMiddleware, ThunkAction } from "@reduxjs/toolkit";
import logger from "redux-logger";
import app from "./app";

// root reducer
const rootReducer = app;

// store
const store = configureStore({
    middleware: getDefaultMiddleware().concat(logger),
    reducer: rootReducer,
});

// exports
export type RootState = ReturnType<typeof rootReducer>;
export type AppDispatch = typeof store.dispatch;
export type AppThunk = ThunkAction<void, RootState, unknown, AnyAction>;
export default store;
