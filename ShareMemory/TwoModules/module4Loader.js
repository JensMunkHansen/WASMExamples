import Module4Factory from "./module4.js"; // Import the generated JavaScript file
import { sharedMemory } from "./sharedMemory.js"; // Shared memory module

const moduleTable = new WebAssembly.Table({ initial: 50, maximum: 100, element: "anyfunc" });

export async function loadModule4() {
    return Module4Factory({
        wasmMemory: sharedMemory, // Use the same shared memory instance
        wasmTable: moduleTable,
        env: {
            tableBase: 50, // REQUIRED to avoid function index conflicts
        }        
    });
}
