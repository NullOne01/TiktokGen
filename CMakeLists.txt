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

set(FFMPEG_DIR ${LIB_DIR}/ffmpeg_build)
set(FFMPEG_INCLUDE_DIR ${FFMPEG_DIR}/include)
set(FFMPEG_LIB_DIR ${FFMPEG_DIR}/lib)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBAV REQUIRED IMPORTED_TARGET
        libavdevice
        libavfilter
        libavformat
        libavcodec
        libswresample
        libswscale
        libavutil
        )

link_libraries(PkgConfig::LIBAV)
# if clang: error: no such file or directory: 'AudioToolbox' is printed, then reload cmake
message(${LIBAV_FOUND})
message(${LIBAV_LINK_LIBRARIES})

#find_path(AVCODEC_INCLUDE_DIR libavcodec/avcodec.h)
#find_library(AVCODEC_LIBRARY avcodec)
#
#find_path(AVFORMAT_INCLUDE_DIR libavformat/avformat.h)
#find_library(AVFORMAT_LIBRARY avformat)
#
#find_path(AVUTIL_INCLUDE_DIR libavutil/avutil.h)
#find_library(AVUTIL_LIBRARY avutil)
#
#find_path(AVDEVICE_INCLUDE_DIR libavdevice/avdevice.h)
#find_library(AVDEVICE_LIBRARY avdevice)
#
#message(JOPA)
#message(${AVCODEC_INCLUDE_DIR})
#message(${AVCODEC_LIBRARY})
#include_directories(${AVCODEC_INCLUDE_DIR} ${AVFORMAT_INCLUDE_DIR} ${AVUTIL_INCLUDE_DIR} ${AVDEVICE_INCLUDE_DIR})
#link_libraries(${AVCODEC_LIBRARY} ${AVFORMAT_LIBRARY} ${AVUTIL_LIBRARY} ${AVDEVICE_LIBRARY})

#include_directories(${FFMPEG_INCLUDE_DIR})
#link_libraries(${FFMPEG_LIB_DIR}/libavcodec.a ${FFMPEG_LIB_DIR}/libavformat.a ${FFMPEG_LIB_DIR}/libavutil.a ${FFMPEG_LIB_DIR}/libavdevice.a)

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

add_library(IMGUI)
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

set(Td_DIR ${LIB_DIR}/td/tdlib/lib/cmake/Td)
find_package(Td 1.8.4 REQUIRED)
#add_subdirectory(${Td_DIR})
link_libraries(Td::TdJsonStatic Td::TdStatic)

find_package(OpenGL REQUIRED)
link_libraries(IMGUI glfw ${OpenCV_LIBS})
add_executable(TiktokGen main.cpp src/model/generators/frame_generators/FrameGenerator.cpp src/model/generators/frame_generators/FrameGenerator.h src/view/Renderer.cpp src/view/Renderer.h src/view/utils/SDLWindowDeleter.h src/model/user/UserState.cpp src/model/user/UserState.h src/view_model/MainViewModel.cpp src/view_model/MainViewModel.h src/model/generators/frame_generators/utils/GeneratorName.h src/view/MainView.cpp src/view/MainView.h src/model/generators/frame_generators/TextVideoGenerator.cpp src/model/generators/frame_generators/TextVideoGenerator.h src/view_model/utils/LogFunctions.h src/model/generators/frame_generators/NoneGenerator.cpp src/model/generators/frame_generators/NoneGenerator.h src/model/generators/Generator.h src/view/utils/SDL_GLContextDeleter.h src/model/generators/video_generators/VideoGenerator.cpp src/model/generators/video_generators/VideoGenerator.h src/view_model/utils/StringFunctions.h src/model/generators/audio_generators/AudioGenerator.cpp src/model/generators/audio_generators/AudioGenerator.h src/model/generators/audio_generators/AddAudioGenerator.cpp src/model/generators/audio_generators/AddAudioGenerator.h src/model/generators/requirements/InputTextRequirement.cpp src/model/generators/requirements/InputTextRequirement.h src/model/generators/requirements/GeneratorRequirement.cpp src/model/generators/requirements/GeneratorRequirement.h src/model/generators/requirements/VideoChooseRequirement.cpp src/model/generators/requirements/VideoChooseRequirement.h src/model/generators/requirements/IGeneratorRequirement.h src/model/generators/requirements/TelegramRandomRequirement.cpp src/model/generators/requirements/TelegramRandomRequirement.h src/model/user/ConfigRead.cpp src/model/user/ConfigRead.h src/model/user/Config.cpp src/model/user/Config.h src/model/generators/requirements/utils/ThreadWrapper.cpp src/model/generators/requirements/utils/ThreadWrapper.h  src/view/utils/TextFunctions.h src/model/generators/requirements/utils/TelegramListenerThread.cpp src/model/generators/requirements/utils/TelegramListenerThread.h src/model/generators/requirements/utils/TelegramClient.cpp src/model/generators/requirements/utils/TelegramClient.h src/model/generators/frame_generators/TelegramRandomGenerator.cpp src/model/generators/frame_generators/TelegramRandomGenerator.h)
