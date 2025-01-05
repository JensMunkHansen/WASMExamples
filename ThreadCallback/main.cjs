const { Worker } = require('worker_threads');

// Function to be invoked on the main thread with the worker's result
const onResult = (result) => {
    console.log(`Callback invoked on main thread with result: ${result}`);
};

// Start the worker
const worker = new Worker('./worker.cjs');

// Listen for messages from the worker
worker.on('message', (result) => {
    console.log(`Received result from worker: ${result}`);
    onResult(result); // Invoke the callback function with the result
});

// Handle errors in the worker
worker.on('error', (err) => {
    console.error(`Worker error: ${err}`);
});

// Handle worker exit
worker.on('exit', (code) => {
    if (code !== 0) {
        console.error(`Worker stopped with exit code ${code}`);
    } else {
        console.log('Worker exited successfully.');
    }
});
