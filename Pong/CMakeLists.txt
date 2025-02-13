#TODO: Update
cmake_minimum_required(VERSION 3.22...3.28)

project(Pong)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../CMake)
include(spsEmscriptenDefaults)

if (NOT DEFINED WASMExamples_DEBUG)
  sps_set_emscripten_defaults(WASMExamples)
endif()
include(spsEmscriptenSettings)
include(spsConfig)

set(TARGET_NAME pong_wasm)

find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (EMSCRIPTEN)
  sps_emscripten_module(
    VERBOSE
    FETCH
    SDL2
    SDL_TTF
    SDL_MIXER
    EMBIND ON
    FILE_SYSTEM ON    
    ENABLE_EXCEPTIONS ON
    ENVIRONMENT NONE
    ES6_MODULE OFF
    THREADING_ENABLED OFF
    THREAD_POOL_SIZE 4
    TARGET_NAME ${TARGET_NAME}
    SOURCE_FILES pong.cpp
    JAVASCRIPT_FILES pong.html pong.js
    LIBRARIES idbfs.js;boost_headers
    EXTRA_LINK_ARGS -s\ USE_BOOST_HEADERS=1\ --preload-file\ fonts/\ --preload-file\ sounds/
    OPTIMIZATION ${WASMExamples_TEST_OPTIMIZATION}
    DEBUG ${WASMExamples_TEST_DEBUG}
  )
endif()

set(OUTPUT_DIR $<TARGET_FILE_DIR:${TARGET_NAME}>)

add_custom_command(
    TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/fonts"
    "${OUTPUT_DIR}/fonts"
    COMMENT "Copying source_folder to output directory")

add_custom_command(
    TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/sounds"
    "${OUTPUT_DIR}/sounds"
    COMMENT "Copying source_folder to output directory")
