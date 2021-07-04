import { AnyAction, configureStore, getDefaultMiddleware, ThunkAction } from "@reduxjs/toolkit";
import { createLogger } from "redux-logger";
import app from "./app";

// root reducer
const rootReducer = app;

// configure logger
const logger = createLogger({
    predicate: (getState, action) => action.type !== "app/updateParameterValues", // filter frequent parameter updates
});

// store
const store = configureStore({
    middleware: process.env.NODE_ENV === "development" ? getDefaultMiddleware().concat(logger) : getDefaultMiddleware(),
    reducer: rootReducer,
});

// exports
export type RootState = ReturnType<typeof rootReducer>;
export type AppDispatch = typeof store.dispatch;
export type AppThunk = ThunkAction<void, RootState, unknown, AnyAction>;
export default store;
