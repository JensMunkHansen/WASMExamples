import createModule from './ThreadingCPP.js';

let wasmReady = null;
let wasmModule = null;

// Initialize the WASM module and resolve the promise
wasmReady = createModule().then((module) => {
    wasmModule = module;
});

self.onmessage = async function(event) {
    const { memory, wasmInstance } = event.data;

    // Wait for the WASM module to be ready
    if (!wasmModule) {
        await wasmReady;
    }
    console.log('Worker onmessage');
    // Access exported C function
    const myFunction = wasmModule.MyFunction; // Change to your function name

    // Example: Call function with some argument
    const result = myFunction(42);
    console.log(`Worker ${self.name}: Result = ${result}`);
    self.postMessage(result);    
};
