# - Try to find the CMakeHPCE library
# Once done this will define
#
#  CMakeHPCE_FOUND - system has CMakeHPCE
#  CMakeHPCE_INCLUDE_DIR - CMakeHPCE include directory
#  CMakeHPCE_LIB - CMakeHPCE library directory
#  CMakeHPCE_LIBRARIES - CMakeHPCE libraries to link

if(CMakeHPCE_FOUND)
    return()
endif()

# We prioritize libraries installed in /usr/local with the prefix .../CMakeHPCE-*, 
# so we make a list of them here
file(GLOB lib_glob "/usr/local/lib/CMakeHPCE-*")
file(GLOB inc_glob "/usr/local/include/CMakeHPCE-*")

# Find the library with the name "CMakeHPCE" on the system. Store the final path
# in the variable CMakeHPCE_LIB
find_library(CMakeHPCE_LIB 
    # The library is named "CMakeHPCE", but can have various library forms, like
    # libCMakeHPCE.a, libCMakeHPCE.so, libCMakeHPCE.so.1.x, etc. This should
    # search for any of these.
    NAMES CMakeHPCE
    # Provide a list of places to look based on prior knowledge about the system.
    # We want the user to override /usr/local with environment variables, so
    # this is included here.
    HINTS
        ${CMakeHPCE_DIR}
        ${CMAKEHPCE_DIR}
        $ENV{CMakeHPCE_DIR}
        $ENV{CMAKEHPCE_DIR}
        ENV CMAKEHPCE_DIR
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

# Find the path to the file "source_file.hpp" on the system. Store the final
# path in the variables CMakeHPCE_INCLUDE_DIR. The HINTS, PATHS, and
# PATH_SUFFIXES, arguments have the same meaning as in find_library().
find_path(CMakeHPCE_INCLUDE_DIR source_file.hpp
    HINTS
        ${CMakeHPCE_DIR}
        ${CMAKEHPCE_DIR}
        $ENV{CMakeHPCE_DIR}
        $ENV{CMAKEHPCE_DIR}
        ENV CMAKEHPCE_DIR
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
find_package_handle_standard_args(CMakeHPCE
    "\nCMakeHPCE not found --- You can download it using:\n\tgit clone 
    https://github.com/pr0f3ss/HPCE\n and setting the CMAKEHPCE_DIR environment variable accordingly"
    CMakeHPCE_LIB CMakeHPCE_INCLUDE_DIR)

# These variables don't show up in the GUI version of CMake. Not required but
# people seem to do this...
mark_as_advanced(CMakeHPCE_INCLUDE_DIR CMakeHPCE_LIB)

# Finish defining the variables specified above. Variables names here follow
# CMake convention.
set(CMakeHPCE_INCLUDE_DIRS ${CMakeHPCE_INCLUDE_DIR})
set(CMakeHPCE_LIBRARIES ${CMakeHPCE_LIB})

# If the above CMake code was successful and we found the library, and there is
# no target defined, lets make one.
if(CMakeHPCE_FOUND AND NOT TARGET CMakeHPCE::CMakeHPCE)
    add_library(CMakeHPCE::CMakeHPCE UNKNOWN IMPORTED)
    # Set location of interface include directory, i.e., the directory
    # containing the header files for the installed library
    set_target_properties(CMakeHPCE::CMakeHPCE PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${CMakeHPCE_INCLUDE_DIRS}"
        )

    # Set location of the installed library
    set_target_properties(CMakeHPCE::CMakeHPCE PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
        IMPORTED_LOCATION "${CMakeHPCE_LIBRARIES}"
        )
endif()