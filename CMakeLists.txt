cmake_minimum_required(VERSION 3.10)
project(ImakeNOISE)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PORTAUDIO REQUIRED portaudio-2.0)

add_executable(playsound main.c) 
target_include_directories(playsound PRIVATE ${PORTAUDIO_INCLUDE_DIRS})

target_link_libraries(playsound PRIVATE ${PORTAUDIO_LIBRARIES})


