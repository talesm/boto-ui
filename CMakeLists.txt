cmake_minimum_required(VERSION 3.3)
project(dui VERSION 0.2.2)

add_definitions(-Wall -pedantic)

find_package(PkgConfig REQUIRED)
# if(PKGCONFIG_FOUND)
pkg_search_module(SDL2 REQUIRED IMPORTED_TARGET SDL2>=2.0.8 sdl2>=2.0.8)
# pkg_search_module(SDL2_gfx REQUIRED IMPORTED_TARGET SDL2_gfx>=1.0.0)
# pkg_search_module(SDL2_image REQUIRED IMPORTED_TARGET SDL2_image>=2.0.0 SDL2_Image>=2.0.0)

add_library(dui INTERFACE)
target_include_directories(dui INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(dui INTERFACE PkgConfig::SDL2)
target_compile_features(dui INTERFACE cxx_std_17)

add_executable(elements_demo examples/elements_demo.cpp)
target_link_libraries(elements_demo PRIVATE dui)
add_executable(focus_demo examples/focus_demo.cpp)
target_link_libraries(focus_demo PRIVATE dui)
add_executable(hello_demo examples/hello_demo.cpp)
target_link_libraries(hello_demo PRIVATE dui)

add_custom_target(single_header
  node ${CMAKE_CURRENT_SOURCE_DIR}/makeSingleHeader.js
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "Generating single header file" 
  VERBATIM
)

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
