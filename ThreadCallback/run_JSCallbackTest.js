import loadJSCallbackTestModule from './JSCallbackTest.js'

// Define the options object
const options = { print: console.info, printErr: console.error };

var wasmModule;

async function main() {
    try {
        // Load the WASM module
        wasmModule = await loadJSCallbackTestModule(options);
        console.log("Module loaded");

        function myCallback(message) {
            console.log("Callback received in JavaScript:", message);
        }

        // Set the callback in C++
        wasmModule.setCallback((msg) => myCallback(msg));

        // Start the pthread worker
        wasmModule.startWorker();
        console.log("Faking a worker has been started.");                
    } catch (error) {
        console.error("An error occurred:", error);
    }
}
await main();
