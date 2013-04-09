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

include_directories(${COMMON_DIR} ${CLIENT_DIR})

find_package(SDL)
find_package(SDL_mixer)
find_package(SDL_net)

include_directories(${SDL_INCLUDE_DIR})
include_directories(${SDLMIXER_INCLUDE_DIR})
include_directories(${SDLNET_INCLUDE_DIR})

add_executable(winmbf WIN32
  ${WINMBF_SOURCES} ${WINMBF_HEADERS})
target_link_libraries(winmbf ${SDL_LIBRARY})
if(MSVC)
  target_link_libraries(winmbf ${SDLMAIN_LIBRARY})
endif()
target_link_libraries(winmbf ${SDLMIXER_LIBRARY})
target_link_libraries(winmbf ${SDLNET_LIBRARY})
