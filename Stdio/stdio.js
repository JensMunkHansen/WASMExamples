const Module = require('./example.js'); // Load the Emscripten module

Module.onRuntimeInitialized = function () {
    const { ccall } = Module; // Access ccall for calling C functions

    function displayPrompt() {
        ccall('display_prompt', null, [], []); // Call the C function to display the prompt
    }

    function processInput(input) {
        ccall('process_input', null, ['string'], [input]); // Pass the input string to the C function
    }

    // Handle user input dynamically
    process.stdin.setEncoding('utf8');
    process.stdin.on('data', (chunk) => {
        const input = chunk.trim(); // Trim any extra whitespace
        processInput(input);

        if (input === 'exit') {
            console.log('Exiting...');
            process.stdin.end();
        } else {
            displayPrompt();
        }
    });

    process.stdin.on('end', () => {
        console.log('Input stream closed.');
    });

    process.stdin.resume(); // Start listening for input

    // Show the initial prompt
    displayPrompt();
};
