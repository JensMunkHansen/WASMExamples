import ModuleFactory from "./module3.js"; // Import the generated JavaScript file
import { sharedMemory } from "./sharedMemory.js"; // Shared memory module

export async function loadModule3() {
    return ModuleFactory({
        wasmMemory: sharedMemory, // Use the same shared memory instance
    });
}
