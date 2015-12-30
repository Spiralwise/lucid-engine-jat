# Lucid Engine - CMakeLists #
cmake_minimum_required(VERSION 2.8)
project(Lucid C CXX)

# Settings
set(EXECUTABLE_OUTPUT_PATH build/${CMAKE_BUILD_TYPE})
#set(GLFW_LINK_DIR ./libraries/glfw/lib-mingw)

# Package requirements
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

# Sources
file(
	GLOB_RECURSE
	sources
	lucid/*
)

# Includes
include_directories(
	./includes
	${GLFW_INCLUDE_DIRS}
)

# Libraries
target_link_libraries(
	${PROJECT_NAME}
	${GLFW_LIBRARIES}
)

# Executable
add_executable(
	${PROJECT_NAME}
	${sources}
)
