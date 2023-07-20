
cmake_minimum_required (VERSION 3.8)
project ("ECSOpenGLEngine" LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory("external")

add_library(${PROJECT_NAME})

target_sources(
	${PROJECT_NAME}
	PRIVATE
		src/core/ResourceManager.cpp

		src/graphics/IndexBuffer.cpp
		src/graphics/Model.cpp
		src/graphics/Shader.cpp
		src/graphics/Texture.cpp
		src/graphics/TextureArray.cpp
		src/graphics/VertexArrayBuffer.cpp
		src/graphics/VertexBuffer.cpp

		src/componentsFactory/CameraFactory.cpp
		src/componentsFactory/PrimitiveFactory.cpp
		src/componentsFactory/SpriteFactory.cpp

		src/input/Input.cpp
		src/input/InputManager.cpp

		src/scene/Camera.cpp
		src/scene/Entity.cpp
		src/scene/Scene.cpp

		src/utils/Debug.cpp
		src/utils/UUID.cpp

		src/glad/glad.c
)

if (WIN32)
	target_compile_definitions(
		${PROJECT_NAME}
		PRIVATE
			WIN32_LEAN_AND_MEAN
			NOMINMAX
	)
endif()

target_include_directories(
	${PROJECT_NAME}
	PUBLIC
		$<INSTALL_INTERFACE:include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>

		GLM_INCLUDE_DIR
		ENTT_INCLUDE_DIR
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
	SYSTEM
		GLFW_INCLUDE_DIR
		external/tinyobj
		external/stb
)

target_link_libraries(
	${PROJECT_NAME}
	PUBLIC
		glm
		EnTT
	PRIVATE
		glfw
)

add_executable(sandbox "sandbox/main.cpp")

target_include_directories(
	sandbox
	PRIVATE
		include
		GLFW_INCLUDE_DIR
)

target_link_libraries(
	sandbox
    PRIVATE
		${PROJECT_NAME}
		glfw
)

# # Define the include DIRs
# include_directories(
# 	${CMAKE_SOURCE_DIR}/src
# 	${CMAKE_SOURCE_DIR}/include
# )

# macro(add_project_files)
# 	# Add source files
# 	file(GLOB_RECURSE SOURCE_FILES
# 		${CMAKE_CURRENT_SOURCE_DIR}/*.c
# 		${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
	
# 	# Add header files
# 	file(GLOB_RECURSE HEADER_FILES
# 		${CMAKE_CURRENT_SOURCE_DIR}/*.h
# 		${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
# endmacro()

# macro(copy_dlls proj_name)
# 	if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
# 		set(DLL_PATH "dlls/debug")
# 	endif()
# 	if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
# 		set(DLL_PATH "dlls/release")
# 	endif()
# 	message(STATUS "Found DLLs path: ${PROJECT_SOURCE_DIR}/${DLL_PATH}")
# 	add_custom_command(TARGET ${ARGV0} POST_BUILD
# 		COMMAND ${CMAKE_COMMAND} -E copy_directory
# 		${PROJECT_SOURCE_DIR}/${DLL_PATH}
# 		$<TARGET_FILE_DIR:${ARGV0}>)
# endmacro()

# macro(copy_shaders)
# 	file(GLOB_RECURSE SHADER_FILES
# 		${CMAKE_CURRENT_SOURCE_DIR}/*.vert
# 		${CMAKE_CURRENT_SOURCE_DIR}/*.frag
# 		${CMAKE_CURRENT_SOURCE_DIR}/*.geom
# 		${CMAKE_CURRENT_SOURCE_DIR}/*.glsl)
# 	FILE(COPY ${SHADER_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# endmacro()

# # Include sub-projects.
# add_subdirectory("src/Sandbox")

