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
	const instance1 = wasmModule1._CreateStruct(42, 3.14);
	wasmModule1._Consume(instance1);
        var i1 = wasmModule1._ConsumeTest(instance1);
        if (i1 == 42) {
            console.log("Module1 can see data from Module1");
            console.log(`MyData.i: ${i1}`);
        }
        
	// Doesn't share memory, but at least the types are known
	wasmModule2._Consume(instance1);
        var i2 = wasmModule2._ConsumeTest(instance1);
        if (i2 == 0) {
            console.log("Module2 cannot see data from Module1");
            console.log(`MyData.i: ${i2}`);
        }
        wasmModule2._DestroyStruct(instance1);

        wasmModule1._DestroyStruct(instance1);
        
    } catch (error) {
	nErrors = 1;
        console.error("An error occurred:", error);
    }
}

// Execute the main function
await main();
