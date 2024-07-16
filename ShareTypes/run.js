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
	const wasmModule1 = await loadModule('./First.js', 'loadFirstModule');
	const wasmModule2 = await loadModule('./Second.js', 'loadSecondModule');
        // Create an instance of the module object
	const instance1 = wasmModule1._CreateStruct();
	wasmModule1._Consume(instance1);
	// Do not share memory, but at least the types are known
	wasmModule2._Consume(instance1);
    } catch (error) {
	nErrors = 1;
        console.error("An error occurred:", error);
    }
}

// Execute the main function
await main();
