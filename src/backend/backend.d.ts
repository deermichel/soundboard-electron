// backend interface typings
interface Backend {
    initialize: () => void,
}

declare module "*backend.node" {
    const backend: Backend;
    export default backend;
}
