#[==[.rst:
.. cmake:command:: emscripten_settings
Set various variables for Emscripten
.. code-block:: cmake
emscripten_settings(
  TRHEADING_ENABLED             <ON|OFF>
  EMBIND                        <ON|OFF>
  ES6_MODULE                    <ON|OFF>
  EXPORT_NAME                   <variable>
  OPTIMIZATION                  <NONE, LITTLE, MORE, BEST, SMALL, SMALLEST, SMALLEST_WITH_CLOSURE>
  DEBUG                         <NONE, READABLE_JS, PROFILE, DEBUG_NATIVE>
  EMSCRIPTEN_DEBUG_INFO         <variable>
  EMSCRIPTEN_LINK_OPTIONS       <variable>
  EMSCRIPTEN_OPTIMIZATION_FLAGS <variable>)

We can add more input/output variables. Note, this is not what we like
for our production code, e.g. the export _DoWork or the fixed number of
threads.
#]==]

function(emscripten_settings)
  # Define the arguments that the function accepts
  set(options)  # Boolean options (without ON/OFF). We have none
  set(one_value_args
    THREADING_ENABLED
    EMBIND
    ES6_MODULE
    EXPORT_NAME
    OPTIMIZATION
    DEBUG
    EMSCRIPTEN_LINK_OPTIONS
    EMSCRIPTEN_OPTIMIZATION_FLAGS
    EMSCRIPTEN_DEBUG_INFO
  )
  set(multi_value_args
    SIDE_MODULES
  )

  # Parse the arguments using cmake_parse_arguments
  cmake_parse_arguments(ARGS "${options}" "${one_value_args}" "${multi_value_args}" ${ARGV})

  # Validate required arguments
  if (NOT ARGS_EMSCRIPTEN_LINK_OPTIONS)
    message(FATAL_ERROR "EMSCRIPTEN_LINK_OPTIONS must be specified.")
  endif()

  if (NOT ARGS_EMSCRIPTEN_OPTIMIZATION_FLAGS)
    message(FATAL_ERROR "EMSCRIPTEN_OPTIMIZATION_FLAGS must be specified.")
  endif()

  if (NOT ARGS_EMSCRIPTEN_DEBUG_INFO)
    message(FATAL_ERROR "EMSCRIPTEN_OPTIMIZATION_FLAGS must be specified.")
  endif()
  
  # Default values  
  if (NOT DEFINED ARGS_THREADING_ENABLED)
    set(ARGS_THREADING_ENABLED OFF) 
  endif()
  if (NOT DEFINED ARGS_ES6_MODULE)
    set(ARGS_ES6_MODULE ON)
  endif()
  if (NOT DEFINED ARGS_EMBIND)
    set(ARGS_EMBIND OFF)
  endif()
  if (NOT DEFINED ARGS_OPTIMIZATION)
    set(ARGS_OPTIMIZATION "NONE")
  endif()
  if (NOT DEFINED ARGS_DEBUG)
    set(ARGS_DEBUG "NONE")
  endif()

  # Define valid options for OPTIMIZATION
  set(valid_optimization_levels NONE LITTLE MORE BEST SMALL SMALLEST SMALLEST_WITH_CLOSURE)
  
  # Validate OPTIMIZATION argument
  list(FIND valid_optimization_levels "${ARGS_OPTIMIZATION}" opt_index)
  if (opt_index EQUAL -1)
    message(FATAL_ERROR "Invalid value for OPTIMIZATION. Must be one of NONE, LITTLE, or MORE.")
  endif()

  # Define valid options for DEBUG
  set(valid_debug_levels NONE READABLE_JS PROFILE DEBUG_NATIVE SOURCE_MAPS)
  
  # Validate DEBUG argument
  list(FIND valid_debug_levels "${ARGS_DEBUG}" opt_index)
  if (opt_index EQUAL -1)
    message(FATAL_ERROR "Invalid value for DEBUG. Must be one of NONE, READABLE_JS, PROFILE, DEBUG_NATIVE or SOURCE_MAPS")
  endif()

  # Populate lists
  set(emscripten_debug_info)
  set(emscripten_link_options)
  
  # Set the optimization flags based on OPTIMIZATION value
  if (ARGS_OPTIMIZATION STREQUAL "NONE")
    set(emscripten_optimization_flags "-O0")
  elseif (ARGS_OPTIMIZATION STREQUAL "LITTLE")
    set(emscripten_optimization_flags "-O1")
  elseif (ARGS_OPTIMIZATION STREQUAL "MORE")
    set(emscripten_optimization_flags "-O2")
  elseif(ARGS_OPTIMIZATION STREQUAL "BEST")
    list(APPEND emscripten_optimization_flags
      "-O3")
  elseif(ARGS_OPTIMIZATION STREQUAL "SMALL")
    list(APPEND emscripten_optimization_flags
      "-Os")
  elseif(ARGS_OPTIMIZATION STREQUAL "SMALLEST")
    list(APPEND emscripten_optimization_flags
      "-Oz")
  elseif(ARGS_OPTIMIZATION STREQUAL "SMALLEST_WITH_CLOSURE")
    list(APPEND emscripten_optimization_flags
      "-Oz")
    list(APPEND emscripten_link_options
      "--closure 1")
  endif()

  # Set the debug flags based on DEBUG value
  if(ARGS_DEBUG STREQUAL "NONE")
    list(APPEND emscripten_debug_info
      "-g0")
  elseif(ARGS_DEBUG STREQUAL "READABLE_JS")
    list(APPEND emscripten_debug_info
      "-g1")
  elseif(ARGS_DEBUG STREQUAL "PROFILE")
    list(APPEND emscripten_debug_info
      "-g2")
  elseif(ARGS_DEBUG STREQUAL "DEBUG_NATIVE")
    list(APPEND emscripten_debug_info
      "-g3")
    list(APPEND emscripten_link_options
      "-sASSERTIONS=1")
  elseif(ARGS_DEBUG STREQUAL "SOURCE_MAPS")
    list(APPEND emscripten_debug_info
      "-gsource-map")
  endif()
  
  list(APPEND emscripten_link_options
    "-sASSERTIONS=1"
    "-sERROR_ON_UNDEFINED_SYMBOLS=1"
    "-sNO_EXIT_RUNTIME=0")

  if (ARGS_EMBIND STREQUAL "ON")
    list(APPEND emscripten_link_options
      "-lembind")
  endif()

  if (ARGS_ES6_MODULE STREQUAL "ON")
    list(APPEND emscripten_link_options
      "-sMODULARIZE=1"
      "-sEXPORT_ES6=1"
      "-sEXPORTED_FUNCTIONS=['_free', '_malloc']"
      "-sEXPORTED_RUNTIME_METHODS=['ENV', 'FS', 'ccall', 'stringToNewUTF8', 'addFunction']"
      "-sINCLUDE_FULL_LIBRARY"
      "-sALLOW_TABLE_GROWTH=1"
      "-sALLOW_MEMORY_GROWTH=1"
      "-sEXPORT_NAME=${ARGS_EXPORT_NAME}")
    if (ARGS_THREADING_ENABLED STREQUAL "ON")
      list(APPEND emscripten_link_options
        "-sENVIRONMENT=web,node,worker"
      )
    else()
      list(APPEND emscripten_link_options
        "-sENVIRONMENT=web,node"
      )
    endif()
    # Copy package-json
    set(node_files
      package.json
      package-lock.json
    )
  
    foreach(node_file ${node_files})
      if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${node_file}")
        add_custom_command(
          TARGET ${TARGET_NAME}
          COMMAND
          ${CMAKE_COMMAND} -E copy_if_different
          "${CMAKE_CURRENT_SOURCE_DIR}/${node_file}"
          "${CMAKE_CURRENT_BINARY_DIR}")
      endif()
    endforeach()

    add_custom_command(
      TARGET ${TARGET_NAME}
      POST_BUILD
      COMMAND
        npm install
      WORKING_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR})
  else()
    if (ARGS_THREADING_ENABLED STREQUAL "ON")
      list(APPEND emscripten_link_options
        "-sENVIRONMENT=node,worker"
      )  
    else()
      list(APPEND emscripten_link_options
        "-sENVIRONMENT=node"
      )  
    endif()
  endif()
  if (ARGS_THREADING_ENABLED STREQUAL "ON")
    list(APPEND emscripten_link_options
      "-sUSE_PTHREADS=1"
      "-sSHARED_MEMORY=1"
      "-sPTHREAD_POOL_SIZE_STRICT=4")
  endif()

  # Assign the options list to the specified variable
  set(${ARGS_EMSCRIPTEN_LINK_OPTIONS} "${emscripten_link_options}" PARENT_SCOPE)
  set(${ARGS_EMSCRIPTEN_OPTIMIZATION_FLAGS} "${emscripten_optimization_flags}" PARENT_SCOPE)
  set(${ARGS_EMSCRIPTEN_DEBUG_INFO} "${emscripten_debug_info}" PARENT_SCOPE)
