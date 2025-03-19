import { loadModule1 } from "./module1Loader.js";
import { loadModule2 } from "./module2Loader.js";

async function main() {
    // Load both modules
    const module1 = await loadModule1();
    const module2 = await loadModule2();

    let offset = 1024;

    // Write data using module1
    module1._write_data(offset, 42);

    // Read data using module2
    let value = module2._read_data(offset);
    console.log("Shared memory value:", value); // Should print: 42

    module1._use_heap();
    module2._use_heap();
    let testValue = module1._read_heap();
    console.log("Heap value:", testValue);
}

main().catch(console.error);
