FIND_PATH(SDL2Mixer_INCLUDE_DIR SDL_mixer.h
        HINTS
        ${SDL2}
        $ENV{SDL2}
        $ENV{SDL2_MIXER}
        PATH_SUFFIXES include/SDL2 include SDL2
        i686-w64-mingw32/include/SDL2
        x86_64-w64-mingw32/include/SDL2
        PATHS
        ~/Library/Frameworks
        /Library/Frameworks
        /usr/local/include/SDL2
        /usr/include/SDL2
        /sw # Fink
        /opt/local # DarwinPorts
        /opt/csw # Blastwave
        /opt
        )

# Lookup the 64 bit libs on x64
IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
    FIND_LIBRARY(SDL2Mixer_LIBRARY_TEMP
            NAMES SDL2_mixer
            HINTS
            ${SDL2}
            $ENV{SDL2}
            $ENV{SDL2_MIXER}
            PATH_SUFFIXES lib64 lib
            lib/x64
            x86_64-w64-mingw32/lib
            PATHS
            ~/Library/Frameworks
            /Library/Frameworks
            /usr/local
            /usr
            /sw # Fink
            /opt/local # DarwinPorts
            /opt/csw # Blastwave
            /opt
            )
    # On 32bit build find the 32bit libs
ELSE(CMAKE_SIZEOF_VOID_P EQUAL 8)
    FIND_LIBRARY(SDL2Mixer_LIBRARY_TEMP
            NAMES SDL2_mixer
            HINTS
            ${SDL2}
            $ENV{SDL2}
            $ENV{SDL2_MIXER}
            PATH_SUFFIXES lib
            lib/x86
            i686-w64-mingw32/lib
            PATHS
            ~/Library/Frameworks
            /Library/Frameworks
            /usr/local
            /usr
            /sw # Fink
            /opt/local # DarwinPorts
            /opt/csw # Blastwave
            /opt
            )
ENDIF(CMAKE_SIZEOF_VOID_P EQUAL 8)

SET(SDL2Mixer_FOUND "NO")
IF(SDL2Mixer_LIBRARY_TEMP)
    # Set the final string here so the GUI reflects the final state.
    SET(SDL2Mixer_LIBRARY ${SDL2Mixer_LIBRARY_TEMP} CACHE STRING "Where the SDL2_mixer Library can be found")
    # Set the temp variable to INTERNAL so it is not seen in the CMake GUI
    SET(SDL2Mixer_LIBRARY_TEMP "${SDL2Mixer_LIBRARY_TEMP}" CACHE INTERNAL "")
    SET(SDL2Mixer_FOUND "YES")
ENDIF(SDL2Mixer_LIBRARY_TEMP)

INCLUDE(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(SDL2Mixer REQUIRED_VARS SDL2Mixer_LIBRARY SDL2Mixer_INCLUDE_DIR)

if (SDL2Mixer_FOUND)
    set(SDL2Mixer_LIBRARIES ${SDL2Mixer_LIBRARY} ${SDL2_LIBRARIES})
    set(SDL2Mixer_INCLUDE_DIRS ${SDL2Mixer_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS})
endif()