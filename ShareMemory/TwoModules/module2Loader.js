import ModuleFactory from "./module2.js"; // Import the generated JavaScript file
import { sharedMemory } from "./sharedMemory.js"; // Shared memory module

export async function loadModule2() {
    return ModuleFactory({
        wasmMemory: sharedMemory, // Use the same shared memory instance
    });
}
