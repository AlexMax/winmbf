cmake_minimum_required(VERSION 2.8)

project(WinMBF)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# We need to provide a few modules for older CMake versions.  We want to use
# the "old" behavior of prioritizing the version in the module path because the
# version of FindSDL.cmake we provide relies on VERSION_VAR, which isn't in
# older versions of CMake.
if(${CMAKE_MAJOR_VERSION} EQUAL 2
   AND ${CMAKE_MINOR_VERSION} EQUAL 8
   AND ${CMAKE_PATCH_VERSION} GREATER 3)
  cmake_policy(SET CMP0017 OLD)
endif()

if(MSVC)
  add_definitions(/D _CRT_NONSTDC_NO_DEPRECATE)
  add_definitions(/D _CRT_SECURE_NO_WARNINGS)
endif()

include(CheckFunctionExists)
include(CheckIncludeFiles)

check_function_exists(sched_setaffinity HAVE_SCHED_SETAFFINITY)
check_include_files(malloc.h HAVE_MALLOC_H)

set(WINMBF_DIR source)
file(GLOB WINMBF_HEADERS ${WINMBF_DIR}/*.h)
file(GLOB WINMBF_SOURCES ${WINMBF_DIR}/*.c)

configure_file(config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
set(WINMBF_HEADERS ${WINMBF_HEADERS} ${CMAKE_CURRENT_BINARY_DIR}/config.h)

if(APPLE)
  file(GLOB APPLE_HEADERS ${WINMBF_DIR}/apple/*.h)
  file(GLOB APPLE_SOURCES ${WINMBF_DIR}/apple/*.m)
  set(WINMBF_HEADERS ${WINMBF_HEADERS} ${APPLE_HEADERS})
  set(WINMBF_SOURCES ${WINMBF_SOURCES} ${APPLE_SOURCES})
endif()

set(TEXTSCREEN_LIBRARY "textscreen")
set(TEXTSCREEN_DIR textscreen)
file(GLOB TEXTSCREEN_HEADERS ${TEXTSCREEN_DIR}/*.h)
file(GLOB TEXTSCREEN_SOURCES ${TEXTSCREEN_DIR}/*.c)

include_directories(${TEXTSCREEN_DIR})
add_library(${TEXTSCREEN_LIBRARY} STATIC ${TEXTSCREEN_SOURCES} ${TEXTSCREEN_HEADERS})
message(STATUS "Textscreen will be statically linked.")

find_package(SDL)
find_package(SDL_mixer)
find_package(SDL_net)

include_directories(${SDL_INCLUDE_DIR})
include_directories(${SDLMIXER_INCLUDE_DIR})
include_directories(${SDLNET_INCLUDE_DIR})

add_executable(winmbf WIN32
  ${WINMBF_SOURCES} ${WINMBF_HEADERS})
target_link_libraries(winmbf ${TEXTSCREEN_LIBRARY})
target_link_libraries(winmbf ${SDL_LIBRARY})
target_link_libraries(winmbf ${SDLMIXER_LIBRARY})
target_link_libraries(winmbf ${SDLNET_LIBRARY})
