cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 11)

# 
set(CMakeHPCE_TESTS_SRC
    hpce_test_driver.cpp
    hpce_tests.cpp
)

PREPEND(CMakeHPCE_TESTS_SRC)

add_executable(TestCMakeHPCE ${CMakeHPCE_TESTS_SRC})

# Enable testing via CTest
enable_testing()
# Add test as runnable via CTest
add_test(NAME TestCMakeHPCE  COMMAND TestCMakeHPCE)

# Link unit tests against library we compiled
target_link_libraries(TestCMakeHPCE CMakeHPCE)