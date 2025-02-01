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
	const wasmModule1 = await loadModule('./FirstCPP.js', 'loadFirstCPPModule');
	const wasmModule2 = await loadModule('./SecondCPP.js', 'loadSecondCPPModule');

        let obj = new wasmModule1.WrappedMyData(42, 3.14);
        console.log(obj.i);  // 42
        console.log(obj.f); // 3.14
        wasmModule1.ConsumeTestCPP(obj);

        wasmModule2.ConsumeTestCPP(obj);
        
    } catch (error) {
	nErrors = 1;
        console.error("An error occurred:", error);
    }
}

// Execute the main function
await main();
