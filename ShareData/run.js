async function loadModule(modulePath, moduleName) {
  // Dynamic import for both browser and Node.js
  const module = await import(modulePath);
  return module.default();
}

let nErrors = 0;
// Main function to load and use the WASM module
async function main() {
    try {
        // Load the WASM modules
	const wasmModule1 = await loadModule('./module1.mjs', 'loadModule1');
	const wasmModule2 = await loadModule('./module2.mjs', 'loadModule2');

        const sharedMemory = new Int32Array(wasmModule1.HEAP32.buffer, 0, 1);

        const printFromModule1 = wasmModule1.cwrap('print_from_module1', null, ['number']);
        const setValueInSharedMemory = wasmModule2.cwrap('set_value_in_shared_memory', null, ['number']);
	console.log(sharedMemory.byteOffset);
        setValueInSharedMemory(sharedMemory.byteOffset);
        printFromModule1(sharedMemory.byteOffset);	
    } catch (error) {
	nErrors = 1;
        console.error("An error occurred:", error);
    }
}

// Execute the main function
await main();

// TODO: Check references

// Can be captured with $?
process.exit(nErrors == 0 ? 0 : 1);
