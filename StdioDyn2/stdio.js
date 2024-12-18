const Module = require('./example.cjs'); // Load the Emscripten module
let mainCalled = false; // Ensure this is a global flag

Module.onRuntimeInitialized = async function () {
    console.log("Module initialized.");

    // Check if main has already been called
    if (mainCalled) {
        console.log("Main already called. Exiting...");
        return;
    }

    mainCalled = true; // Mark main as called

    // Wrap main logic in an async function
    const runMain = async () => {
        console.log("Starting main...");
        try {
            // Call the WebAssembly main function
            console.log("Before calling callMain");
            await Module.callMain([]);
            console.log("After calling callMain");
        } catch (err) {
            console.error("Error in main:", err);
        }
        console.log("Main finished.");
    };

    // Call the async wrapper
    await runMain();

    console.log("Exiting...");
};
