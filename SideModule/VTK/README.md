The function call is almost as fast as calling an internal
function. This is comparable to PLT (Procedure Linkage Table) lookups
in native DLLs. Calling requires lookup in WASM table. One jump
instruction. First time a Global Offset Table (GOT) lookup. First call
10-100 times slower. 5 cycles slower, '-fno-plt' Use -flto


Why doesn't Embind support type library generation that can be shared between WASM modules

ChatGPT said:

Great question! The reason Embind does not support type library
generation for shared use between multiple WebAssembly (WASM) modules
comes down to a combination of technical limitations in the current
WebAssembly environment and Embind's design choices. Let’s break this
down:

1. Lack of a Common Type System Between WASM Modules

In WebAssembly, each module is an isolated environment with its own
memory space and symbol table. This means:

* No shared runtime type information (RTTI): C++ relies heavily on
  RTTI (e.g., type metadata for casting and inheritance), but this is
  not natively supported across multiple WASM modules.

* Memory and type isolation: WASM modules do not have a standard
  mechanism for exchanging complex type information (classes, structs)
  beyond basic C-style primitives (int, float, pointers, etc.).

 * Function pointers and vtables are local to each module and are not
   easily shareable across modules, which is crucial for polymorphism
   and callbacks.

2. Embind Focuses on JavaScript Bindings, Not Inter-Module Communication

Embind was designed to provide bindings for C++ to JavaScript, not C++
to C++ between WASM modules. As a result:

* Type marshaling: Embind generates glue code that converts C++ types
  to corresponding JavaScript representations (like std::vector<int>
  to a JavaScript array). This glue code is tightly coupled to the
  specific compiled module.

* No serialization of C++ types: There’s no built-in mechanism to
  serialize complex types (e.g., C++ classes) into a format that
  another module could easily read.


3. Module Instantiation and Memory Layout

* Separate Memory Spaces: Each WASM module has its own linear memory
  and cannot directly access the memory of another module.

* Memory Offsets: Embind relies on fixed memory layouts for
  objects. When passing objects between modules, these memory
  addresses become invalid or meaningless in the context of the other
  module.


4. ABI (Application Binary Interface) Differences

Even though C ABI is stable and works well across shared libraries
(e.g., extern "C" functions), C++ ABI is notoriously platform- and
compiler-specific:

* Class layout, virtual functions, and inheritance may vary between
  modules, even when compiled with the same toolchain.

* Name mangling: C++ symbols are often mangled to include type
  information, which complicates symbol resolution between separate
  modules.

Possible Workarounds or Solutions:

1. C ABI + Shared Memory: Use a C-style API (like your VectorView
   example) and manage complex objects through shared memory. This
   avoids most of the issues with type information and RTTI.

* You can serialize complex objects into shared memory and pass
  offsets between modules.

* Shared memory can be mapped into both modules' address spaces,
  enabling zero-copy access.

2. Custom Type Serialization: Define a protocol for serializing and
   deserializing types between modules. This could involve:

* Flattening complex C++ types into raw data buffers.

* Passing the serialized buffer and its metadata between modules.

3. Type Metadata at Runtime: Build a lightweight runtime system that
   tracks type metadata at runtime and shares it between modules
   through an external registry.

4. Future WebAssembly Interface Types: The upcoming WebAssembly
   Interface Types proposal aims to improve interoperability between
   modules and languages, allowing richer types (like strings, arrays,
   and records) to be passed across module boundaries.

Conclusion:

The lack of type library generation in Embind for cross-module use is
primarily due to WebAssembly’s isolation model, C++ ABI complexities,
and Embind’s focus on JavaScript bindings. However, C-style APIs with
shared memory and custom serialization offer practical workarounds
until WebAssembly Interface Types mature.

Would you like an example showing how to share complex objects between
two WASM modules using shared memory? 😊
