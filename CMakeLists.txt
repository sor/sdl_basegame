cmake_minimum_required( VERSION 3.20 )
project( sdl_basegame )

set( CMAKE_C_STANDARD   17 )
set( CMAKE_CXX_STANDARD 20 )

#set(      VCPKG_VERBOSE                 ON )
set(      VCPKG_BOOTSTRAP_OPTIONS       "-disableMetrics" )
set(      VCPKG_INSTALL_OPTIONS         "--no-print-usage")
set(      VCPKG_INSTALLED_DIR           ${CMAKE_SOURCE_DIR}/build/vcpkg_installed ) # comment this out to have a vcpkg_installed per build target
set(      VCPKG_DISABLE_METRICS         1 )
set( $ENV{VCPKG_DISABLE_METRICS}        1 )
set(      DOTNET_CLI_TELEMETRY_OPTOUT   1 )
set( $ENV{DOTNET_CLI_TELEMETRY_OPTOUT}  1 )

if( NOT MSVC )
    # Might rework MSVC to align with the adjusted CLion paths
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
endif()

# Add path of current cmake(.exe) to PATH
cmake_path( GET CMAKE_COMMAND PARENT_PATH CMAKE_COMMAND_PATH )
if( WIN32 )
    set( ENV{PATH} "${CMAKE_COMMAND_PATH};$ENV{PATH}" )
else()
    set( ENV{PATH} "${CMAKE_COMMAND_PATH}:$ENV{PATH}" )
endif()

get_filename_component( OUTSIDE_VCPKG_DIR "${CMAKE_SOURCE_DIR}/../vcpkg/" ABSOLUTE )
set( OUTSIDE_VCPKG_DIR "${OUTSIDE_VCPKG_DIR}"             CACHE PATH "Location of the outside vcpkg" )
set( INSIDE_VCPKG_DIR  "${CMAKE_SOURCE_DIR}/build/vcpkg/" CACHE PATH "Location of the inside vcpkg" )
if( EXISTS "${OUTSIDE_VCPKG_DIR}" )
    option( OUTSIDE_VCPKG_USE "Use a vcpkg which is outside of this repo, in a parallel folder" ON )
    if( OUTSIDE_VCPKG_USE )
        message( STATUS "Using outside vcpkg in ${OUTSIDE_VCPKG_DIR}" )
        set( VCPKG_DIR ${OUTSIDE_VCPKG_DIR} )
    else()
        set( VCPKG_DIR ${INSIDE_VCPKG_DIR} )
    endif()
else()
    option( OUTSIDE_VCPKG_USE "Use a vcpkg which is outside of this repo, in a parallel folder" OFF )
    set( VCPKG_DIR ${INSIDE_VCPKG_DIR} )
endif()

find_package( Git )

file( LOCK "${VCPKG_DIR}.lock" )

if( GIT_FOUND AND NOT OUTSIDE_VCPKG_USE )
    if( EXISTS "${PROJECT_SOURCE_DIR}/.git" )
        option( GIT_VCPKG "Check vcpkg during build" ON )
        if( GIT_VCPKG )
            if( NOT EXISTS "${VCPKG_DIR}" )
                message( STATUS "Cloning ${VCPKG_DIR}" )
                execute_process(
                        COMMAND             ${GIT_EXECUTABLE} clone https://github.com/Microsoft/vcpkg.git ${VCPKG_DIR}
                        WORKING_DIRECTORY   ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE     GIT_RESULT )
            else()
                message( STATUS "Pulling ${VCPKG_DIR}" )
                execute_process(
                        COMMAND             ${GIT_EXECUTABLE} pull -q
                        WORKING_DIRECTORY   ${VCPKG_DIR}
                        RESULT_VARIABLE     GIT_RESULT )
            endif()

            if( NOT GIT_RESULT EQUAL "0" )
                message( FATAL_ERROR "git command failed with ${GIT_RESULT}" )
            endif()
        endif()

        if( EXISTS "${PROJECT_SOURCE_DIR}/.gitmodules" )
            option( GIT_SUBMODULE "Check submodules during build" OFF )
            if( GIT_SUBMODULE )
                message( STATUS "Update submodules" )
                execute_process(
                        COMMAND             ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY   ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE     GIT_SUBMOD_RESULT )

                if( NOT GIT_SUBMOD_RESULT EQUAL "0" )
                    message( FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules" )
                endif()
            endif()
        endif()
    endif()
endif()

include( ${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake )

file( LOCK "${VCPKG_DIR}.lock" RELEASE )


find_package( SDL2       CONFIG REQUIRED )
find_package( SDL2_ttf   CONFIG REQUIRED )
find_package( SDL2_mixer CONFIG REQUIRED )
find_package( SDL2_image CONFIG REQUIRED )
find_package( SDL2_net   CONFIG REQUIRED )

if( CMAKE_CXX_COMPILER_ID MATCHES "Clang" )
    set( CLANG 1 ) # Why does this exist for MSVC but not Clang or GCC?
elseif( CMAKE_CXX_COMPILER_ID MATCHES "GNU" )
    set( GCC 1 )
endif()

option( USE_OpenMP "Use OpenMP" ON )
if( USE_OpenMP )
    find_package( OpenMP )
endif()

if( MSVC )
    add_compile_options( /W4 /wd4100 /we4456 )
else()
    # needs to be before all the -Wno-*
    set( CMAKE_C_FLAGS           "${CMAKE_C_FLAGS}           -Wall -Wextra" )
    set( CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS}         -Wall -Wextra" )

    if( GCC )
        set( CMAKE_C_FLAGS       "${CMAKE_C_FLAGS}           -Wshadow=local -Werror=shadow=local" )
        set( CMAKE_CXX_FLAGS     "${CMAKE_CXX_FLAGS}         -Wshadow=local -Werror=shadow=local" )
    elseif( CLANG )
        # TODO: Clang uses different names
    endif()

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
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,               SDL2::SDL2,             SDL2::SDL2-static>
        $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,       SDL2_ttf::SDL2_ttf,     SDL2_ttf::SDL2_ttf-static>
        $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,   SDL2_mixer::SDL2_mixer, SDL2_mixer::SDL2_mixer-static>
        $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,   SDL2_image::SDL2_image, SDL2_image::SDL2_image-static>
        $<IF:$<TARGET_EXISTS:SDL2_net::SDL2_net>,       SDL2_net::SDL2_net,     SDL2_net::SDL2_net-static>
        $<TARGET_NAME_IF_EXISTS:OpenMP::OpenMP_CXX>
        )
