# Lucid Engine - CMakeLists #
cmake_minimum_required(VERSION 2.8)
project(Lucid CXX)
set(EXECUTABLE_OUTPUT_PATH lucid-build/${CMAKE_BUILD_TYPE})
find_package(OpenGL REQUIRED)

# Sources
file(
	GLOB_RECURSE
	sources
	lucid/*
)

# Libraries
include_directories(
	${GLEW_INCLUDE_DIR}
	#./libraries/glew/include
	${GLFW_INCLUDE_DIR}
	#./libraries/glfw/include
	${GLM_INCLUDE_DIR}
	#./libraries/glm
)
link_directories(
	${GLEW_LINK_DIR}
	#./libraries/glew/lib
	${GLFW_LINK_DIR}
	#./libraries/glfw/lib-mingw
)

# Executable
add_executable(
	${PROJECT_NAME}
	${sources}
)
target_link_libraries(
	${PROJECT_NAME}
	openGL32
	glew32.dll
	glfw3dll
)