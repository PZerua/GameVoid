cmake_minimum_required(VERSION 3.12)

# Directories
set(GV_DIR_ROOT       ${CMAKE_CURRENT_LIST_DIR})
set(GV_DIR_LIBRARIES  "${GV_DIR_ROOT}/libraries")
set(GV_DIR_SOURCES    "${GV_DIR_ROOT}/src")

project (GameVoid CXX)

# Enable multicore and simd compile on VS solution
if(MSVC)
    add_definitions(/MP)
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

GroupSources(src)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "build")

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

add_executable(${PROJECT_NAME} ${GV_SOURCES})

if(MSVC)
    set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
endif()

target_include_directories(${PROJECT_NAME} PUBLIC ${GV_DIR_SOURCES})

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)

# glfw
add_subdirectory(libraries/glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
set_property(TARGET glfw PROPERTY FOLDER "External/imgui")

# glew
add_subdirectory(libraries/glew-cmake)
add_definitions(-DGLEW_STATIC)
target_link_libraries(${PROJECT_NAME} PRIVATE libglew_static)
set_property(TARGET libglew_static PROPERTY FOLDER "External/imgui")

# imgui
add_library(imgui STATIC
    libraries/imgui/imgui.cpp
    libraries/imgui/imgui_demo.cpp
    libraries/imgui/imgui_draw.cpp
    libraries/imgui/imgui_widgets.cpp
    libraries/imgui/imgui_tables.cpp
    libraries/imgui/backends/imgui_impl_glfw.cpp
    libraries/imgui/backends/imgui_impl_glfw.h
    libraries/imgui/backends/imgui_impl_opengl3.cpp
    libraries/imgui/backends/imgui_impl_opengl3.h
    libraries/imgui/imconfig.h
    libraries/imgui/imgui.h
    libraries/imgui/imgui_internal.h
    libraries/imgui/imstb_rectpack.h
    libraries/imgui/imstb_textedit.h
    libraries/imgui/imstb_truetype.h
)

set_property(TARGET imgui PROPERTY CXX_STANDARD 17)

target_link_libraries(imgui PUBLIC glfw)
target_include_directories(imgui PUBLIC libraries/imgui)
target_link_libraries(${PROJECT_NAME} PUBLIC imgui)
set_property(TARGET imgui PROPERTY FOLDER "External/imgui")

# Properties
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD_REQUIRED ON)
