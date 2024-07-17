import fs from 'fs';
import path from 'path';

// Dynamically import the compiled WASM modules
const Module1 = (await import('./module1.js')).default;
const Module2 = (await import('./module2.js')).default;

// Create shared memory
const sharedMemory = new WebAssembly.Memory({
    initial: 1024, // 1024 pages = 64MB
    maximum: 1024, // 1024 pages = 64MB
    shared: true
});
const sharedMemoryBuffer = new Int32Array(sharedMemory.buffer);
console.log(sharedMemoryBuffer.byteOffset)
console.log('Initializing Module1 and Module2...');

// Synchronization delay
const delay = ms => new Promise(resolve => setTimeout(resolve, ms));

console.log('Initializing Module1...');

Module1({ wasmMemory: sharedMemory }).then(instance1 => {
    console.log('Module1 initialized');
    delay(100).then(() => {
        console.log('Initializing Module2...');
	
	Module2({ wasmMemory: sharedMemory }).then(instance2 => {
            console.log('Module2 initialized');

	    // Function to run the operations with synchronization
            async function runOperations() {
	        const largeOffset = 1024*1024 / 4;
                // Set shared memory to 42 using Module1
                console.log('Setting memory with Module1...');
                instance1._set_shared_memory(sharedMemoryBuffer.byteOffset + largeOffset, 41);
                console.log('Set shared memory to 42 at large offset');
                await delay(100); // Small delay for synchronization
                instance1._get_shared_memory(sharedMemoryBuffer.byteOffset + largeOffset);
                await delay(100); // Small delay for synchronization
	    
                // Read shared memory using Module2
                console.log('Reading memory with Module2...');
                instance2._get_shared_memory(sharedMemoryBuffer.byteOffset + largeOffset);
	    
                await delay(100); // Small delay for synchronization
	    
                // Set shared memory to 84 using Module2
                console.log('Setting memory with Module2...');
                instance2._set_shared_memory(sharedMemoryBuffer.byteOffset + largeOffset, 84);
                console.log('Set shared memory to 84 at large offset using Module2');
	    
                await delay(100); // Small delay for synchronization
	    
                // Read shared memory using Module1
                console.log('Reading memory with Module1...');
                instance1._get_shared_memory(sharedMemoryBuffer.byteOffset + largeOffset);
            }
	    runOperations();
	}).catch(err => {
            console.error('Error initializing Module2:', err);
	});
    });
}).catch(err => {
    console.error('Error initializing Module1:', err);
});
