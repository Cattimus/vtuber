cmake_minimum_required(VERSION 2.15)

project(main)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES
	src/main.cpp
	libs/glad/glad.c)

include_directories(libs)
include_directories(src)
include_directories(/usr/include/SDL2)

add_compile_options(-g -Wall)

add_executable(main ${SOURCES})
target_link_libraries(main ${CMAKE_DL_LIBS})
target_link_libraries(main SDL2)
target_link_libraries(main SDL2_image)
target_link_libraries(main pulse pulse-simple pthread)
