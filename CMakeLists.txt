cmake_minimum_required(VERSION 3.0)
project (GameVoid)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "build")

add_subdirectory(libraries/glfw EXCLUDE_FROM_ALL)
add_subdirectory(libraries/glew-cmake EXCLUDE_FROM_ALL)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif()

# Macro to map filters to folder structure for MSVC projects
macro(GroupSources curdir)
    if(MSVC)
        file(GLOB children RELATIVE ${PROJECT_SOURCE_DIR}/${curdir} ${PROJECT_SOURCE_DIR}/${curdir}/*)

        foreach(child ${children})
            if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${curdir}/${child})
                GroupSources(${curdir}/${child})
            else()
                string(REPLACE "/" "\\" groupname ${curdir})
                source_group(${groupname} FILES ${PROJECT_SOURCE_DIR}/${curdir}/${child})
            endif()
        endforeach()
    endif()
endmacro()

include_directories("src")
include_directories("src/utils")
include_directories("src/graphics")
include_directories("src/graphics/buffers")
include_directories("src/input")
include_directories("src/gb")
include_directories("libraries/stb")

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/utils/*.cpp src/graphics/*.cpp src/graphics/buffers/*.cpp src/input/*.cpp src/gb/*.cpp)
file(GLOB_RECURSE HEADER_FILES src/*.h src/utils/*.h src/graphics/*.h src/graphics/buffers/*.h src/input/*.h src/gb/*.h)

GroupSources(src)

add_executable(GameVoid ${SOURCE_FILES} ${HEADER_FILES})

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT GameVoid)
set_property(TARGET GameVoid PROPERTY CXX_STANDARD 11)

# glfw
target_link_libraries(GameVoid PRIVATE glfw)

# glew
add_definitions(-DGLEW_STATIC)
target_link_libraries(GameVoid PRIVATE libglew_static)
