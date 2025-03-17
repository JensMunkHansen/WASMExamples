import Module1Factory from "./module1.js"; // Import the generated JavaScript file
import { sharedMemory } from "./sharedMemory.js"; // Shared memory module

export async function loadModule1() {
    return Module1Factory({
        wasmMemory: sharedMemory, // Force the use of the shared memory instance
    });
}
