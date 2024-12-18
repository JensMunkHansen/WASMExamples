const Module = require('./example.cjs'); // Load the Emscripten module
let mainCalled = false;

Module.onRuntimeInitialized = async function () {
    console.log("Module initialized.");

    if (mainCalled) {
        console.log("Main already called. Exiting...");
        return;
    }

    mainCalled = true;    
    
    // Wrap main logic in an async function
    const runMain = async () => {
        console.log("Starting main...");
        try {
            // Call the WebAssembly main function
            await Module.callMain([]);
        } catch (err) {
            console.error("Error in main:", err);
        }
        console.log("Main finished.");
    };

    // Call the async wrapper
    await runMain();

    console.log("Exiting...");
};

