#ifndef PGN_CHESS_GAME_HPP
#define PGN_CHESS_GAME_HPP

#include <map>
#include <string>
#include <vector>

struct Move {
  int move_nr;
  int turn;
  std::string move_notation;

  bool operator==(const Move &other) const {
    return move_nr == other.move_nr && turn == other.turn &&
           move_notation == other.move_notation;
  }
};

class PGN_Chess_Game {
public:
  PGN_Chess_Game(std::map<std::string, std::string> tag_pairs);
  ~PGN_Chess_Game(void);

  int add_move(Move move);
  std::map<std::string, std::string> get_tag_pairs(void);
  std::vector<Move> get_move_sequence(void);
  void set_move_sequence(std::vector<Move> &p_move_sequence);

private:
  std::map<std::string, std::string> tag_pairs;
  std::vector<Move> move_sequence;
};

#endif // PGN_CHESS_GAME_HPP
