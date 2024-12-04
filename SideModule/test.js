const fs = require('fs');

(async () => {
    // Load libtypes.wasm
    const libtypesWasm = fs.readFileSync('./libtypes.wasm');
    const libtypes = await WebAssembly.instantiate(libtypesWasm, {});

    // Prepare the `env` imports from libtypes
    const envImports = libtypes.instance.exports;

    // Load module1.wasm and link to libtypes.wasm
    const module1Wasm = fs.readFileSync('./module1.wasm');
    const module1 = await WebAssembly.instantiate(module1Wasm, {
        env: envImports,
    });

    // Load module2.wasm and link to libtypes.wasm
    const module2Wasm = fs.readFileSync('./module2.wasm');
    const module2 = await WebAssembly.instantiate(module2Wasm, {
        env: envImports,
    });

    // Run module1
    console.log("Running Module 1:");
    module1.instance.exports._main();

    // Run module2
    console.log("Running Module 2:");
    module2.instance.exports._main();
})();
