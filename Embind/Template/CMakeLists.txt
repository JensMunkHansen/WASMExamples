cmake_minimum_required(VERSION 3.15)
project(Template)

include(CTest)
enable_testing()

find_package(VTK 
COMPONENTS
  CommonCore
)

set(TARGET_TEST_NAME main)
# https://gitlab.kitware.com/vtk/vtk/-/issues/19008
add_executable(${TARGET_TEST_NAME} main.cpp)
set(emscripten_optimization_flags -g3 -O0)
target_link_libraries(${TARGET_TEST_NAME} PRIVATE VTK::CommonCore)
target_link_options(${TARGET_TEST_NAME} PRIVATE
  "-lembind"
  "-sMODULARIZE=1"
  "-sEXPORT_ES6=1"   # Needed for js script, but remove for index.htnl
  "-sEXPORT_NAME=loadTemplateWasmModule"
  "${emscripten_optimization_flags}")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/template.html ${CMAKE_CURRENT_BINARY_DIR}/index.html @ONLY)

target_compile_options(${TARGET_TEST_NAME} PRIVATE ${emscripten_optimization_flags})

# Copy Node/JavaScript to build directory
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

add_custom_command(
  TARGET ${TARGET_TEST_NAME}
  POST_BUILD
  COMMAND
    npm install
  WORKING_DIRECTORY
    ${CMAKE_CURRENT_BINARY_DIR})
    
add_test(NAME TemplatedFunction_Call_NoExceptions
  COMMAND
    npm run test
  WORKING_DIRECTORY
    ${CMAKE_CURRENT_BINARY_DIR})
      
