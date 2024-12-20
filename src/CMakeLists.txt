set(HPCE_SRC
    hpce.cpp
    pgn_reader.cpp
)

PREPEND(HPCE_SRC)
set(HPCE_SRC ${HPCE_SRC}  PARENT_SCOPE)