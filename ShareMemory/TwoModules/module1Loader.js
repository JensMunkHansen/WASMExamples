import ModuleFactory from "./module1.js"; // Import the generated JavaScript file
import { sharedMemory } from "./sharedMemory.js"; // Shared memory module

export async function loadModule1() {
    return ModuleFactory({
        wasmMemory: sharedMemory, // Force the use of the shared memory instance
    });
}
