import loadES6Module from './ES6.js'

// Define the options object
const options = { print: console.info, printErr: console.error };

var wasmModule;

async function main() {
    try {
        // Load the WASM module
        wasmModule = await loadES6Module(options);
        console.log("Module loaded");
        wasmModule._HelloWorld();
    } catch (error) {
        console.error("An error occurred:", error);
    }
}
await main();
