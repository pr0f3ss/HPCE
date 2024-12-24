cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 17)

project(HPCE VERSION 1.0)
set(HPCE_VERSION 1.0)

# enable unit testing via "make test" once the code has been compiled.
include(CTest)

# Function to prepend the subdirectory to source files in subdirectories
function(PREPEND var )
   set(listVar "")
   foreach(f ${${var}})
       list(APPEND listVar "${CMAKE_CURRENT_SOURCE_DIR}/${f}")
   endforeach(f)
   set(${var} "${listVar}" PARENT_SCOPE)
endfunction(PREPEND)

# Put executables and static libraries outside the build directory in
# bin/ and lib/, respectively.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")

# Include source code and headers.
add_subdirectory(src)
add_subdirectory(include)

# Add the test directory. It is optional and can be disabled during with
#   cmake -DBUILD_TESTING=OFF ..
# To run unit tests produced here, we only need to run:
#   make test
# or
#   ctest 
# In case your tests are printing to console, you can view their output to
# stdout with:
#   ctest -V

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

# Add the library HPCE as a target, with the contents of src/ and include/
# as dependencies.
add_library(HPCE STATIC ${HPCE_SRC} ${HPCE_INC})

# These variables slightly modify the install location to allow for version
# specific installations.
set(HPCE_INCLUDE_DEST "include/HPCE-${HPCE_VERSION}")
set(HPCE_LIB_DEST "lib/HPCE-${HPCE_VERSION}")


# generator expressions are needed for the include directories, since installing 
# headers changes the include path.
# Specify that HPCE requires the files located in the include/ directory at
# compile time. This would normally look like 
#   target_include_directories(HPCE PUBLIC include/)
# PUBLIC means that other libraries including HPCE should also include the
# directory include/.
# However, there is a catch. If we are installing the project in
# CMAKE_INSTALL_PREFIX, we can't specify include/ in the build directory: we have 
# copied the contents of include to CMAKE_INSTALL_PREFIX/include and we would
# like  other projects to include this directory instead of include/. The following
# CMake command handles this. $<BUILD_INTERFACE:...> and
# $<INSTALL_INTERFACE:...> are macros whose values change depending on if we are
# simply building the code or if we are installing it.
target_include_directories(HPCE PUBLIC
   # headers to include when building from source
   $<BUILD_INTERFACE:${HPCE_SOURCE_DIR}/include> 
   $<BUILD_INTERFACE:${HPCE_BINARY_DIR}/include> 

   # headers to include when installing  
   # (implicitly prefixes with ${CMAKE_INSTALL_PREFIX}).
   $<INSTALL_INTERFACE:include> 
   )

# Specify that HPCE requires LAPACK to link properly. Ideally, LAPACK would
# specify LAPACK::LAPACK for linking so that we can avoid using the variables.
# However, each package is different and one must check the documentation to 
# see what variables are defined.
# target_link_libraries(HPCE ${LAPACK_LIBRARIES})

# Install HPCE in CMAKE_INSTALL_PREFIX (defaults to /usr/local on linux). 
# To change the install location, run 
#   cmake -DCMAKE_INSTALL_PREFIX=<desired-install-path> ..

# install(...) specifies installation rules for the project. It can specify
# location of installed files on the system, user permissions, build
# configurations, etc. Here, we are only copying files.
# install(TARGETS ...) specifies rules for installing targets. 
# Here, we are taking a target or list of targets (HPCE) and telling CMake
# the following:
#   - put shared libraries associated with HPCE in ${HPCE_LIB_DEST}
#   - put static libraries associated with HPCE in ${HPCE_LIB_DEST}
#   - put include files associated with HPCE in ${HPCE_INCLUDE_DEST}
# We also need to specify the export that is associated with HPCE; an export 
# is just a list of targets to be installed.
# So we are associating HPCE with HPCETargets.
install(
    # targets to install
    TARGETS HPCE 
    # name of the CMake "export group" containing the targets we want to install
    EXPORT HPCETargets
    # Dynamic, static library and include destination locations after running
    # "make install"
    LIBRARY DESTINATION ${HPCE_LIB_DEST}
    ARCHIVE DESTINATION ${HPCE_LIB_DEST} 
    INCLUDES DESTINATION ${HPCE_INCLUDE_DEST}
    )

# We now need to install the export HPCETargets that we defined above. This
# is needed in order for another project to import HPCE using 
#   find_package(HPCE)
# find_package(HPCE) will look for HPCE-config.cmake to provide
# information about the targets contained in the project HPCE. Fortunately,
# this is specified in the export HPCETargets, so we will install this too.
# install(EXPORT ...) will install the information about an export. Here, we
# save it to a file {$HPCE_LIB_DEST}/HPCETargets.cmake and prepend 
# everything inside HPCETargets  with the namespace HPCE::.
install(
    # The export we want to save (matches name defined above containing the
    # install targets)
    EXPORT HPCETargets
    # CMake file in which to store the export's information
    FILE  HPCETargets.cmake
    # Namespace prepends all targets in the export (when we import later, we
    # will use HPCE::HPCE)
    NAMESPACE HPCE::
    # where to place the resulting file (here, we're putting it with the library)
    DESTINATION ${HPCE_LIB_DEST}
    )

# install(FILES ...) simply puts files in a certain place with certain
# properties. We're just copying include files to the desired include directory
# here.
install(FILES ${HPCE_INC} DESTINATION ${HPCE_INCLUDE_DEST})

# Write a "version file" in case someone wants to only load a particular version of
# HPCE 
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    HPCEConfigVersion.cmake
    VERSION ${HPCE_VERSION}
    COMPATIBILITY AnyNewerVersion
    )

# Copies the resulting CMake config files to the installed library directory
install(
    FILES 
        "cmake/HPCE-config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/HPCEConfigVersion.cmake"
    DESTINATION ${HPCE_LIB_DEST}
    )