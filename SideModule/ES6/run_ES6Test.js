import loadES6Test from './ES6Test.js';

const options = { print: console.info, printErr: console.error };

var wasmModule

// Main function to load and use the WASM module
async function main() {
    try {
        // Load the WASM module
        wasmModule = await loadES6Test(options);
        wasmModule.AddOne(1);
    } catch (error) {
        console.error("An error occurred:", error);
    }
}

// Execute the main function
await main();

