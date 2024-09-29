add_library(first STATIC base.cpp)
target_include_directories(first
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  )
