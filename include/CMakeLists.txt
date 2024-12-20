set(CMakeHPCE_INC
    hpce.hpp
    pgn_reader.hpp
    hpce_test_driver.hpp
)

PREPEND(CMakeHPCE_INC)
set(CMakeHPCE_INC ${CMakeHPCE_INC}  PARENT_SCOPE)