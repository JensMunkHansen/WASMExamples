// Empty for now
import loadSideModuleTest from './VTKABITest.js';

const options = { print: console.info, printErr: console.error };

var wasmModule

// Main function to load and use the WASM module
async function main() {
    try {
        // Load the WASM module
        wasmModule = await loadSideModuleTest(options);
        console.log("Module loaded");
        wasmModule.TestMe();
    } catch (error) {
        console.error("An error occurred:", error);
    }
}

// Execute the main function
await main();
