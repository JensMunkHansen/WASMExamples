const { parentPort } = require('worker_threads');

// Simulate some work in the worker
const performWork = () => {
    setTimeout(() => {
        const result = 42; // Example result
        parentPort.postMessage(result); // Send the result to the main thread
    }, 1000); // Simulate asynchronous work
};

// Start work when the worker is initialized
performWork();
