cmake_minimum_required(VERSION 3.3)
project(dui VERSION 0.3)

option(SINGLE_HEADER "Produce single header" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

if (NOT WIN32)
  add_definitions(-Wall -pedantic)
endif()

# Find SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_library(dui INTERFACE)
target_include_directories(dui INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include/dui/)
target_link_libraries(dui INTERFACE ${SDL2_LIBRARIES})
target_compile_features(dui INTERFACE cxx_std_17)

add_executable(elements_demo examples/elements_demo.cpp)
target_link_libraries(elements_demo PRIVATE dui)
add_executable(focus_demo examples/focus_demo.cpp)
target_link_libraries(focus_demo PRIVATE dui)
add_executable(hello_demo examples/hello_demo.cpp)
target_link_libraries(hello_demo PRIVATE dui)
add_executable(scrolling_demo examples/scrolling_demo.cpp)
target_link_libraries(scrolling_demo PRIVATE dui)

if (SINGLE_HEADER)
  add_custom_target(single_header ALL
    node ${CMAKE_CURRENT_SOURCE_DIR}/makeSingleHeader.js ${CMAKE_CURRENT_BINARY_DIR}/dui.hpp
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/dui/
    COMMENT "Generating single header file"
    VERBATIM
  )
endif()

# Build documentation
find_package(Doxygen)
if(DOXYGEN_FOUND)
  add_custom_target(docs
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/docs
    COMMENT "Generating API documentation with Doxygen" 
    VERBATIM
  )
endif()
