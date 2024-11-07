cmake_minimum_required(VERSION 3.22...3.28)

project(Catch2Test)

option(THREADING_ENABLED OFF)
set(_THREADING_ENABLED 0)
if (THREADING_ENABLED)
  set(_THREADING_ENABLED 1)
endif()

set(TARGET_NAME main)
set(LIBRARY_NAME library)

find_package(Threads REQUIRED)

include(GenerateExportHeader)

# Configuration file for enabling/disabling threading
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/config.h.in" "${CMAKE_CURRENT_BINARY_DIR}/config.h" @ONLY)

if (EMSCRIPTEN)
  set(emscripten_optimization_flags -g2 -O0)
  add_executable(${LIBRARY_NAME})
else()
  add_library(${LIBRARY_NAME} STATIC "")
  generate_export_header(${LIBRARY_NAME})
endif()

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
  # -----------------------------------------------------------------------------
  # WebAssembly build options
  # -----------------------------------------------------------------------------
  set(emscripten_link_options)
  list(APPEND emscripten_link_options
      "-sMODULARIZE=1"
      "-sEXPORT_ES6=1"
      "-sEXPORTED_FUNCTIONS=['_free', '_malloc', '_DoWork']"
      "-sEXPORTED_RUNTIME_METHODS=['ENV', 'FS', 'ccall', 'stringToNewUTF8', 'addFunction']"
      "-sINCLUDE_FULL_LIBRARY"
      "-sASSERTIONS=1"
      "-sALLOW_TABLE_GROWTH=1"
      "-sALLOW_MEMORY_GROWTH=1"  
      "-sERROR_ON_UNDEFINED_SYMBOLS=1"
      "-sEXPORT_NAME=loadCatch2TestModule"
      "-sNO_EXIT_RUNTIME=0"      
  )
  if (THREADING_ENABLED)
    list(APPEND emscripten_link_options
      "-sENVIRONMENT=web,worker"
      "-sUSE_PTHREADS=1"
      "-sPTHREAD_POOL_SIZE=4"
    )  
  else()
    list(APPEND emscripten_link_options
      "-sENVIRONMENT=web"
    )  
  endif()
  list(APPEND emscripten_link_options
    "${emscripten_optimization_flags}")

  target_link_options(${LIBRARY_NAME} PRIVATE
    ${emscripten_link_options})
  
  set(script_files
    index.html
  )
  
  foreach(script_file ${script_files})
    set(copyTarget ${LIBRARY_NAME}_copy_${script_file})
    add_custom_target(
      ${copyTarget}
      COMMAND
        ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/${script_file}"
        "${CMAKE_CURRENT_BINARY_DIR}")
    add_dependencies(${LIBRARY_NAME} ${copyTarget})
  endforeach()  
else()
  add_executable(${TARGET_NAME} main.cxx)
  message("${LIBRARY_NAME}")
  target_link_libraries(${TARGET_NAME} PRIVATE ${LIBRARY_NAME}) 
endif()

