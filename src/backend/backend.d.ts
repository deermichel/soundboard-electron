// backend interface typings
interface Backend {
    addAudioUnit: (id: string) => number,
}

declare module "*backend.node" {
    const backend: Backend;
    export default backend;
}
