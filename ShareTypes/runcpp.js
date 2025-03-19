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
        console.log("obj.i: ${obj.i} obj.f: ${obj.f}");
        var i = wasmModule1.ConsumeTestCPP(obj);
        if (i != 42) {
            throw "wasmModule1 cannot read WrappedMyData.i";
        }
        try {
            wasmModule2.ConsumeTestCPP(obj);
        } catch (e) {
            if (e.name !== "BindingError") {
                console.log(e.error());
                console.log("We expected a BindindError");
                nErrors++;
            }
            console.log("We expect this error: " + e.name);
            console.log("wasmModule2 cannot understand type WrappedMyData from wasmModule1");
        }

        // Reference values
        console.log(`wasmModule1: obj.i = ${obj.i}, obj.f = ${obj.f}`);
        
        // Retrieve the raw memory pointer
        let ptr1 = obj.getPointer();  // Assume `getPointer()` returns the memory address
        let structSize = 8; // Assuming 4 bytes for int + 4 bytes for float

        // Read the raw memory from wasmModule1
        let memory1 = new Uint8Array(wasmModule1.HEAPU8.buffer, ptr1, structSize);
        // Allocate host memory
        let copiedData = new Uint8Array(structSize);
	// Copy the memory from WASM1 to a JS buffer
        copiedData.set(memory1); 

        // Allocate a struct in wasmModule2
        let obj2 = new wasmModule2.WrappedMyData(0, 0.0); // Create a new instance in wasm2
        let ptr2 = obj2.getPointer();  // Get its memory address

        // Copy data to wasmModule2's memory
        let memory2 = new Uint8Array(wasmModule2.HEAPU8.buffer, ptr2, structSize);
        memory2.set(copiedData); // Overwrite wasm2's memory
        //obj2.i = 7; // Saved by mangling

        wasmModule2.ConsumeTestCPP(obj2);

        
        // Verify the copied values
        console.log(`wasmModule2: obj2.i = ${obj2.i}, obj2.f = ${obj2.f}`);

        console.log(wasmModule1.WrappedMyData.StaticInt);
        console.log(wasmModule2.WrappedMyData.StaticInt);
        
        // Check if the data was successfully transferred
        if (obj2.i !== obj.i || Math.abs(obj2.f - obj.f) > 0.001) {
            throw "Memory copy failed!";
        }
    } catch (error) {
	nErrors = 1;
        console.error("An error occurred:", error);
    }
}

// Execute the main function
await main();

process.exit(nErrors == 0 ? 0 : 1);
