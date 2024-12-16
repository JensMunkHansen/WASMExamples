const createModule = require('./output.js');

(async () => {
  const wasmModule = await createModule({
    locateFile: (path) => {
      if (path.endsWith('.wasm')) {
        return './output.wasm'; // Specify the exact path to the .wasm file
      }
      return path;
    },
  });

  // Log all exports for debugging
  console.log('Available exports:', Object.keys(wasmModule));

  // Call the mangled function
  if (wasmModule.__Z16mangled_functioni) {
    console.log('Calling _Z16mangled_functioni...');
    const result = wasmModule.__Z16mangled_functioni(4);
    console.log(`Result: ${result}`);
  } else {
    console.error('Function _Z16mangled_functioni not found!');
  }
})();
