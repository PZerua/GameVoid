cmake_minimum_required(VERSION 3.12)

# Directories
set(GV_DIR_ROOT       ${CMAKE_CURRENT_LIST_DIR})
set(GV_DIR_LIBRARIES  "${GV_DIR_ROOT}/externals")
set(GV_DIR_SOURCES    "${GV_DIR_ROOT}/src")

project (GameVoid CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "build")

add_subdirectory(libraries/glfw EXCLUDE_FROM_ALL)
add_subdirectory(libraries/glew-cmake EXCLUDE_FROM_ALL)

# Sources
macro(GV_FILES_APPEND)
    file(GLOB FILES_APPEND CONFIGURE_DEPENDS ${ARGV})
    list(APPEND GV_SOURCES ${FILES_APPEND})
endmacro()
macro(GV_SOURCES_APPEND)
    GV_FILES_APPEND(${ARGV0}/*.h)
    GV_FILES_APPEND(${ARGV0}/*.cpp)
endmacro()

GV_SOURCES_APPEND(${GV_DIR_SOURCES})
GV_SOURCES_APPEND(${GV_DIR_SOURCES}/gb)
GV_SOURCES_APPEND(${GV_DIR_SOURCES}/gb/mbc)
GV_SOURCES_APPEND(${GV_DIR_SOURCES}/graphics)
GV_SOURCES_APPEND(${GV_DIR_SOURCES}/graphics/buffers)
GV_SOURCES_APPEND(${GV_DIR_SOURCES}/input)
GV_SOURCES_APPEND(${GV_DIR_SOURCES}/utils)

add_executable(GameVoid ${GV_SOURCES})

target_include_directories(GameVoid PUBLIC ${GV_DIR_SOURCES})

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT GameVoid)
set_property(TARGET GameVoid PROPERTY CXX_STANDARD 11)

# glfw
target_link_libraries(GameVoid PRIVATE glfw)

# glew
add_definitions(-DGLEW_STATIC)
target_link_libraries(GameVoid PRIVATE libglew_static)

# Properties
set_target_properties(GameVoid PROPERTIES CXX_STANDARD 11)
set_target_properties(GameVoid PROPERTIES CXX_STANDARD_REQUIRED ON)
