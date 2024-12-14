import { Worker } from 'worker_threads';

// Define tasks for the workers
const tasks = [
  { a: 5, b: 3 },
  { a: 10, b: 7 }
];

// Create workers
const worker1 = new Worker(new URL('./worker.js', import.meta.url), { type: 'module' });
const worker2 = new Worker(new URL('./worker.js', import.meta.url), { type: 'module' });

// Helper function to terminate a worker after receiving the message
const handleWorkerMessage = (worker, id) => {
  return (result) => {
    console.log(`Worker ${id} result: ${result}`);
    worker.terminate(); // Terminate the worker
    checkForCompletion(); // Check if all workers are done
  };
};

// Track active workers
let activeWorkers = 2;

// Check if all workers have completed their tasks
const checkForCompletion = () => {
  activeWorkers--;
  if (activeWorkers === 0) {
    console.log("All workers have completed their tasks. Exiting...");
    process.exit(0); // Exit the program
  }
};

// Listen for messages from workers
worker1.on('message', handleWorkerMessage(worker1, 1));
worker2.on('message', handleWorkerMessage(worker2, 2));

// Start workers with tasks
worker1.postMessage(tasks[0]);
worker2.postMessage(tasks[1]);
