import Module from '../build/example.js';

async function main() {
    const inputFile = 'input.json';
    const outputFile = 'output.json';
    const inputData = "[1, 2, 3, 4, 5]"; // Example input data

    // Initialize the WebAssembly module
    const wasmModule = await Module();

    console.log("WASM Module initialized successfully.");

    // Write the input file to the WASM virtual file system
    wasmModule.FS.writeFile(inputFile, inputData);

    // Use stringToNewUTF8 to allocate strings in WASM memory
    const allocateString = (str) => wasmModule.stringToNewUTF8(str);

    // Create the arguments
    const args = ['TestMe', '-D', inputFile, '-T', outputFile];
    const argc = args.length;

    // Allocate memory for argv (array of pointers to strings)
    const argv = wasmModule._malloc(argc * 4); // Each pointer is 4 bytes in WASM

    // Write pointers to argv
    for (let i = 0; i < argc; i++) {
        const argPtr = allocateString(args[i]);
        wasmModule.HEAP32[(argv / 4) + i] = argPtr; // Write each pointer to argv
    }

    // Call the TestMe function with argc and argv
    const result = wasmModule._TestMe(argc, argv);

    // Free allocated memory
    for (let i = 0; i < argc; i++) {
        wasmModule._free(wasmModule.HEAP32[(argv / 4) + i]); // Free each string
    }
    wasmModule._free(argv); // Free the argv array

    // Check the result and read the output file
    if (result === 0) {
        console.log("TestMe completed successfully.");
        const output = wasmModule.FS.readFile(outputFile, { encoding: 'utf8' });
        console.log("Output from WASM:");
        console.log(output);
    } else {
        console.error("TestMe failed with error code:", result);
    }
}

main().catch(err => {
    console.error("Error running WASM test:", err);
});