endfunction()

#[==[.rst:
.. cmake:command:: emscripten_module
Create a WASM Emscripten module
.. code-block:: cmake
emscripten_module(
  TARGET_NAME                   <variable>
  SOURCE_FILES                  <list>
  JAVASCRIPT_FILES              <list>
  TRHEADING_ENABLED             <ON|OFF>
  EMBIND                        <ON|OFF>
  ES6_MODULE                    <ON|OFF>
  EXPORT_NAME                   <variable>
  OPTIMIZATION                  <NONE, LITTLE, MORE, BEST, SMALL, SMALLEST, SMALLEST_WITH_CLOSURE>
  DEBUG                         <NONE, READABLE_JS, PROFILE, DEBUG_NATIVE>)

#]==]

function(emscripten_module)
  # Define the arguments that the function accepts
  set(options)
  set(one_value_args TARGET_NAME ES6_MODULE EMBIND EXPORT_NAME DEBUG OPTIMIZATION)
  set(multi_value_args SOURCE_FILES JAVASCRIPT_FILES)

  # Parse the arguments using cmake_parse_arguments
  cmake_parse_arguments(ARGS "${options}" "${one_value_args}" "${multi_value_args}" ${ARGV})

  # Validate required arguments
  if (NOT ARGS_TARGET_NAME)
    message(FATAL_ERROR "TARGET_NAME must be specified.")
  endif()
  if (NOT ARGS_SOURCE_FILES)
    message(FATAL_ERROR "SOURCE_FILES must be specified.")
  endif()

  # Add executable
  add_executable(${ARGS_TARGET_NAME} ${ARGS_SOURCE_FILES})

  # Prepare variables for emscripten_settings
  set(emscripten_link_options)
  set(emscripten_optimization_flags)
  set(emscripten_debug_info)

  # Call emscripten_settings with the provided arguments
  emscripten_settings(
    ES6_MODULE ${ARGS_ES6_MODULE}
    EMBIND ${ARGS_EMBIND}
    EXPORT_NAME ${ARGS_EXPORT_NAME}
    DEBUG ${ARGS_DEBUG}
    OPTIMIZATION ${ARGS_OPTIMIZATION}
    EMSCRIPTEN_LINK_OPTIONS emscripten_link_options
    EMSCRIPTEN_OPTIMIZATION_FLAGS emscripten_optimization_flags
    EMSCRIPTEN_DEBUG_INFO emscripten_debug_info
  )

  # Link and compile options
  target_link_options(${ARGS_TARGET_NAME} PRIVATE ${emscripten_link_options})
  target_compile_options(${ARGS_TARGET_NAME} PRIVATE ${emscripten_optimization_flags})

  foreach(javascript_file ${ARGS_JAVASCRIPT_FILES})
    set(copyTarget ${TARGET_NAME}_copy_${javascript_file})
    add_custom_target(
      ${copyTarget}
      COMMAND
      ${CMAKE_COMMAND} -E copy_if_different
      "${CMAKE_CURRENT_SOURCE_DIR}/${javascript_file}"
      "${CMAKE_CURRENT_BINARY_DIR}")
    add_dependencies(${TARGET_NAME} ${copyTarget})
  endforeach()
endfunction()
