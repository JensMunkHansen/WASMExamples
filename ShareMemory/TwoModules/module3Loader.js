import Module3Factory from "./module3.js"; // Import the generated JavaScript file
import { sharedMemory } from "./sharedMemory.js"; // Shared memory module

const moduleTable3 = new WebAssembly.Table({ initial: 50, maximum: 100, element: "anyfunc" });
export async function loadModule3() {
    return Module3Factory({
        wasmMemory: sharedMemory, // Use the same shared memory instance
        wasmTable: moduleTable3,
    });
}
