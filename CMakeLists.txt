cmake_minimum_required(VERSION 3.12)
project( sdl_basegame )

set( CMAKE_C_STANDARD   17 )
set( CMAKE_CXX_STANDARD 17 )

find_package( Git )

if( GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git" )
    # Update submodules as needed
    option( GIT_SUBMODULE "Check submodules during build" ON )
    if( GIT_SUBMODULE )
        message( STATUS "Update submodules (this includes vcpkg)" )
        execute_process(
                COMMAND             ${GIT_EXECUTABLE} submodule update --init --recursive
                WORKING_DIRECTORY   ${CMAKE_CURRENT_SOURCE_DIR}
                RESULT_VARIABLE     GIT_SUBMOD_RESULT )
        if( NOT GIT_SUBMOD_RESULT EQUAL "0" )
            message( FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules" )
        endif()
    endif()
endif()

set(      DOTNET_CLI_TELEMETRY_OPTOUT   1 )
set( $ENV{DOTNET_CLI_TELEMETRY_OPTOUT}  1 )
set(      VCPKG_DISABLE_METRICS         1 )
set( $ENV{VCPKG_DISABLE_METRICS}        1 )
set(      VCPKG_INSTALLED_DIR           ${CMAKE_SOURCE_DIR}/vcpkg_installed )
include( ${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake )

find_package( SDL2       CONFIG REQUIRED )
find_package( SDL2_ttf   CONFIG REQUIRED )
find_package( SDL2_mixer CONFIG REQUIRED )
find_package( SDL2_image CONFIG REQUIRED )
find_package( SDL2_net   CONFIG REQUIRED )

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set( CLANG 1 )
endif()

option( USE_OpenMP "Use OpenMP" ON )
if( USE_OpenMP )
    find_package( OpenMP )
endif()

if( MSVC )
    add_compile_options( /W4 )
else()
    # needs to be before all the -Wno-*
    set( CMAKE_C_FLAGS           "${CMAKE_C_FLAGS}           -Wall -Wextra" )
    set( CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS}         -Wall -Wextra" )

    set( CMAKE_C_FLAGS           "${CMAKE_C_FLAGS}           -Wno-unused-parameter" )   # baseclass functions have plenty unused parameters
    set( CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS}         -Wno-unused-parameter" )   # baseclass functions have plenty unused parameters

    if( CLANG )
        set( CMAKE_C_FLAGS       "${CMAKE_C_FLAGS}           -Wno-unknown-attributes" ) # clang does not know [[(un)likely]]
        set( CMAKE_CXX_FLAGS     "${CMAKE_CXX_FLAGS}         -Wno-unknown-attributes" ) # clang does not know [[(un)likely]]
    endif()

    # (how) does this work for MSVC?
    if( OPENMP_FOUND )
        set( CMAKE_C_FLAGS       "${CMAKE_C_FLAGS}           ${OpenMP_C_FLAGS}"   )
        set( CMAKE_CXX_FLAGS     "${CMAKE_CXX_FLAGS}         ${OpenMP_CXX_FLAGS}" )
    else()
        set( CMAKE_C_FLAGS       "${CMAKE_C_FLAGS}           -Wno-unknown-pragmas" )
        set( CMAKE_CXX_FLAGS     "${CMAKE_CXX_FLAGS}         -Wno-unknown-pragmas" )
    endif()

    set( CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS_DEBUG}     -Og" )
    set( CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   -Og" )
    set( CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE}   -O2" )     # default is -O3, which might cause concurrency problems
    set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2" )     # default is -O3, which might cause concurrency problems
endif()

add_compile_options( "$<$<CONFIG:DEBUG>:-DDEBUG>" )

add_executable( sdl_basegame
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

target_include_directories( sdl_basegame
        PRIVATE
        src
        ${SDL2_INCLUDE_DIR}
        )

target_link_libraries( sdl_basegame
        PRIVATE
        #$<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,               SDL2::SDL2,             SDL2::SDL2-static>
        $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,       SDL2_ttf::SDL2_ttf,     SDL2_ttf::SDL2_ttf-static>
        $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,   SDL2_mixer::SDL2_mixer, SDL2_mixer::SDL2_mixer-static>
        $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,   SDL2_image::SDL2_image, SDL2_image::SDL2_image-static>
        $<IF:$<TARGET_EXISTS:SDL2_net::SDL2_net>,       SDL2_net::SDL2_net,     SDL2_net::SDL2_net-static>
        $<TARGET_NAME_IF_EXISTS:OpenMP::OpenMP_CXX>
        )
