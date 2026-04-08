########################
##
##  Created: Wed Apr 08 2026
##  File: CMakeLists.txt
##
########################

cmake_minimum_required(VERSION 3.28)
project(ECS LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DARC_DEBUG -O0 -g")

add_executable(ecs
	src/main.cpp
)

target_include_directories(ecs
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(ecs PUBLIC cxx_std_23)

if (MSVC)
	target_compile_options(ecs PRIVATE /W4 /permissive-)
else()
	target_compile_options(ecs PRIVATE -Wall -Wextra -Wpedantic -Werror -O3)
endif()
