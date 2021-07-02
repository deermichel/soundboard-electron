import { configureStore, getDefaultMiddleware } from "@reduxjs/toolkit";
import app from "./app";

// root reducer
const rootReducer = app;

// store
const store = configureStore({
    middleware: [...getDefaultMiddleware()],
    reducer: rootReducer,
});

// exports
export type RootState = ReturnType<typeof rootReducer>;
export type AppDispatch = typeof store.dispatch;
export default store;
