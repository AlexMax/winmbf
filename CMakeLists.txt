cmake_minimum_required(VERSION 2.8)

project(WinMBF)

include(CheckFunctionExists)

check_function_exists(sched_setaffinity HAVE_SCHED_SETAFFINITY)
if(HAVE_SCHED_SETAFFINITY)
  add_definitions(-DHAVE_SCHED_SETAFFINITY)
endif()

set(WINMBF_DIR source)
file(GLOB WINMBF_HEADERS ${WINMBF_DIR}/*.h)
file(GLOB WINMBF_SOURCES ${WINMBF_DIR}/*.c)

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
if(MSVC)
  target_link_libraries(winmbf ${SDLMAIN_LIBRARY})
endif()
target_link_libraries(winmbf ${SDLMIXER_LIBRARY})
target_link_libraries(winmbf ${SDLNET_LIBRARY})
