import { parentPort } from 'worker_threads';
import CustomModuleName from './example.js';

parentPort.on('message', async (data) => {
  const { a, b } = data;
  const wasmModule = await CustomModuleName();

  // Call the WASM function
  const result = wasmModule.ccall('add', 'number', ['number', 'number'], [a, b]);
  parentPort.postMessage(result);
});
