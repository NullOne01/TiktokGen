cmake_minimum_required(VERSION 3.20)
project(TiktokGen)

set(CMAKE_CXX_STANDARD 14)
if(APPLE)
    enable_language(OBJC)
endif()

set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/modules)

set(GLFW_DIR ${LIB_DIR}/glfw)
include_directories(${GLFW_DIR})
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${GLFW_DIR})

set(SDL2_DIR ${LIB_DIR}/SDL)
include_directories(${SDL2_DIR})
add_subdirectory(${SDL2_DIR})

add_library(IMGUI STATIC)
set(IMGUI_DIR ${LIB_DIR}/imgui)
include_directories(${IMGUI_DIR})
target_sources( IMGUI
        PRIVATE
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/imgui.cpp

        PRIVATE
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
        ${IMGUI_DIR}/backends/imgui_impl_sdl.cpp
        )

target_include_directories( IMGUI
        PUBLIC ${IMGUI_DIR}
        PUBLIC ${IMGUI_DIR}/backends
        PUBLIC ${SDL2_DIR}/include
        )
target_link_libraries(IMGUI glfw SDL2)

find_package(OpenGL REQUIRED)
link_libraries(IMGUI glfw)
add_executable(TiktokGen main.cpp)
