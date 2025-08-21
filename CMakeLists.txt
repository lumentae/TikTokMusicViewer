cmake_minimum_required(VERSION 4.0)
project(tt_music_viewer)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

set(CMAKE_PREFIX_PATH "${VCPKG_PATH}/installed/x64-windows-static/debug;${VCPKG_PATH}/installed/x64-windows-static;${CMAKE_PREFIX_PATH}")

find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(httplib CONFIG REQUIRED)
find_package(OpenSSL CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(CURL CONFIG REQUIRED)
find_path(MINIAUDIO_INCLUDE_DIRS "miniaudio.h")

file(GLOB_RECURSE SRC_FILES
        source/*.cpp
        source/*.c
        source/*.hpp
        source/*.h,
)

add_executable(tt_music_viewer ${SRC_FILES})
set_property(TARGET tt_music_viewer PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_link_libraries(tt_music_viewer
        PRIVATE
        glfw
        imgui::imgui
        httplib::httplib
        OpenSSL::SSL
        OpenSSL::Crypto
        nlohmann_json::nlohmann_json
        CURL::libcurl
)

target_include_directories(tt_music_viewer
        PRIVATE
        ${MINIAUDIO_INCLUDE_DIRS}
        source
)
