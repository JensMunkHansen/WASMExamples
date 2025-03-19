import { loadModule1 } from "./module1Loader.js";
import { loadModule2 } from "./module2Loader.js";
import { loadModule3 } from "./module3Loader.js";

async function main() {
    // Load both modules
    const module1 = await loadModule1();
    const module2 = await loadModule2();
    const module3 = await loadModule3();

    // Ensure WASM memory growth is handled in JS
    module3.resizeHeap = function (size) {
        console.log("Growing WebAssembly Memory...");
        let pagesNeeded = Math.ceil((size - module.wasmMemory.buffer.byteLength) / 65536);
        console.log(`Trying to grow by ${pagesNeeded} pages`);

        let result = module.wasmMemory.grow(pagesNeeded);
        console.log(`Memory growth result: ${result}`);
    
        return result;
    };

    module3._printHeapInfo();
    const ptr = module3._custom_malloc(12);
    console.log("Allocated at:", ptr);    
    

    let offset = 1024;

    // Write data using module1
    module1._write_data(offset, 42);

    // Read data using module2
    let value = module2._read_data(offset);
    console.log("Shared memory value:", value); // Should print: 42

    module1._use_heap();
    module2._use_heap();
    let testValue = module1._read_heap();
    console.log("Heap value:", testValue);
}

main().catch(console.error);
