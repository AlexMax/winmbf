cmake_minimum_required(VERSION 2.8)

project(WinMBF)

include(CheckFunctionExists)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

check_function_exists(sched_setaffinity HAVE_SCHED_SETAFFINITY)
if(HAVE_SCHED_SETAFFINITY)
  add_definitions(-DHAVE_SCHED_SETAFFINITY)
endif()

set(WINMBF_DIR source)
file(GLOB WINMBF_HEADERS ${WINMBF_DIR}/*.h)
file(GLOB WINMBF_SOURCES ${WINMBF_DIR}/*.c)

include_directories(${COMMON_DIR} ${CLIENT_DIR})

set(TEXTSCREEN_LIBRARY "textscreen")
set(TEXTSCREEN_DIR textscreen)
file(GLOB TEXTSCREEN_HEADERS ${TEXTSCREEN_DIR}/*.h)
file(GLOB TEXTSCREEN_SOURCES ${TEXTSCREEN_DIR}/*.c)

include_directories(${TEXTSCREEN_DIR})
add_library(${TEXTSCREEN_LIBRARY} STATIC ${TEXTSCREEN_SOURCES} ${TEXTSCREEN_HEADERS})
message(STATUS "Textscreen will be statically linked.")

set(LUA_LIBRARY "lua52")
set(LUA_DIR lua/src)
set(LUA_CORE
  lapi.c lcode.c lctype.c ldebug.c ldo.c ldump.c lfunc.c lgc.c llex.c
  lmem.c lobject.c lopcodes.c lparser.c lstate.c lstring.c ltable.c
  ltm.c lundump.c lvm.c lzio.c
)
set(LUA_LIB
  lauxlib.c lbaselib.c lbitlib.c lcorolib.c ldblib.c liolib.c
  lmathlib.c loslib.c lstrlib.c ltablib.c loadlib.c linit.c
)
foreach(LUA_SOURCE IN LISTS LUA_CORE LUA_LIB)
  set(LUA_SOURCES ${LUA_SOURCES} ${LUA_DIR}/${LUA_SOURCE})
endforeach()

include_directories(${LUA_DIR})
if(WIN32)
  add_library(${LUA_LIBRARY} SHARED ${LUA_SOURCES} ${LUA_HEADERS})
  set_property(TARGET ${LUA_LIBRARY} PROPERTY COMPILE_DEFINITIONS LUA_BUILD_AS_DLL)
  message(STATUS "Lua will be dynamically linked.")
else()
  add_library(${LUA_LIBRARY} STATIC ${LUA_SOURCES} ${LUA_HEADERS})
  if(APPLE)
    set_property(TARGET ${LUA_LIBRARY} PROPERTY COMPILE_DEFINITIONS LUA_USE_MACOSX)
  else()
    set_property(TARGET ${LUA_LIBRARY} PROPERTY COMPILE_DEFINITIONS LUA_USE_LINUX)
  endif()
  message(STATUS "Lua will be statically linked.")
endif()
set_property(TARGET ${LUA_LIBRARY} PROPERTY COMPILE_DEFINITIONS LUA_COMPAT_ALL)

find_package(SDL REQUIRED)
find_package(SDL_mixer REQUIRED)
find_package(SDL_net REQUIRED)

include_directories(${SDL_INCLUDE_DIR})
include_directories(${SDLMIXER_INCLUDE_DIR})
include_directories(${SDLNET_INCLUDE_DIR})
include_directories(${LUA_INCLUDE_DIR})

add_executable(winmbf WIN32
  ${WINMBF_SOURCES} ${WINMBF_HEADERS})
target_link_libraries(winmbf ${TEXTSCREEN_LIBRARY})
target_link_libraries(winmbf ${SDL_LIBRARY})
if(MSVC)
  target_link_libraries(winmbf ${SDLMAIN_LIBRARY})
endif()
target_link_libraries(winmbf ${SDLMIXER_LIBRARY})
target_link_libraries(winmbf ${SDLNET_LIBRARY})
target_link_libraries(winmbf ${LUA_LIBRARY})
