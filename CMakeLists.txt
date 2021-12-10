cmake_minimum_required(VERSION 3.3)
project(boto-ui)

add_definitions(-Wall -pedantic)

find_package(PkgConfig REQUIRED)
# if(PKGCONFIG_FOUND)
pkg_search_module(SDL2 REQUIRED IMPORTED_TARGET SDL2>=2.0.8 sdl2>=2.0.8)
# pkg_search_module(SDL2_gfx REQUIRED IMPORTED_TARGET SDL2_gfx>=1.0.0)
# pkg_search_module(SDL2_image REQUIRED IMPORTED_TARGET SDL2_image>=2.0.0 SDL2_Image>=2.0.0)

add_library(boto-ui INTERFACE)
target_include_directories(boto-ui INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include/boto-ui/)
target_link_libraries(boto-ui INTERFACE PkgConfig::SDL2)
target_compile_features(boto-ui INTERFACE cxx_std_17)

add_executable(elements_demo examples/elements_demo.cpp)
target_link_libraries(elements_demo PRIVATE boto-ui)
add_executable(focus_demo examples/focus_demo.cpp)
target_link_libraries(focus_demo PRIVATE boto-ui)
add_executable(hello_demo examples/hello_demo.cpp)
target_link_libraries(hello_demo PRIVATE boto-ui)
add_executable(scrolling_demo examples/scrolling_demo.cpp)
target_link_libraries(scrolling_demo PRIVATE boto-ui)

add_custom_target(single_header ALL
  node ${CMAKE_CURRENT_SOURCE_DIR}/makeSingleHeader.js ${CMAKE_CURRENT_BINARY_DIR}/boto-ui.hpp
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/boto-ui/
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
