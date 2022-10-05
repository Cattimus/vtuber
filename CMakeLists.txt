cmake_minimum_required(VERSION 2.15)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(main)

set(SOURCES
	src/main.cpp
	src/state.hpp
	src/state.cpp

	src/primitives/avatar.hpp
	src/primitives/avatar.cpp

	src/primitives/entity.hpp
	src/primitives/entity.cpp

	src/primitives/object.hpp
	src/primitives/object.cpp

	src/primitives/texture.hpp

	src/user_input/mic.hpp
	src/user_input/console.hpp

	src/lua_bindings/object_binding.hpp
	src/lua_bindings/object_binding.cpp

	src/lua_bindings/script.hpp
	src/lua_bindings/script.cpp
	)

include_directories(libs)
include_directories(src)
include_directories(/usr/include/SDL2)
include_directories(/usr/include/luajit-2.1)

add_compile_options(-g -Wall -D_GLIBCXX_DEBUG)

add_executable(main ${SOURCES})
target_link_libraries(main ${CMAKE_DL_LIBS})
target_link_libraries(main SDL2)
target_link_libraries(main SDL2_image)
target_link_libraries(main pulse pulse-simple pthread)
target_link_libraries(main luajit-5.1)
