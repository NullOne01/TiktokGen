cmake_minimum_required(VERSION 3.20)

set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/modules)
set(CMAKE_TOOLCHAIN_FILE "${LIB_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")

project(TiktokGen)

set(CMAKE_CXX_STANDARD 20)
if (APPLE)
    enable_language(OBJC)
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# Set runtime resources.
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/)

set(GLFW_DIR ${LIB_DIR}/glfw)
include_directories(${GLFW_DIR})
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${GLFW_DIR})

set(FFMPEG_DIR ${LIB_DIR}/FFmpeg)
find_package(PkgConfig REQUIRED)
find_package(FFMPEG REQUIRED)
include_directories(${FFMPEG_INCLUDE_DIRS})
link_libraries(${FFMPEG_LIBRARY})
link_libraries(${FFMPEG_LIBRARIES})
link_libraries(${FFMPEG_LIBRARY_DIRS})

set(SDL2_DIR ${LIB_DIR}/SDL)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
include_directories(${SDL2_DIR})
add_subdirectory(${SDL2_DIR})

set(OpenCV_DIR ${LIB_DIR}/opencv_extra_build)
SET(WITH_FREETYPE ON CACHE BOOL "" FORCE)
#include_directories(${OPENCV_DIR}/include)
#add_subdirectory(${OPENCV_DIR})
find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${OpenCV_DIR})

set(PLOG_DIR ${LIB_DIR}/plog)
include_directories(${PLOG_DIR}/include)

add_library(IMGUI src/view/utils/TextFunctions.h)
set(IMGUI_DIR ${LIB_DIR}/imgui)
set(IMGUI_FILE_DIR ${LIB_DIR}/imgui-filebrowser)
include_directories(${IMGUI_DIR})
target_sources(IMGUI
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

target_include_directories(IMGUI
        PUBLIC ${IMGUI_DIR}
        PUBLIC ${IMGUI_DIR}/backends
        PUBLIC ${IMGUI_DIR}/misc/cpp
        PUBLIC ${SDL2_DIR}/include
        PUBLIC ${IMGUI_FILE_DIR}/
        )
target_link_libraries(IMGUI glfw SDL2main SDL2-static)

find_package(OpenGL REQUIRED)
link_libraries(IMGUI glfw ${OpenCV_LIBS})
add_executable(TiktokGen main.cpp src/model/generators/frame_generators/FrameGenerator.cpp src/model/generators/frame_generators/FrameGenerator.h src/view/Renderer.cpp src/view/Renderer.h src/view/utils/SDLWindowDeleter.h src/model/user/UserState.cpp src/model/user/UserState.h src/view_model/MainViewModel.cpp src/view_model/MainViewModel.h src/model/generators/frame_generators/utils/GeneratorName.h src/view/MainView.cpp src/view/MainView.h src/model/generators/frame_generators/TextVideoGenerator.cpp src/model/generators/frame_generators/TextVideoGenerator.h src/model/generators/frame_generators/utils/GeneratorRequirement.h src/view_model/utils/LogFunctions.h src/model/generators/frame_generators/NoneGenerator.cpp src/model/generators/frame_generators/NoneGenerator.h src/model/generators/Generator.h src/view/utils/SDL_GLContextDeleter.h src/model/generators/frame_generators/utils/GeneratorRequirementType.h src/model/generators/video_generators/VideoGenerator.cpp src/model/generators/video_generators/VideoGenerator.h src/view_model/utils/StringFunctions.h src/model/generators/audio_generators/AudioGenerator.cpp src/model/generators/audio_generators/AudioGenerator.h src/model/generators/audio_generators/AddAudioGenerator.cpp src/model/generators/audio_generators/AddAudioGenerator.h)
