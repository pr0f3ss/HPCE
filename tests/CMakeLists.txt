cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 17)

# 
set(HPCE_TESTS_SRC
    hpce_tests.cpp
    hpce_test_driver.cpp
)

set(HPCE_TESTS_HEADER
    catch.hpp
)

PREPEND(HPCE_TESTS_SRC)

add_executable(TestHPCE ${HPCE_TESTS_SRC} ${HPCE_TESTS_HEADER})

# Enable testing via CTest
enable_testing()

# Add test as runnable via CTest
add_test(NAME TestHPCE  COMMAND TestHPCE)

# Link unit tests against library we compiled
target_link_libraries(TestHPCE HPCE)

# Set the rpath to make sure it finds libHPCE.so
set_target_properties(TestHPCE PROPERTIES
    INSTALL_RPATH "/usr/local/lib/HPCE-1.0"
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH_USE_LINK_PATH TRUE
)