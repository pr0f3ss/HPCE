set(CMakeHPCE_SRC
    hpce.cpp
    pgn_reader.cpp
)

PREPEND(CMakeHPCE_SRC)
set(CMakeHPCE_SRC ${CMakeHPCE_SRC}  PARENT_SCOPE)