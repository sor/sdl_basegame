cmake_minimum_required(VERSION 3.12)
project(sdl_basegame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter")     # baseclass functions have plenty unused parameters
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-attributes")   # clang does not know [[(un)likely]]
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

set(CMAKE_CXX_FLAGS_DEBUG   "-Og")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

add_compile_options("$<$<CONFIG:DEBUG>:-D_DEBUG>")

add_executable(sdl_basegame
        src/global.h
        src/gamebase.h
        src/gamebase.cpp
        src/recthelper.h

        src/example/pongmain.cpp

        src/example/main.cpp
        src/example/examplegame.h
        src/example/examplegame.cpp
        src/example/camerastate.cpp
        src/example/introstate.cpp
        src/example/plasmastate.cpp
        src/example/shooterstate.cpp
        src/example/sortstate.cpp
)

include(FindPkgConfig)

if (MSVC)
    # still does not work in my old VS 2017, but at least it does not complain
	# if someone finds out how to easily use the nuget packages here let me know
else()
    pkg_search_module(SDL2      REQUIRED sdl2>=2.0.0)
    pkg_search_module(SDL2TTF   REQUIRED SDL2_ttf>=2.0.0)
    pkg_search_module(SDL2NET   REQUIRED SDL2_net>=2.0.0)
    pkg_search_module(SDL2MIXER REQUIRED SDL2_mixer>=2.0.0)
    pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
    #pkg_search_module(SDL2GFX   REQUIRED SDL2_gfx>=1.0.0) # this 1.x.x is correct
endif()

include_directories(
        ${SDL2_INCLUDE_DIRS}
        ${SDL2TTF_INCLUDE_DIRS}
        ${SDL2NET_INCLUDE_DIRS}
        ${SDL2MIXER_INCLUDE_DIRS}
        ${SDL2IMAGE_INCLUDE_DIRS}
        #${SDL2GFX_INCLUDE_DIRS}
)

target_link_libraries(
        ${PROJECT_NAME}
        ${SDL2_LIBRARIES}
        ${SDL2TTF_LIBRARIES}
        ${SDL2NET_LIBRARIES}
        ${SDL2MIXER_LIBRARIES}
        ${SDL2IMAGE_LIBRARIES}
        #${SDL2GFX_LIBRARIES}
)
