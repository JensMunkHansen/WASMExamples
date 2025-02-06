async function loadModule(modulePath, sharedMemory) {
    const module = await import(modulePath);
    return module.default({ wasmMemory: sharedMemory });
}

let nErrors = 0;

async function main() {
    try {
        // Create a shared memory instance
        let sharedMemory = new WebAssembly.Memory({
            initial: 256,  // 256 pages (each page is 64KB)
            maximum: 256,  // Keep the max same
            shared: true   // Enables shared memory
        });
        // Load both WASM modules with the shared memory
        const wasmModule2 = await loadModule('./SecondSharedCPP.js', sharedMemory);
        const wasmModule1 = await loadModule('./FirstSharedCPP.js', sharedMemory);

        const getMemoryStart1 = wasmModule1.cwrap('getMemoryStartFirst', 'number', []);
        const getMemoryStart2 = wasmModule2.cwrap('getMemoryStartSecond', 'number', []);
        console.log("First memory address:", getMemoryStart1());
        console.log("Second memory address:", getMemoryStart2());
        
        // Create an object in wasmModule1
        let obj = new wasmModule1.WrappedMyData(42, 3.14);
        console.log(`wasmModule1: obj.i = ${obj.i}, obj.f = ${obj.f}`);

        // Get the memory pointer
        let ptr1 = obj.getPointer(); // Assume `getPointer()` returns a memory address

        let obj2 = null;

        let copyToNewLocation = false;
        
        if (copyToNewLocation) {
            // Read raw memory using the shared buffer
            let structSize = 8; // Assuming 4 bytes for int + 4 bytes for float
            let memoryView = new Uint8Array(sharedMemory.buffer, ptr1, structSize);
            
            // Create an instance in wasmModule2 and manually set memory
            obj2 = new wasmModule2.WrappedMyData(0, 0.0); 
            let ptr2 = obj2.getPointer();  // Get memory pointer in wasmModule2
            
            // **Explicitly set obj2's memory using shared buffer**
            let memoryView2 = new Uint8Array(sharedMemory.buffer, ptr2, structSize);
            memoryView2.set(memoryView); // Copy shared memory values (location of obj2)
        }
        else {
            // We have implemented function for directly setting the pointer
            obj2 = wasmModule2.WrappedMyData.fromPointer(ptr1);
        }
        // Validate shared memory worked
        console.log(`wasmModule2: obj2.i = ${obj2.i}, obj2.f = ${obj2.f}`);

        // Check if values are correctly copied
        if (obj2.i !== obj.i || Math.abs(obj2.f - obj.f) > 0.001) {
            throw "Shared memory copy failed!";
        }

        // Dangerous using shared memory (dangerous they share static content)
        console.log(wasmModule1.getGlobalStaticValue());
        console.log(wasmModule2.getGlobalStaticValue());

        // Calling the wrong function
        wasmModule1.ConsumeTestCPP(obj);
        wasmModule2.ConsumeTestCPP(obj2);

        console.log(wasmModule1.getTrueStaticValue());
        console.log(wasmModule2.getTrueStaticValue());

    } catch (error) {
        nErrors = 1;
        console.error("An error occurred:", error);
    }
}

// Run main function
await main();

process.exit(nErrors === 0 ? 0 : 1);


