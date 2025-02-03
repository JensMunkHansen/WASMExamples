import createModule from './add.mjs';

let wasmReady = null;
let wasmModule = null;

// Initialize the WASM module and resolve the promise
wasmReady = createModule().then((module) => {
    wasmModule = module;
});

// Handle messages from the main thread
self.onmessage = async function (event) {
    const { a, b } = event.data;

    // Wait for the WASM module to be ready
    if (!wasmModule) {
        await wasmReady;
    }

    // Get the 'add' function from the WASM module
    const add = wasmModule.cwrap('add', 'number', ['number', 'number']);

    // Execute the function and post the result back to the main thread
    const result = add(a, b);
    self.postMessage(result);
};
