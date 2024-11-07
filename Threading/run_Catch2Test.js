// Import the function from wasmLoader.js
import loadCatch2TestModule from './wasmlibrary.js';

// Define the options object
const options = { print: console.info, printErr: console.error };

var wasmModule
let nErrors = 0;
// Main function to load and use the WASM module
async function main() {
    try {
        // Load the WASM module
        wasmModule = await loadCatch2TestModule(options);
        nErrors = wasmModule._DoWork();
    } catch (error) {
	nErrors++;
        console.error("An error occurred:", error);
    }
}

// Execute the main function
await main();

// TODO: Check references

// Can be captured with $?
process.exit(nErrors == 0 ? 0 : 1);
