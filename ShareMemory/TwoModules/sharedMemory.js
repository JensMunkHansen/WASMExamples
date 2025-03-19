export const sharedMemory = new WebAssembly.Memory({
    initial: 256,  // Minimum pages (64KB each)
    maximum: 512,  // Maximum pages (64KB each)
    shared: true   // Enable shared memory
});
