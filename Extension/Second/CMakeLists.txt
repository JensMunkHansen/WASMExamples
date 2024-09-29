if (EMSCRIPTEN)
  add_executable(second derived.cpp)
else()
  add_library(second STATIC derived.cpp)
endif()

target_link_libraries(second PUBLIC first)
