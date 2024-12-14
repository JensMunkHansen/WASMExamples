MODULARIZE with ES6

main.js

const { Worker } = require('worker_threads');

// Initialize the WebAssembly module
const CustomModuleName = require('./example.js');

// Create workers
const worker1 = new Worker('./worker.js');
const worker2 = new Worker('./worker.js');

// Define tasks for the workers
const tasks = [
  { a: 5, b: 3 },
  { a: 10, b: 7 }
];

// Listen for messages from workers
worker1.on('message', (result) => console.log(`Worker 1 result: ${result}`));
worker2.on('message', (result) => console.log(`Worker 2 result: ${result}`));

// Start workers with tasks
worker1.postMessage(tasks[0]);
worker2.postMessage(tasks[1]);

worker.js

const { parentPort } = require('worker_threads');
const CustomModuleName = require('./example.js');

parentPort.on('message', async (data) => {
  const { a, b } = data;
  const wasmModule = await CustomModuleName();

  // Call the WASM function
  const result = wasmModule.ccall('add', 'number', ['number', 'number'], [a, b]);
  parentPort.postMessage(result);
});
