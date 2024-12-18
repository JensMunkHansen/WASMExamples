const Module = require('./example.cjs'); // Load the Emscripten module

Module.onRuntimeInitialized = function () {
    const { callMain } = Module;

    // Example arguments (simulate passing arguments to main)
    const args = ['example', 'arg1', 'arg2', 'arg3'];

    // Call the main function with arguments
    callMain(args);

    // Optional: Clean up if needed
    console.log('Program completed.');
};
