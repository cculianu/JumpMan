cmake_minimum_required(VERSION 3.5)

project(JumpMan LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    # warning level 4
    add_compile_options(/W4)
else()
    # lots of warnings
    add_compile_options(-Wall -Wextra -pedantic)
endif()

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)
include_directories(${PROJECT_SOURCE_DIR}/src)
add_executable(jumpman ${SRC_FILES})

find_package(PkgConfig REQUIRED)

pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_MIXER REQUIRED sdl2_mixer)
pkg_check_modules(SDL2_TTF REQUIRED sdl2_ttf)
pkg_check_modules(SDL2_IMAGE REQUIRED sdl2_image)

target_link_libraries(jumpman
    ${SDL2_LINK_LIBRARIES}
    ${SDL2_MIXER_LINK_LIBRARIES}
    ${SDL2_TTF_LINK_LIBRARIES}
    ${SDL2_IMAGE_LINK_LIBRARIES}
)

target_include_directories(jumpman PRIVATE
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
)

target_compile_options(jumpman PRIVATE
    ${SDL2_CFLAGS_OTHER}
    ${SDL2_MIXER_CFLAGS_OTHER}
    ${SDL2_TTF_CFLAGS_OTHER}
    ${SDL2_IMAGE_CFLAGS_OTHER}
)
