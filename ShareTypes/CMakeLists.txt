cmake_minimum_required(VERSION 3.15)

project(ShareTypes)

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
  "-sEXPORTED_FUNCTIONS=['_CreateStruct','_Produce','_Consume']"
  "${emscripten_optimization_flags}")

add_executable(${TARGET_TEST_NAME} Types.cxx First.cxx)
target_include_directories(${TARGET_TEST_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_options(${TARGET_TEST_NAME} PUBLIC ${emscripten_link_options})
target_link_options(${TARGET_TEST_NAME} PUBLIC "-s EXPORT_NAME=loadFirstModule")
target_compile_options(${TARGET_TEST_NAME} PRIVATE ${emscripten_optimization_flags})

set(TARGET_TEST_NAME Second)
add_executable(${TARGET_TEST_NAME} Types.cxx First.cxx)
target_include_directories(${TARGET_TEST_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_options(${TARGET_TEST_NAME} PUBLIC ${emscripten_link_options})
target_link_options(${TARGET_TEST_NAME} PUBLIC "-s EXPORT_NAME=loadSecondModule")
target_compile_options(${TARGET_TEST_NAME} PRIVATE ${emscripten_optimization_flags})

set(node_and_javascript_files
  package.json
  package-lock.json
  run.js
)

foreach(node_and_javascript_file ${node_and_javascript_files})
  add_custom_command(
    TARGET ${TARGET_TEST_NAME}
    COMMAND
      ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/${node_and_javascript_file}"
        $<TARGET_FILE_DIR:${TARGET_TEST_NAME}>)
endforeach()
