import loadJSCallbackTestModule from './JSCallbackThreadJoinableTest.js';

(async () => {
    const options = { print: console.info, printErr: console.error };

    const wasmModule = await loadJSCallbackTestModule(options);
    console.log("Module loaded");

    // Define the JavaScript function to handle notifications
    wasmModule.notifyCallback = (message) => {
        console.log("Callback received in JavaScript:", message);
        // Exit the process explicitly to prevent hanging
        //process.exit(0);
    };

    console.log("Starting the pthread...");
    await wasmModule.startPthread();
    console.log("Pthread worker has completed.");
})();
