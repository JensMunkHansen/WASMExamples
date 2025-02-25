import { Component } from '@webassembly/component';

async function main() {
    // Load the shared interface
    const sharedInterface = await Component.fromWit('./shared.wit');

    // Load the modules
    const moduleA = await Component.fromFile('./module_a.wasm', sharedInterface);
    const moduleB = await Component.fromFile('./module_b.wasm', sharedInterface);

    // Create a Person in ModuleA
    const person = moduleA.create_person("Alice", 30);

    // Process the Person in ModuleB
    moduleB.process_person(person);
}

main().catch(console.error);
