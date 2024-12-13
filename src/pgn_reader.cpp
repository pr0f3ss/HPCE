#include "pgn_reader.h"
#include "hpce.h"

#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <string>

/**
 * Default constructor. Initializes test driver class.
 */
PGN_Reader::PGN_Reader() {

}

/**
 * Default deconstructor.
 */
PGN_Reader::~PGN_Reader() { }

/**
 * Returns all games contained in the specified PGN file as a vector of PGN_Chess_Games.
 */
std::vector<PGN_Chess_Game> PGN_Reader::return_games(std::string file_path) {
    file_stream.open(file_path);

    std::string curr_line;

    while(!file_stream.eof()){
        file_stream.getline(curr_line);
    }
}