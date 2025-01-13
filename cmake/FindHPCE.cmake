# - Try to find the HPCE library
# Once done this will define
#
#  HPCE_FOUND - system has HPCE
#  HPCE_INCLUDE_DIR - HPCE include directory
#  HPCE_LIB - HPCE library directory
#  HPCE_LIBRARIES - HPCE libraries to link

if(HPCE_FOUND)
    return()
endif()

# We prioritize libraries installed in /usr/local with the prefix .../HPCE-*, 
# so we make a list of them here
file(GLOB lib_glob "/usr/local/lib/HPCE-*")
file(GLOB inc_glob "/usr/local/include/HPCE-*")

# Find the library with the name "HPCE" on the system. Store the final path
# in the variable HPCE_LIB
find_library(HPCE_LIB 
    # The library is named "HPCE", but can have various library forms, like
    # libHPCE.a, libHPCE.so, libHPCE.so.1.x, etc. This should
    # search for any of these.
    NAMES HPCE
    # Provide a list of places to look based on prior knowledge about the system.
    # We want the user to override /usr/local with environment variables, so
    # this is included here.
    HINTS
        ${HPCE_DIR}
        ${HPCE_DIR}
        $ENV{HPCE_DIR}
        $ENV{HPCE_DIR}
        ENV HPCE_DIR
    # Provide a list of places to look as defaults. /usr/local shows up because
    # that's the default install location for most libs. The globbed paths also
    # are placed here as well.
    PATHS
        /usr
        /usr/local
        /usr/local/lib
        ${lib_glob}
    # Constrain the end of the full path to the detected library, not including
    # the name of library itself.
    PATH_SUFFIXES 
        lib
)

# Find the path to the header source files on the system. Store the final
# path in the variables HPCE_INCLUDE_DIR. The HINTS, PATHS, and
# PATH_SUFFIXES, arguments have the same meaning as in find_library().
find_path(HPCE_INCLUDE_DIR hpce.hpp
    HINTS
        ${HPCE_DIR}
        ${HPCE_DIR}
        $ENV{HPCE_DIR}
        $ENV{HPCE_DIR}
        ENV HPCE_DIR
    PATHS
        /usr
        /usr/local
        /usr/local/include
        ${inc_glob}
    PATH_SUFFIXES 
        include
)

find_path(HPCE_INCLUDE_DIR pgn_reader.hpp
    HINTS
        ${HPCE_DIR}
        ${HPCE_DIR}
        $ENV{HPCE_DIR}
        $ENV{HPCE_DIR}
        ENV HPCE_DIR
    PATHS
        /usr
        /usr/local
        /usr/local/include
        ${inc_glob}
    PATH_SUFFIXES 
        include
)

find_path(HPCE_INCLUDE_DIR hpce_test_driver.hpp
    HINTS
        ${HPCE_DIR}
        ${HPCE_DIR}
        $ENV{HPCE_DIR}
        $ENV{HPCE_DIR}
        ENV HPCE_DIR
    PATHS
        /usr
        /usr/local
        /usr/local/include
        ${inc_glob}
    PATH_SUFFIXES 
        include
)


# Check that both the paths to the include and library directory were found.
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(HPCE
    "\nHPCE not found --- You can download it using:\n\tgit clone 
    https://github.com/pr0f3ss/HPCE\n and setting the HPCE_DIR environment variable accordingly"
    HPCE_LIB HPCE_INCLUDE_DIR)

# These variables don't show up in the GUI version of CMake. Not required but
# people seem to do this...
mark_as_advanced(HPCE_INCLUDE_DIR HPCE_LIB)

# Finish defining the variables specified above. Variables names here follow
# CMake convention.
set(HPCE_INCLUDE_DIRS ${HPCE_INCLUDE_DIR})
set(HPCE_LIBRARIES ${HPCE_LIB})

# If the above CMake code was successful and we found the library, and there is
# no target defined, lets make one.
if(HPCE_FOUND AND NOT TARGET HPCE::HPCE)
    add_library(HPCE::HPCE UNKNOWN IMPORTED)
    # Set location of interface include directory, i.e., the directory
    # containing the header files for the installed library
    set_target_properties(HPCE::HPCE PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${HPCE_INCLUDE_DIRS}"
        )

    # Set location of the installed library
    set_target_properties(HPCE::HPCE PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
        IMPORTED_LOCATION "${HPCE_LIBRARIES}"
        )
endif()