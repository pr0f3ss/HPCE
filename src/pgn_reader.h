#ifndef _PGN_READER_H // include guard
#define _PGN_READER_H

#include <iostream>
#include <fstream>
#include <string>

class PGN_Reader {

public:
  PGN_Reader(void);
  ~PGN_Reader(void);

private:
  std::ifstream file_stream;
  
};

class PGN_Chess_Game {

public:
  PGN_Chess_Game(void);
  ~PGN_Chess_Game(void);

private:
  
};


#endif