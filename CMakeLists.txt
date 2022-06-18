cmake_minimum_required(VERSION 3.20)
project(TiktokGen)

set(CMAKE_CXX_STANDARD 20)
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

set(OpenCV_DIR ${LIB_DIR}/opencv_build)
#include_directories(${OPENCV_DIR}/include)
#add_subdirectory(${OPENCV_DIR})
find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})

set(PLOG_DIR ${LIB_DIR}/plog)
include_directories(${PLOG_DIR}/include)

add_library(IMGUI STATIC src/model/generators/Generator.h src/view/utils/SDL_GLContextDeleter.h src/model/generators/frame_generators/utils/GeneratorRequirementType.h)
set(IMGUI_DIR ${LIB_DIR}/imgui)
set(IMGUI_FILE_DIR ${LIB_DIR}/imgui-filebrowser)
include_directories(${IMGUI_DIR})
target_sources( IMGUI
        PRIVATE
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/misc/cpp/imgui_stdlib.cpp

        PRIVATE
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
        ${IMGUI_DIR}/backends/imgui_impl_sdl.cpp
        )

target_include_directories( IMGUI
        PUBLIC ${IMGUI_DIR}
        PUBLIC ${IMGUI_DIR}/backends
        PUBLIC ${IMGUI_DIR}/misc/cpp
        PUBLIC ${SDL2_DIR}/include
        PUBLIC ${IMGUI_FILE_DIR}/
        )
target_link_libraries(IMGUI glfw SDL2)

find_package(OpenGL REQUIRED)
link_libraries(IMGUI glfw)
add_executable(TiktokGen main.cpp src/model/generators/frame_generators/FrameGenerator.cpp src/model/generators/frame_generators/FrameGenerator.h src/view/Renderer.cpp src/view/Renderer.h src/view/utils/SDLWindowDeleter.h src/model/user/UserState.cpp src/model/user/UserState.h src/view_model/MainViewModel.cpp src/view_model/MainViewModel.h src/model/generators/frame_generators/utils/GeneratorName.h src/view/MainView.cpp src/view/MainView.h src/model/generators/frame_generators/TextVideoGenerator.cpp src/model/generators/frame_generators/TextVideoGenerator.h src/model/generators/frame_generators/utils/GeneratorRequirement.h src/view_model/utils/LogFunctions.h)
