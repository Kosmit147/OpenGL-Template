cmake_minimum_required(VERSION 3.24)

include(cmake/warnings.cmake)

project(example VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS Off)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE On)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO On)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_MINSIZEREL On)

set(PROJECT_SOURCES
    src/main.cpp
    src/io/file_io.cpp
    src/gl/shader.cpp
    src/gl/texture.cpp
    src/window/gl_window.cpp
)

add_executable(
    example
    ${PROJECT_SOURCES}
)

if (CMAKE_BUILD_TYPE MATCHES "Debug")
    add_compile_definitions(_DEBUG)
elseif(CMAKE_BUILD_TYPE MATCHES "Release" OR CMAKE_BUILD_TYPE MATCHES "MinSizeRel" OR CMAKE_BUILD_TYPE MATCHES "RelWithDebInfo")
    add_compile_definitions(_RELEASE)
endif()

set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")
option(GLFW_BUILD_X11 OFF)
option(GLFW_BUILD_WAYLAND OFF)

if(NOT WIN32 AND NOT GLFW_BUILD_X11 AND NOT GLFW_BUILD_WAYLAND)
    message(FATAL_ERROR "You need to specify either GLFW_BUILD_X11 or GLFW_BUILD_WAYLAND on Linux")
endif()

if(GLFW_BUILD_WAYLAND)
    target_compile_definitions(example PRIVATE _GLFW_WAYLAND)
elseif(GLFW_BUILD_X11)
    target_compile_definitions(example PRIVATE _GLFW_X11)
endif()

add_subdirectory(dependencies/glad)
add_subdirectory(dependencies/glfw)
add_subdirectory(dependencies/stb_image)

target_include_directories(example PUBLIC src)
target_include_directories(example PUBLIC dependencies/glad/include)
target_include_directories(example PUBLIC dependencies/glfw/include)
target_include_directories(example PUBLIC dependencies/glm)
target_include_directories(example PUBLIC dependencies/stb_image/include)

target_link_libraries(example glad)
target_link_libraries(example glfw)
target_link_libraries(example stb_image)

target_precompile_headers(example PUBLIC src/pch.h)

set_project_warnings(PROJECT_WARNINGS)
set_property(TARGET example PROPERTY COMPILE_WARNING_AS_ERROR ON)
target_compile_options(example PRIVATE ${PROJECT_WARNINGS})
