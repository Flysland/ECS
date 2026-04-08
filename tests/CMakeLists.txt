########################
##
##  Created: Wed Apr 08 2026
##  File: CMakeLists.txt
##
########################

add_executable(ecs-tests
	src/main.cpp
)

target_link_libraries(ecs-tests PRIVATE ecs)
add_test(NAME TESTS COMMAND ecs-tests)

if (MSVC)
	target_compile_options(ecs-tests PRIVATE /W4 /permissive-)
else()
	target_compile_options(ecs-tests PRIVATE -Wall -Wextra -Wpedantic -Werror -O3)
endif()
