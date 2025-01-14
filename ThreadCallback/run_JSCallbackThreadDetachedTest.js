import loadJSCallbackTestModule from './JSCallbackThreadDetachedTest.js';

(async () => {
    const options = { print: console.info, printErr: console.error };

    // Load the WebAssembly module
    const wasmModule = await loadJSCallbackTestModule(options);
    console.log("Module loaded");
    
    // Create a promise to wait for the callback
    const callbackPromise = new Promise((resolve) => {
        // Define the JavaScript function to handle notifications
        wasmModule.notifyCallback = (message) => {
            console.log("Callback received in JavaScript:", message);
            resolve(); // Resolve the promise when the callback is invoked
        };
    });

    console.log("Starting the pthread...");
    wasmModule.startPthread();

    // Wait for the callback to complete
    await callbackPromise;

    console.log("Pthread worker has completed.");
})();
