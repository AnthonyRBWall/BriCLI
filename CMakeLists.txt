# ======================
# == Project Settings ==
# ======================

# Ensure our CMake version is met
cmake_minimum_required(VERSION  3.21)

project(BriCLI
	LANGUAGES C
	VERSION 0.0.0
	DESCRIPTION "An easy-to-use CLI (Command Line Interface) library"
)

option(${PROJECT_NAME}_BUILD_TESTS "Enable to build unit tests" ON)
option(${PROJECT_NAME}_BUILD_SAMPLES "Enable to build samples" ON)

# ===========
# == Paths ==
# ===========

set(SOURCE_DIR source)
set(INC_DIR include)
set(CONFIG_DIR config)
set(TEST_DIR test)
set(SAMPLE_DIR examples)

# =============
# == Library ==
# =============

add_library(bricli STATIC
	${SOURCE_DIR}/auth.c
	${SOURCE_DIR}/bricli.c
	${SOURCE_DIR}/colours.c
	${SOURCE_DIR}/system_commands.c
	${SOURCE_DIR}/writer.c
)
target_include_directories(bricli PRIVATE
	${SOURCE_DIR}
	${CONFIG_DIR}
)
target_include_directories(bricli PUBLIC
	${INC_DIR}
)

# Only build the test exe if we are compiling for host
if (${PROJECT_NAME}_BUILD_TESTS)
	enable_testing()
	add_subdirectory(${TEST_DIR})
endif() # ${PROJECT_NAME}_BUILD_TESTS

# Sample projects
if(${PROJECT_NAME}_BUILD_SAMPLES)
	add_subdirectory(${SAMPLE_DIR}/basic)
	add_subdirectory(${SAMPLE_DIR}/authentication)
endif() # ${PROJECT_NAME}_BUILD_SAMPLES
