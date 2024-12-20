#define CATCH_CONFIG_MAIN

#include "../include/hpce.hpp"
#include "../include/hpce_test_driver.hpp"
#include <iostream>
#include <string>
#include "catch.hpp"

TEST_CASE("Test a_library_function", "[unit-test]"){
    HPCE_Test_Driver test_driver = HPCE_Test_Driver();
    REQUIRE(1 == 1);
}