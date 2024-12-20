import Module from '../build/example.js';
import fs from 'fs';
import path from 'path';
import yargs from 'yargs';
import { hideBin } from 'yargs/helpers';

async function main() {
    const argv = yargs(hideBin(process.argv))
        .option('wasm', { type: 'string', demandOption: true, describe: 'Path to the WASM file' })
        .option('input', { type: 'string', demandOption: true, describe: 'Path to the input file' })
        .option('expected', { type: 'string', demandOption: true, describe: 'Path to the expected output file' })
        .argv;

    const input = JSON.parse(fs.readFileSync(path.resolve(argv.input), 'utf8'));
    const expectedOutput = JSON.parse(fs.readFileSync(path.resolve(argv.expected), 'utf8'));

    // Initialize the WebAssembly module
    const wasmModule = await Module();

    // Allocate memory for the input array in WebAssembly memory
    const inputArray = new Int32Array(input);
    const byteSize = inputArray.length * inputArray.BYTES_PER_ELEMENT;

    const wasmMemoryPtr = wasmModule._malloc(byteSize);
    const wasmMemory = new Int32Array(wasmModule.HEAP32.buffer, wasmMemoryPtr, inputArray.length);

    // Copy input data into WASM memory
    wasmMemory.set(inputArray);

  // Call the WASM function
    const result = wasmModule._sum_array(wasmMemoryPtr, inputArray.length);

    // Free the allocated memory
    wasmModule._free(wasmMemoryPtr);

    // Compare result
    if (result === expectedOutput.result) {
        console.log("Test passed! Result:", result);
    } else {
        console.error("Test failed! Expected:", expectedOutput.result, "Got:", result);
        process.exit(1);
    }
}

main().catch(err => {
    console.error("Error running WASM test:", err);
    process.exit(1);
});
