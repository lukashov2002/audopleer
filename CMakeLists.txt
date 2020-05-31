cmake_minimum_required(VERSION 3.15)
project(sound)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 17)
add_library(fmod INTERFACE)
find_package(Threads)

target_link_directories(fmod INTERFACE "${PROJECT_SOURCE_DIR}/FMOD/lib/x64/")
target_link_libraries(fmod INTERFACE fmod_vc)

set_target_properties(fmod PROPERTIES
        #IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/FMOD/lib/x64/fmod_vc.lib"
        INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/FMOD/inc/"
        )

add_executable(sound main.cpp common_platform.cpp common.cpp)
target_link_libraries(sound PUBLIC fmod Threads::Threads)