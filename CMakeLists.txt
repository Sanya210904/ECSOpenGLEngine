# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)
project ("ECSOpenGLEngine")

link_directories(${CMAKE_SOURCE_DIR}/lib)

configure_file("src/helpers/RootDir.h.in" "src/helpers/RootDir.h")
include_directories("${CMAKE_BINARY_DIR}/src")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# GLM
find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")

# GLFW
find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")

# IMGUI
find_package(IMGUI REQUIRED)
message(STATUS "IMGUI included at ${IMGUI_INCLUDE_DIR}")
file(GLOB_RECURSE IMGUI
	${IMGUI_INCLUDE_DIR}/imgui/*)

# GLAD
file(GLOB_RECURSE GLAD
	${CMAKE_SOURCE_DIR}/include/glad/*)

# Put all libraries into a variable
set(LIBS ${GLFW3_LIBRARY} ${OPENGL_LIBRARY} ${CMAKE_DL_LIBS})

# Define the include DIRs
include_directories(
	${CMAKE_SOURCE_DIR}/src
	${CMAKE_SOURCE_DIR}/include
)

# FILE(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

macro(add_project_files)
	# Add source files
	file(GLOB_RECURSE SOURCE_FILES
		${CMAKE_CURRENT_SOURCE_DIR}/*.c
		${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
	
	# Add header files
	file(GLOB_RECURSE HEADER_FILES
		${CMAKE_CURRENT_SOURCE_DIR}/*.h
		${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
endmacro()

macro(copy_dlls proj_name)
	if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
		set(DLL_PATH "dlls/debug")
	endif()
	if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
		set(DLL_PATH "dlls/release")
	endif()
	message(STATUS "Found DLLs path: ${PROJECT_SOURCE_DIR}/${DLL_PATH}")
	add_custom_command(TARGET ${ARGV0} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${PROJECT_SOURCE_DIR}/${DLL_PATH}
		$<TARGET_FILE_DIR:${ARGV0}>)
endmacro()

macro(copy_shaders)
	file(GLOB_RECURSE SHADER_FILES
		${CMAKE_CURRENT_SOURCE_DIR}/*.vert
		${CMAKE_CURRENT_SOURCE_DIR}/*.frag
		${CMAKE_CURRENT_SOURCE_DIR}/*.geom
		${CMAKE_CURRENT_SOURCE_DIR}/*.glsl)
	FILE(COPY ${SHADER_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endmacro()

# Include sub-projects.
add_subdirectory("src/Sandbox")

