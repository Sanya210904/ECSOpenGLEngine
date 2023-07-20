# cmake_minimum_required (VERSION 3.8)

# add_project_files()

# # Add source to this project's executable.
# file(GLOB_RECURSE GRAPHICS
# 	${CMAKE_SOURCE_DIR}/src/graphics/*.cpp
# 	${CMAKE_SOURCE_DIR}/src/graphics/*.hpp)

# file(GLOB_RECURSE COMPONENTS
# 	${CMAKE_SOURCE_DIR}/src/components/*.cpp
# 	${CMAKE_SOURCE_DIR}/src/components/*.hpp)
	
# file(GLOB_RECURSE COMPONENTS_FACTORY
# 	${CMAKE_SOURCE_DIR}/src/componentsFactory/*.cpp
# 	${CMAKE_SOURCE_DIR}/src/componentsFactory/*.hpp)

# file(GLOB_RECURSE SCENE
# 	${CMAKE_SOURCE_DIR}/src/scene/*.cpp
# 	${CMAKE_SOURCE_DIR}/src/scene/*.hpp)

# file(GLOB_RECURSE UTILS
# 	${CMAKE_SOURCE_DIR}/src/utils/*.cpp
# 	${CMAKE_SOURCE_DIR}/src/utils/*.hpp)

# file(GLOB_RECURSE INPUT
# 	${CMAKE_SOURCE_DIR}/src/input/*.cpp
# 	${CMAKE_SOURCE_DIR}/src/input/*.hpp)

# file(GLOB_RECURSE CORE
# 	${CMAKE_SOURCE_DIR}/src/core/*.cpp
# 	${CMAKE_SOURCE_DIR}/src/core/*.hpp)

# add_executable(Sandbox ${HEADER_FILES} ${SOURCE_FILES} ${GLAD} ${GRAPHICS} ${COMPONENTS} ${COMPONENTS_FACTORY} ${SCENE} ${UTILS} ${INPUT} ${CORE})

# set_property(TARGET Sandbox PROPERTY CXX_STANDARD 17)

# target_link_libraries(Sandbox ${LIBS})

# # Copy dlls
# if(WIN32)
# 	copy_dlls(Sandbox)
# endif()

# # copy_shaders()
