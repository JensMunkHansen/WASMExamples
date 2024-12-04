cmake_minimum_required(VERSION 3.14)
project(WASMSharedLibraryExample)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Common compilation and linking flags
set(COMPILE_FLAGS "-fPIC")
set(SIDE_MODULE_FLAGS "-s SIDE_MODULE=2 -s EMULATE_FUNCTION_POINTER_CASTS=1 -s ERROR_ON_UNDEFINED_SYMBOLS=0 -s EXPORTED_FUNCTIONS=['_pointToString'] -s EXPORTED_RUNTIME_METHODS="['ENV', 'FS', 'ccall', 'stringToNewUTF8', 'addFunction']"")
set(MAIN_MODULE_FLAGS "-s MAIN_MODULE=2 -s EMULATE_FUNCTION_POINTER_CASTS=1 -s ERROR_ON_UNDEFINED_SYMBOLS=0 -s EXPORTED_FUNCTIONS=['_main'] -v")

# Build libtypes as a SIDE_MODULE
add_executable(libtypes libtypes/types.cpp)
target_compile_options(libtypes PRIVATE ${COMPILE_FLAGS})
set_target_properties(libtypes PROPERTIES LINK_FLAGS "${SIDE_MODULE_FLAGS}")
set_target_properties(libtypes PROPERTIES OUTPUT_NAME "libtypes")
set_target_properties(libtypes PROPERTIES SUFFIX ".wasm")

# Build module1 as a MAIN_MODULE
add_executable(module1 module1/module1.cpp)
target_compile_options(module1 PRIVATE ${COMPILE_FLAGS})
set_target_properties(module1 PROPERTIES LINK_FLAGS "${MAIN_MODULE_FLAGS}")
set_target_properties(module1 PROPERTIES OUTPUT_NAME "module1")

# Build module2 as a MAIN_MODULE
add_executable(module2 module2/module2.cpp)
target_compile_options(module2 PRIVATE ${COMPILE_FLAGS})
set_target_properties(module2 PROPERTIES LINK_FLAGS "${MAIN_MODULE_FLAGS}")
set_target_properties(module2 PROPERTIES OUTPUT_NAME "module2")
