cmake_minimum_required(VERSION 3.0)

project(velvetslim)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

if( MSVC OR MSYS OR MINGW )
    set(CMAKE_CXX_FLAGS "/O3 /std:c++17")
else()
  set(CMAKE_CXX_FLAGS "-O3 -march=native -pthread -std=c++17")
  #set(CMAKE_CXX_FLAGS "-ggdb -g -pg -O0 -pthread")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake )

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

if( NOT UNIX )
	set( ZLIB_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/libs/zlib/x64 )
endif( NOT UNIX )

find_file( SDL2_INCLUDE_DIR NAME SDL.h HINTS SDL2 )
find_library( SDL2_LIBRARY NAME SDL2 )

set( SDL2_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2/include )
set( SDL2_IMAGE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2_image/include )
set( SDL2_TTF_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2_ttf/include )
set( SDL2_SOUND_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2_mixer/include )

if( NOT SDL2_LIBRARY )

    set( SDL2_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2/lib/x64 )
    set( SDL2_IMAGE_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2_image/lib/x64 )
    set( SDL2_TTF_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2_ttf/lib/x64 )
    set( SDL2_SOUND_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2_mixer/lib/x64 )

endif()

link_directories(${SDL2_LIBRARY} )
link_directories(${SDL2_IMAGE_LIBRARY})
link_directories(${SDL2_TTF_LIBRARY})
link_directories(${SDL2_SOUND_LIBRARY})
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/zlib/x64)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/bin/bin)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/src )

message( ${SDL2_INCLUDE_DIR} )

file( GLOB VELVET_INCLUDE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
        src/*.h
        src/data/*.h
        src/parser/*.h
        src/object/*.h
        src/draw/*.h
        src/state/*.h
        src/resource/*.h
)

file( GLOB VELVET_SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
        src/*.cpp
        src/data/*.cpp
        src/parser/*.cpp
        src/object/*.cpp
        src/draw/*.cpp
        src/state/*.cpp
        src/resource/*.cpp
)


add_library( velvet SHARED ${VELVET_INCLUDE_FILES} ${VELVET_SOURCE_FILES} )

target_include_directories( velvet PUBLIC ../${CMAKE_CURRENT_SOURCE_DIR} )
target_include_directories( velvet PUBLIC ${SDL2_INCLUDE_DIR} )
target_include_directories( velvet PUBLIC ${SDL2_IMAGE_INCLUDE_DIR} )
target_include_directories( velvet PUBLIC ${SDL2_TTF_INCLUDE_DIR} )
target_include_directories( velvet PUBLIC ${SDL2_SOUND_INCLUDE_DIR} )

target_link_libraries( velvet SDL2main SDL2 SDL2_image SDL2_mixer )