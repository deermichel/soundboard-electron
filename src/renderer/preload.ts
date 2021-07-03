import { Backend } from "backend/types";
import { contextBridge } from "electron";
import backend from "modules/backend.node";

// global typings
declare global {
    interface Window {
        backend: Backend,
    }
}

// expose backend to renderer
contextBridge.exposeInMainWorld("backend", backend);
