import Module from '../build/example.js';
import fs from 'fs/promises';
import path from 'path';
import yargs from 'yargs';
import { hideBin } from 'yargs/helpers';

async function main() {
    // Parse command-line arguments using yargs
    const argv = yargs(hideBin(process.argv))
          .option('wasm', { type: 'string', demandOption: true, describe: 'Path to the WASM file' })
          .option('input', { type: 'string', demandOption: true, describe: 'Path to the input JSON file' })
          .option('expected', { type: 'string', demandOption: true, describe: 'Path to the expected output JSON file' })
          .argv;

    console.log("Parsed arguments:", argv);

    const wasmPath = argv.wasm;
    const inputFilePath = argv.input;
    const expectedFilePath = argv.expected;

    console.log(`WASM file: ${wasmPath}`);
    console.log(`Input file: ${inputFilePath}`);
    console.log(`Expected output file: ${expectedFilePath}`);

    // Initialize WebAssembly Module
    const wasmModule = await Module();

    console.log("WASM Module initialized successfully.");

    // Copy input file into WASM file system
    const inputData = await fs.readFile(inputFilePath, 'utf8');
    const inputFileName = 'input.json';
    const outputFileName = 'output.json';

    wasmModule.FS.writeFile(inputFileName, inputData);

    // Call the TestMe function
    const allocateString = wasmModule.stringToNewUTF8;
    const argsArray = ['TestMe', '-D', inputFileName, '-T', outputFileName];
    const argc = argsArray.length;
    const argvPtr = wasmModule._malloc(argc * 4);

    for (let i = 0; i < argc; i++) {
        wasmModule.HEAP32[(argvPtr / 4) + i] = allocateString(argsArray[i]);
    }

    const result = wasmModule._TestMe(argc, argvPtr);

    // Free allocated memory
    for (let i = 0; i < argc; i++) {
        wasmModule._free(wasmModule.HEAP32[(argvPtr / 4) + i]);
    }
    wasmModule._free(argvPtr);

    if (result === 0) {
        console.log("TestMe completed successfully.");
        const output = wasmModule.FS.readFile(outputFileName, { encoding: 'utf8' });
        console.log("Output from WASM:");
        console.log(output);

        // Compare the output with expected
        const expectedOutput = await fs.readFile(expectedFilePath, 'utf8');
        if (output.trim() === expectedOutput.trim()) {
            console.log("Test passed! Output matches expected output.");
        } else {
            console.error("Test failed! Expected:", expectedOutput, "Got:", output);
            process.exit(1);
        }
    } else {
        console.error("TestMe failed with error code:", result);
        process.exit(1);
    }
}

main().catch((err) => {
    console.error("Error running WASM test:", err);
    process.exit(1);
});
