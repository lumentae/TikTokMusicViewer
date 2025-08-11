include(cmake/CPM.cmake)

cmake_minimum_required(VERSION 4.0)
project(tt_music_viewer)

set(CMAKE_CXX_STANDARD 26)

find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(httplib CONFIG REQUIRED)
find_package(OpenSSL CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(CURL CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_path(MINIAUDIO_INCLUDE_DIRS "miniaudio.h")


file(GLOB_RECURSE SRC_FILES
        source/*.cpp
        source/*.c
        source/*.hpp
        source/*.h,
)

add_executable(tt_music_viewer ${SRC_FILES})

#CPMAddPackage(
#        NAME GLFW
#        GITHUB_REPOSITORY glfw/glfw
#        GIT_TAG 3.4
#        OPTIONS
#        "GLFW_BUILD_TESTS OFF"
#        "GLFW_BUILD_EXAMPLES OFF"
#        "GLFW_BUILD_DOCS OFF"
#)
#
# ImGui
#CPMAddPackage(
#        NAME imgui
#        VERSION 1.92.1
#        GITHUB_REPOSITORY ocornut/imgui
#        DOWNLOAD_ONLY TRUE
#)
#
## CMakeLists.txt from https://gist.githubusercontent.com/rokups/f771217b2d530d170db5cb1e08e9a8f4
#file(
#        DOWNLOAD
#        "https://gist.githubusercontent.com/rokups/f771217b2d530d170db5cb1e08e9a8f4/raw/4c2c14374ab878ca2f45daabfed4c156468e4e27/CMakeLists.txt"
#        "${imgui_SOURCE_DIR}/CMakeLists.txt"
#        EXPECTED_HASH SHA256=fd62f69364ce13a4f7633a9b50ae6672c466bcc44be60c69c45c0c6e225bb086
#)
#
#set(IMGUI_ENABLE_FREETYPE FALSE)
#set(IMGUI_ENABLE_STDLIB_SUPPORT TRUE)
#
## Add subdirectory
#add_subdirectory(${imgui_SOURCE_DIR} EXCLUDE_FROM_ALL SYSTEM)
#
#CPMAddPackage("gh:yhirose/cpp-httplib@0.25.0")
#CPMAddPackage(
#        URI "gh:viaduck/openssl-cmake#1bdd30fd7947bf1402c58922c03afb31d9f6689d"
#        OPTIONS "SYSTEM_OPENSSL ON"
#)
#CPMAddPackage("gh:nlohmann/json@3.12.0")
#
##FetchContent_Declare(
##        stb_image
##        GIT_REPOSITORY  https://github.com/nothings/stb.git
##        GIT_TAG         master
##        GIT_SHALLOW     TRUE
##        GIT_PROGRESS    TRUE
##)
##FetchContent_GetProperties(stb_image)
##if(NOT stb_image_POPULATED)
##    FetchContent_Populate(stb_image)
##    message("Fetching stb_image")
##
##    add_library(stb_image INTERFACE ${stb_image_SOURCE_DIR}/stb_image.h)
##    target_include_directories(stb_image INTERFACE ${stb_image_SOURCE_DIR})
##endif()
#CPMAddPackage("gh:nothings/stb#f58f558c120e9b32c217290b80bad1a0729fbb2c")
#add_library(stb INTERFACE ${stb_SOURCE_DIR}/stb_image.h)
#target_include_directories(stb INTERFACE ${stb_SOURCE_DIR})
#
#CPMAddPackage(
#        NAME libcurl
#        VERSION 8.15.0
#        URL https://curl.se/download/curl-8.15.0.tar.gz
#)

target_link_libraries(tt_music_viewer
        PRIVATE
        glfw
        imgui::imgui
        httplib::httplib
        OpenSSL::SSL
        OpenSSL::Crypto
        nlohmann_json::nlohmann_json
        CURL::libcurl
        glad::glad
)

target_include_directories(tt_music_viewer
        PRIVATE
        ${Stb_INCLUDE_DIR}
        ${MINIAUDIO_INCLUDE_DIRS}
)
