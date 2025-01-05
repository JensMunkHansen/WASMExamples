import loadES6CallbackTestModule from './ES6CallbackTest.js'

// Define the options object
const options = { print: console.info, printErr: console.error };

var wasmModule;

async function main() {
    try {
        // Load the WASM module
        wasmModule = await loadES6CallbackTestModule(options);
        console.log("Module loaded");

        function myCallback(value) {
            console.log("Callback called with value:", value);
            // Ensure the return value is an integer
            return value * 2;
        }       
        //wasmModule.setCallback(myCallback.bind(null));
        wasmModule.setCallback(myCallback);
        wasmModule.startPthread();
        
    } catch (error) {
        console.error("An error occurred:", error);
    }
}
await main();
