cmake_minimum_required(VERSION 2.8)

project(WinMBF)

include(CheckFunctionExists)

set(WINMBF_DIR source)
file(GLOB WINMBF_HEADERS ${WINMBF_DIR}/*.h)
file(GLOB WINMBF_SOURCES ${WINMBF_DIR}/*.c)

include_directories(${COMMON_DIR} ${CLIENT_DIR})

check_function_exists(sched_setaffinity HAVE_SCHED_SETAFFINITY)
if(HAVE_SCHED_SETAFFINITY)
  add_definitions(-DHAVE_SCHED_SETAFFINITY)
endif()

find_package(SDL)
if(SDL_FOUND)
  include_directories(${SDL_INCLUDE_DIR})
else()
  message(WARNING "SDL not found.")
endif()

find_package(SDL_mixer)
if(SDLMIXER_FOUND)
  include_directories(${SDLMIXER_INCLUDE_DIR})
else()
  message(WARNING "SDL_mixer not found.")
endif()

find_package(SDL_net)
if(SDLNET_FOUND)
  include_directories(${SDLNET_INCLUDE_DIR})
else()
  message(WARNING "SDL_net not found.")
endif()

if(SDL_FOUND AND SDLMIXER_FOUND AND SDLNET_FOUND)
  add_executable(winmbf WIN32
    ${WINMBF_SOURCES} ${WINMBF_HEADERS})
  target_link_libraries(winmbf ${SDL_LIBRARY})
  if(MSVC)
    target_link_libraries(winmbf ${SDLMAIN_LIBRARY})
  endif()
  target_link_libraries(winmbf ${SDLMIXER_LIBRARY})
  target_link_libraries(winmbf ${SDLNET_LIBRARY})
endif()
