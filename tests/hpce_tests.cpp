#define CATCH_CONFIG_MAIN

#include "catch_amalgamated.hpp"
#include "../include/hpce.hpp"
#include "../include/hpce_test_driver.hpp"
#include <iostream>
#include <string>

TEST_CASE("Test a_library_function", "[unit-test]"){
    //HPCE_Test_Driver test_driver = HPCE_Test_Driver();
    REQUIRE(1 == 1);
}