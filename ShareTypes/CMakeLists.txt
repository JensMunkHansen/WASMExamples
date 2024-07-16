cmake_minimum_required(VERSION 3.15)

include(CTest)
enable_testing()

set(TARGET_TEST_NAME First)
set(emscripten_optimization_flags -g3 -O0)
set(emscripten_link_options)
list(APPEND emscripten_link_options
  "-lembind"
  "-sMODULARIZE=1"
  "-sEXPORT_ES6=1"
  "-sASSERTIONS=1" # This makes linker assert stuff so we can see what is wrong
  "-sERROR_ON_UNDEFINED_SYMBOLS=1"
  "${emscripten_optimization_flags}")

add_executable(${TARGET_TEST_NAME} First.cxx Types.cxx)
target_link_options(${TARGET_TEST_NAME} ${emscripten_link_options})
target_link_options(${TARGET_TEST_NAME}  "-sEXPORT_NAME=loadFirstModule")
target_compile_options(${TARGET_TEST_NAME} PRIVATE ${emscripten_optimization_flags})

set(TARGET_TEST_NAME Second)
add_executable(${TARGET_TEST_NAME} Second.cxx Types.cxx)
target_link_options(${TARGET_TEST_NAME} ${emscripten_link_options})
target_link_options(${TARGET_TEST_NAME}  "-sEXPORT_NAME=loadSecondModule")
target_compile_options(${TARGET_TEST_NAME} PRIVATE ${emscripten_optimization_flags})
