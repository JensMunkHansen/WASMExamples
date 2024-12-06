cmake_minimum_required(VERSION 3.22...3.28)

project(Catch2Test)

option(THREADING_ENABLED OFF)
set(_THREADING_ENABLED 0)
if (THREADING_ENABLED)
  set(_THREADING_ENABLED 1)
endif()

set(TARGET_NAME main)
set(LIBRARY_NAME library)
set(LIBRARY_PROXY_NAME wasmlibrary)

find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (BUILD_TESTING)
  find_package(Catch2 3.5.2 EXACT REQUIRED)
  include(CTest)
  include(Catch)
endif()

include(GenerateExportHeader)

# Configuration file for enabling/disabling threading
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/config.h.in" "${CMAKE_CURRENT_BINARY_DIR}/config.h" @ONLY)

add_library(${LIBRARY_NAME} STATIC "")

generate_export_header(${LIBRARY_NAME})

target_sources(${LIBRARY_NAME}
  PUBLIC
    FILE_SET public_headers
    TYPE HEADERS
    FILES
    library.h
  PRIVATE
    library.cxx
)

target_include_directories(${LIBRARY_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)

if (THREADING_ENABLED)
  target_link_libraries(${LIBRARY_NAME} PRIVATE Threads::Threads)
endif()

if (EMSCRIPTEN)
  set(emscripten_optimization_flags -g2 -O0)
  # It requires a .cxx file
  add_executable(${LIBRARY_PROXY_NAME} dummy.cxx)
  target_link_libraries(${LIBRARY_PROXY_NAME} PRIVATE ${LIBRARY_NAME})
  
  # -----------------------------------------------------------------------------
  # WebAssembly build options
  # -----------------------------------------------------------------------------
  set(emscripten_link_options)
  list(APPEND emscripten_link_options
      "-sMODULARIZE=1" # Module
      "-sEXPORT_ES6=1" # ES6
      "-sEXPORTED_FUNCTIONS=['_free', '_malloc', '_DoWork']" # Memory and DoWork
      "-sEXPORTED_RUNTIME_METHODS=['ENV', 'FS', 'ccall', 'stringToNewUTF8', 'addFunction']" # Standards including callbacks
      "-sINCLUDE_FULL_LIBRARY" # Prevent deletion of stuff
      "-sASSERTIONS=1"
      "-sALLOW_TABLE_GROWTH=1" # Function pointers
      "-sALLOW_MEMORY_GROWTH=1"
      "-sMAXIMUM_MEMORY=2GB"
      "-sERROR_ON_UNDEFINED_SYMBOLS=1" # Error tracking
      "-sEXPORT_NAME=loadCatch2TestModule" # Module entry
      "-sNO_EXIT_RUNTIME=0" # stdout always propagated      
  )
  if (THREADING_ENABLED)
    # Web with threading
    list(APPEND emscripten_link_options
      "-sENVIRONMENT=web,node,worker"
      "-sUSE_PTHREADS=1"
      "-sSHARED_MEMORY=1"
      "-sPTHREAD_POOL_SIZE=4"
    )  
  else()
    # Web, no threading
    list(APPEND emscripten_link_options
      "-sENVIRONMENT=web,node"
    )  
  endif()
  list(APPEND emscripten_link_options
    "${emscripten_optimization_flags}")

  target_link_options(${LIBRARY_PROXY_NAME} PRIVATE
    ${emscripten_link_options})
  
  set(script_files
    index.html
    run_Catch2Test.js
    package.json
    package-lock.json
  )
  
  foreach(script_file ${script_files})
    set(copyTarget ${LIBRARY_PROXY_NAME}_copy_${script_file})
    add_custom_target(
      ${copyTarget}
      COMMAND
        ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/${script_file}"
        "${CMAKE_CURRENT_BINARY_DIR}")
    add_dependencies(${LIBRARY_PROXY_NAME} ${copyTarget})
  endforeach()
  # We need this if we also make a test using a .js file
  add_custom_command(
    TARGET ${LIBRARY_PROXY_NAME}
    POST_BUILD
    COMMAND
      npm install
    WORKING_DIRECTORY
      ${CMAKE_CURRENT_BINARY_DIR})
else()
  add_executable(${TARGET_NAME} main.cxx)
  target_link_libraries(${TARGET_NAME} PRIVATE ${LIBRARY_NAME}) 
endif()

if (BUILD_TESTING)
  add_executable(sampleTest test.cxx)
  target_link_libraries(sampleTest PRIVATE library)
  target_link_libraries(sampleTest PRIVATE Catch2::Catch2WithMain)
  if (EMSCRIPTEN)
    # Standard Catch2 test as a .cjs file
    set(emscripten_link_options)
    # The testing is not an ES6 module
    list(APPEND emscripten_link_options
      "-sASSERTIONS=1"
      "-sERROR_ON_UNDEFINED_SYMBOLS=1"
      "-sNO_EXIT_RUNTIME=0"
    )
    if (THREADING_ENABLED)
      list(APPEND emscripten_link_options
        "-sUSE_PTHREADS=1"
        "-sSHARED_MEMORY=1"
        "-sPTHREAD_POOL_SIZE_STRICT=4"
        "-sENVIRONMENT=node,worker"
      )  
    else()
      list(APPEND emscripten_link_options
        "-sENVIRONMENT=node"
      )  
    endif()
    target_link_options(sampleTest PRIVATE
      ${emscripten_link_options})
    set_target_properties(sampleTest PROPERTIES SUFFIX ".cjs")

    add_test(NAME Catch2Test_ThreadedExecution_NoErrors
      COMMAND
        npx node run_Catch2Test.js
      WORKING_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR})
  endif()

  catch_discover_tests(sampleTest)
endif()
# npx node sampleTest.cjs

