// Create a new Web Worker using the worker.mjs script
const worker = new Worker('./worker.mjs', { type: 'module' });

// Listen for messages from the worker
worker.onmessage = function (event) {
    if (event.data.error) {
        console.error('Worker error:', event.data.error);
    } else {
        console.log('Result from Web Worker:', event.data);
    }
};

// Send data to the worker
const a = 5;
const b = 10;
worker.postMessage({ a, b });
