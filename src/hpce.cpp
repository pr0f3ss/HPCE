#include "../include/hpce.hpp"
#include "../include/pgn_reader.hpp"
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <string>

/**
 * Default constructor. Initializes board and variagbles and prints the board.
 */
Chess_Board::Chess_Board() {
  turn = 0;

  init_board();
  play_move("e4");
  play_move("e5");

  play_move("Nf3");
  play_move("Nc6");

  play_move("Bb5");
  play_move("a6");

  play_move("O-O");
  play_move("d6");

  play_move("a3");
  play_move("Bd7");

  play_move("a4");
  play_move("Qe7");

  play_move("a5");
  play_move("O-O-O");

  print_board();
}

/**
 * Default deconstructor.
 */
Chess_Board::~Chess_Board() {}

/**
 * Tries playing a move and updates the board if the move is legal. Returns
 * whether a move is legal (1 = legal).
 * @param input move notation
 */
int Chess_Board::play_move(std::string move) {
  int rank_from = 0, file_from = 0, rank_to = 0, file_to = 0;

  int isLegal = is_legal_move(move, rank_from, file_from, rank_to, file_to);
  if (isLegal) {
    // board from-position is updated to be empty and to-position gets updated
    board[rank_to][file_to] = board[rank_from][file_from];
    board[rank_from][file_from] = empty;

    // Special case: Castling;
    // King position gets updated already, we just have to move the rook
    // accordingly
    if (move == "O-O") {
      king_moved[turn] = 1;
      rook_moved[turn][1] = 1;
      if (turn == WHITE) {
        board[7][7] = empty;
        board[7][5] = w_rook;
      } else {
        board[0][7] = empty;
        board[0][5] = w_rook;
      }
    } else if (move == "O-O-O") {
      king_moved[turn] = 1;
      rook_moved[turn][0] = 1;
      if (turn == WHITE) {
        board[7][0] = empty;
        board[7][3] = w_rook;
      } else {
        board[0][0] = empty;
        board[0][3] = w_rook;
      }
    }

    // update king/rook move variable
    if (move[0] == 'K') {
      king_moved[turn] = 1;
    }

    if (move[0] == 'R') {
      if (file_from == 0) {
        rook_moved[turn][0];
      } else {
        rook_moved[turn][1];
      }
    }

    turn = !turn;
    return 1;
  }
  return 0;
}

/**
 * Prints the board to stdout for debugging purposes.
 */
int Chess_Board::print_board() {
  for (int i = 0; i < board_size; i++) {
    std::cout << "|";
    for (int j = 0; j < board_size; j++) {
      std::cout << board[i][j].symbol << "|";
    }
    std::cout << "\n________________\n";
  }

  return 1;
}

/**
 * Initializes the board.
 */
void Chess_Board::init_board() {
  for (int j = 0; j < board_size; j++) {
    board[1][j] = b_pawn;
    board[6][j] = w_pawn;
  }

  for (int i = 2; i < 6; i++) {
    for (int j = 0; j < board_size; j++) {
      board[i][j] = empty;
    }
  }

  // 8th rank black
  board[0][0] = b_rook;
  board[0][1] = b_knight;
  board[0][2] = b_bishop;
  board[0][3] = b_queen;
  board[0][4] = b_king;
  board[0][5] = b_bishop;
  board[0][6] = b_knight;
  board[0][7] = b_rook;
  king_pos[1][0] = 0;
  king_pos[1][1] = 4;

  // 1st rank white
  board[7][0] = w_rook;
  board[7][1] = w_knight;
  board[7][2] = w_bishop;
  board[7][3] = w_queen;
  board[7][4] = w_king;
  board[7][5] = w_bishop;
  board[7][6] = w_knight;
  board[7][7] = w_rook;
  king_pos[0][0] = 7;
  king_pos[0][1] = 4;

  king_moved[0] = 0;
  king_moved[1] = 0;

  rook_moved[0][0] = 0;
  rook_moved[0][1] = 0;
  rook_moved[1][0] = 0;
  rook_moved[1][1] = 0;
}

/**
 * Returns the current game score (in standard notation).
 */
int Chess_Board::get_score() {
  int score = 0;

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      Figure curr = board[i][j];
      score += curr.color ? -curr.value : curr.value;
    }
  }
  return score;
}

/**
 * Returns 1 if and only if all move sequences in referenced game are legal.
 * @param input pgn-based chess game
 * @param output 1 iff legal, else 0.
 */
int Chess_Board::is_legal_game(PGN_Chess_Game game) {
  turn = 0;

  init_board();

  std::vector<Move> move_sequence = game.get_move_sequence();

  // TODO: Implement error logic
  for (Move move : move_sequence)
    play_move(move.move_notation);

  return 1;
}

/**
 * Returns whether a move is legal (1 = legal). If the move is legal, we pass
 * the relevant rank and file positions for the move to be played. Note that
 * there are no guarantees for the rank and file positions if the move is
 * illegal.
 * @param input move notation
 * @param output rank of figure before move
 * @param output file of figure before me
 * @param output rank of figure after move
 * @param output file of figure after move
 */
int Chess_Board::is_legal_move(std::string move, int &rank_from, int &file_from,
                               int &rank_to, int &file_to) {
  // If first letter is uppercase, it is a not a pawn
  if (isupper(move[0])) {
    switch (move[0]) {
    case 'N':
      return handle_knight(move, rank_from, file_from, rank_to, file_to);
    case 'B':
      return handle_bishop(move, rank_from, file_from, rank_to, file_to);
    case 'R':
      return handle_rook(move, rank_from, file_from, rank_to, file_to);
    case 'Q':
      return handle_queen(move, rank_from, file_from, rank_to, file_to);
    case 'K':
      return handle_king(move, rank_from, file_from, rank_to, file_to);
    case 'O':
      return handle_castling(move, rank_from, file_from, rank_to, file_to);
    default:
      return 0;
    }
  } else {
    return handle_pawn(move, rank_from, file_from, rank_to, file_to);
  }
  return 0;
}

/**
 * Checks if the figure can move from (rank_from, file_from) to (rank_to,
 * file_to) without putting the king in check.
 * @param input  referenced figure to check
 * @param output rank of figure before move
 * @param output file of figure before me
 * @param output rank of figure after move
 * @param output file of figure after move
 */
int Chess_Board::is_legal_figure_move(int figure_type, int &rank_from,
                                      int &file_from, int &rank_to,
                                      int &file_to) {
  Figure curr = board[rank_from][file_from];
  return curr.color == turn && curr.type == figure_type &&
         !king_into_check(rank_from, file_from, rank_to, file_to);
}

/**
 * Returns whether a pawn move is legal (1 = legal). Same notations apply as in
 * is_legal_move().
 * @param input move notation
 * @param output rank of figure before move
 * @param output file of figure before me
 * @param output rank of figure after move
 * @param output file of figure after move
 */
int Chess_Board::handle_pawn(std::string move, int &rank_from, int &file_from,
                             int &rank_to, int &file_to) {
  if (move.find('x') != std::string::npos) { // capture case
    // todo: implement
    return 1;
  } else { // pawn moves normally
    int file = file_to_int(move[0]);
    int rank = 8 - (move[1] - '0');

    // Special case: Check if moving forward two squares
    if (rank == 4 && turn == 0) { // for white
      if (board[4][file].empty == 1 && board[5][file].empty == 1) {
        Figure pos = board[6][file];
        rank_from = 6;
        file_from = file;
        rank_to = rank;
        file_to = file;
        return pos.type == 0 && pos.color == 0 &&
               !king_into_check(rank_from, file_from, rank_to, file_to);
      }
    } else if (rank == 3 && turn == 1) { // for black
      if (board[3][file].empty == 1 && board[2][file].empty == 1) {
        Figure pos = board[1][file];
        rank_from = 1;
        file_from = file;
        rank_to = rank;
        file_to = file;
        return pos.type == 0 && pos.color == 1 &&
               !king_into_check(rank_from, file_from, rank_to, file_to);
      }
    }

    // Pawn moves forward one square
    // Special case: Promotion
    if (rank == 0 || rank == 7) {
      char promotion_piece = move[2];
      // todo: implement
    }
    if (board[rank][file].empty == 1) {
      int direction = turn ? -1 : 1;
      rank_from = rank + direction;
      file_from = file;
      rank_to = rank;
      file_to = file;
      Figure pos = board[rank_from][file_from];
      return pos.type == 0 &&
             !king_into_check(rank_from, file_from, rank_to, file_to);
    }

    return 0;
  }
}

/**
 * Returns whether a knight move is legal (1 = legal). Same notations apply as
 * in is_legal_move().
 * @param input move notation
 * @param output rank of figure before move
 * @param output file of figure before me
 * @param output rank of figure after move
 * @param output file of figure after move
 */
int Chess_Board::retrieve_knight_location(std::string move, int &rank_from,
                                          int &file_from, int &rank_to,
                                          int &file_to) {
  // iterate over possible knight positions (copied lots of code from
  // king_into_check) bottom cases
  int r, f;
  r = rank_to + 2;
  f = file_to;

  if (r < 8) {
    if (f + 1 < 8) {
      Figure curr = board[r][f + 1];
      if (curr.color == turn && curr.type == KNIGHT_TYPE) {
        rank_from = r;
        file_from = f + 1;
        return !king_into_check(rank_from, file_from, rank_to, file_to);
      }
    }
    if (f - 1 >= 0) {
      Figure curr = board[r][f - 1];
      if (curr.color == turn && curr.type == KNIGHT_TYPE) {
        rank_from = r;
        file_from = f - 1;
        return !king_into_check(rank_from, file_from, rank_to, file_to);
      }
    }
  }

  // top cases
  r = rank_to - 2;
  f = file_to;
  if (r >= 0) {
    if (f + 1 < 8) {
      Figure curr = board[r][f + 1];
      if (curr.color == turn && curr.type == KNIGHT_TYPE) {
        rank_from = r;
        file_from = f + 1;
        return !king_into_check(rank_from, file_from, rank_to, file_to);
      }
    }
    if (f - 1 >= 0) {
      Figure curr = board[r][f - 1];
      if (curr.color == turn && curr.type == KNIGHT_TYPE) {
        rank_from = r;
        file_from = f - 1;
        return !king_into_check(rank_from, file_from, rank_to, file_to);
      }
    }
  }

  // left cases
  r = rank_to;
  f = file_to - 2;
  if (f >= 0) {
    if (r + 1 < 8) {
      Figure curr = board[r + 1][f];
      if (curr.color == turn && curr.type == KNIGHT_TYPE) {
        rank_from = r + 1;
        file_from = f;
        return !king_into_check(rank_from, file_from, rank_to, file_to);
      }
    }
    if (r - 1 >= 0) {
      Figure curr = board[r - 1][f];
      if (curr.color == turn && curr.type == KNIGHT_TYPE) {
        rank_from = r - 1;
        file_from = f;
        return !king_into_check(rank_from, file_from, rank_to, file_to);
      }
    }
  }

  // right cases
  r = rank_to;
  f = file_to + 2;
  if (f < 8) {
    if (r + 1 < 8) {
      Figure curr = board[r + 1][f];
      if (curr.color == turn && curr.type == KNIGHT_TYPE) {
        rank_from = r + 1;
        file_from = f;
        return !king_into_check(rank_from, file_from, rank_to, file_to);
      }
    }
    if (r - 1 >= 0) {
      Figure curr = board[r - 1][f];
      if (curr.color == turn && curr.type == KNIGHT_TYPE) {
        rank_from = r - 1;
        file_from = f;
        return !king_into_check(rank_from, file_from, rank_to, file_to);
      }
    }
  }

  return 0;
}

/**
 * Returns whether a knight move is legal (1 = legal). Same notations apply as
 * in is_legal_move().
 * @param input move notation
 * @param output rank of figure before move
 * @param output file of figure before me
 * @param output rank of figure after move
 * @param output file of figure after move
 */
int Chess_Board::handle_knight(std::string move, int &rank_from, int &file_from,
                               int &rank_to, int &file_to) {
  bool is_capture = move.find('x') != std::string::npos;

  if (is_capture) {
    if (move.length() == 4) { // unambiguous capture
      file_to = file_to_int(move[2]);
      rank_to = 8 - (move[3] - '0');
      // Check all possible knight moves to the target square
      for (int rank_offset : {-2, -1, 1, 2}) {
        for (int file_offset : {-2, -1, 1, 2}) {
          if (abs(rank_offset) != abs(file_offset)) {
            rank_from = rank_to + rank_offset;
            file_from = file_to + file_offset;
            if (is_legal_figure_move(KNIGHT_TYPE, rank_from, file_from, rank_to,
                                     file_to))
              return 1;
          }
        }
      }
    } else { // disambiguous capture
      return handle_knight_disambiguation(move, rank_from, file_from, rank_to,
                                          file_to, true);
    }
  } else {                    // no capture
    if (move.length() == 3) { // unambiguous move
      file_to = file_to_int(move[1]);
      rank_to = 8 - (move[2] - '0');
      // Check all possible knight moves to the target square
      for (int rank_offset : {-2, -1, 1, 2}) {
        for (int file_offset : {-2, -1, 1, 2}) {
          if (abs(rank_offset) != abs(file_offset)) {
            rank_from = rank_to + rank_offset;
            file_from = file_to + file_offset;
            if (is_legal_figure_move(KNIGHT_TYPE, rank_from, file_from, rank_to,
                                     file_to))
              return 1;
          }
        }
      }
    } else { // disambiguous move
      return handle_knight_disambiguation(move, rank_from, file_from, rank_to,
                                          file_to, false);
    }
  }

  return 0; // Illegal move
}

/**
 * Handles the disambiguation logic for knight moves.
 * @param input move notation
 * @param output rank of figure before move
 * @param output file of figure before me
 * @param output rank of figure after move
 * @param output file of figure after move
 * @param input bool denoting if move captures piece
 */
int Chess_Board::handle_knight_disambiguation(std::string move, int &rank_from,
                                              int &file_from, int &rank_to,
                                              int &file_to, bool is_capture) {
  int move_length = move.length();
  int file_index = is_capture ? 3 : 2;
  int rank_index = is_capture ? 4 : 3;

  file_to = file_to_int(move[file_index]);
  rank_to = 8 - (move[rank_index] - '0');

  if (move_length == (is_capture ? 5 : 4)) { // file or rank disambiguation
    if (is_file(move[1])) {                  // file disambiguation
      file_from = file_to_int(move[1]);
      // Check all possible knight moves from the specified file
      for (int rank_offset : {-2, -1, 1, 2}) {
        for (int file_offset : {-2, -1, 1, 2}) {
          if (abs(rank_offset) != abs(file_offset)) {
            rank_from = rank_to + rank_offset;
            file_from = file_to + file_offset;
            if (is_legal_figure_move(KNIGHT_TYPE, rank_from, file_from, rank_to,
                                     file_to))
              return true;
          }
        }
      }
    } else { // rank disambiguation
      rank_from = 8 - (move[2] - '0');
      // Check all possible knight moves from the specified rank
      for (int rank_offset : {-2, -1, 1, 2}) {
        for (int file_offset : {-2, -1, 1, 2}) {
          if (abs(rank_offset) != abs(file_offset)) {
            rank_from = rank_to + rank_offset;
            file_from = file_to + file_offset;
            if (is_legal_figure_move(KNIGHT_TYPE, rank_from, file_from, rank_to,
                                     file_to))
              return true;
          }
        }
      }
    }
  } else if (move_length ==
             (is_capture ? 6 : 5)) { // both file and rank disambiguation
    file_from = file_to_int(move[1]);
    rank_from = 8 - (move[2] - '0');
    return is_legal_figure_move(KNIGHT_TYPE, rank_from, file_from, rank_to,
                                file_to);
  }

  return false;
}

/**
 * Returns whether a bishop move is legal (1 = legal). Same notations apply as
 * in is_legal_move().
 * @param input move notation
 * @param output rank of figure before move
 * @param output file of figure before me
 * @param output rank of figure after move
 * @param output file of figure after move
 */
int Chess_Board::handle_bishop(std::string move, int &rank_from, int &file_from,
                               int &rank_to, int &file_to) {
  bool is_capture = move.find('x') != std::string::npos;

  if (is_capture) {
    if (move.length() == 4) { // unambiguous capture
      file_to = file_to_int(move[2]);
      rank_to = 8 - (move[3] - '0');
      return check_diagonals(BISHOP_TYPE, rank_from, file_from, rank_to,
                             file_to);
    } else { // disambiguous capture
      return handle_bishop_disambiguation(move, rank_from, file_from, rank_to,
                                          file_to, true);
    }
  } else {                    // no capture
    if (move.length() == 3) { // unambiguous move
      file_to = file_to_int(move[1]);
      rank_to = 8 - (move[2] - '0');
      return check_diagonals(BISHOP_TYPE, rank_from, file_from, rank_to,
                             file_to);
    } else { // disambiguous move
      return handle_bishop_disambiguation(move, rank_from, file_from, rank_to,
                                          file_to, false);
    }
  }
}

/**
 * Handles the disambiguation logic for bishop moves.
 * @param input move notation
 * @param output rank of figure before move
 * @param output file of figure before me
 * @param output rank of figure after move
 * @param output file of figure after move
 * @param input bool denoting if move captures piece
 */
int Chess_Board::handle_bishop_disambiguation(std::string move, int &rank_from,
                                              int &file_from, int &rank_to,
                                              int &file_to, bool is_capture) {
  int move_length = move.length();
  int file_index = is_capture ? 3 : 2;
  int rank_index = is_capture ? 4 : 3;

  file_to = file_to_int(move[file_index]);
  rank_to = 8 - (move[rank_index] - '0');

  if (move_length == (is_capture ? 5 : 4)) { // file or rank disambiguation
    if (is_file(move[1])) {                  // file disambiguation
      file_from = file_to_int(move[1]);
      int dist = file_from - file_to;

      for (int offset : {dist, -dist}) {
        rank_from = rank_to + offset;
        if (is_legal_figure_move(BISHOP_TYPE, rank_from, file_from, rank_to,
                                 file_to))
          return true;
      }
    } else { // rank disambiguation
      rank_from = 8 - (move[2] - '0');
      int dist = rank_from - rank_to;

      for (int offset : {dist, -dist}) {
        file_from = file_to + offset;
        if (is_legal_figure_move(BISHOP_TYPE, rank_from, file_from, rank_to,
                                 file_to))
          return true;
      }
    }
  } else if (move_length ==
             (is_capture ? 6 : 5)) { // both file and rank disambiguation
    file_from = file_to_int(move[1]);
    rank_from = 8 - (move[2] - '0');
    return is_legal_figure_move(BISHOP_TYPE, rank_from, file_from, rank_to,
                                file_to);
  }

  return false;
}

/**
 * Returns whether a rook move is legal (1 = legal). Same notations apply as in
 * is_legal_move().
 * @param input move notation
 * @param output rank of figure before move
 * @param output file of figure before me
 * @param output rank of figure after move
 * @param output file of figure after move
 */
int Chess_Board::handle_rook(std::string move, int &rank_from, int &file_from,
                             int &rank_to, int &file_to) {
  if (move.find('x') != std::string::npos) { // capture case
    // todo: implement
    return 1;
  } else {                    // no capture case
    if (move.length() == 3) { // unambiguous move
      int file = file_to_int(move[1]);
      int rank = 8 - (move[2] - '0');

      rank_to = rank;
      file_to = file;

      // copied mostly from king_into_check()

      // file traversal, left of destination square
      for (int i = file_to - 1; i >= 0; i--) {
        Figure curr = board[rank_to][i];
        if (curr.color == turn && curr.type == ROOK_TYPE) {
          rank_from = rank_to;
          file_from = i;
          return !king_into_check(rank_from, file_from, rank_to, file_to);
        }
        if (curr.color == !turn)
          break;
      }

      // file traversal, right of destination square
      for (int i = file_to + 1; i < 8; i++) {
        Figure curr = board[rank_to][i];
        if (curr.color == turn && curr.type == ROOK_TYPE) {
          rank_from = rank_to;
          file_from = i;
          return !king_into_check(rank_from, file_from, rank_to, file_to);
        }
        if (curr.color == !turn)
          break;
      }

      // rank traversal, bottom of destination square
      for (int i = rank_to + 1; i < 8; i++) {
        Figure curr = board[i][rank_to];
        if (curr.color == turn && curr.type == ROOK_TYPE) {
          rank_from = i;
          file_from = file_to;
          return !king_into_check(rank_from, file_from, rank_to, file_to);
        }
        if (curr.color == !turn)
          break;
      }

      // rank traversal, top of destination square
      for (int i = rank_to - 1; i >= 0; i--) {
        Figure curr = board[i][rank_to];
        if (curr.color == turn && curr.type == ROOK_TYPE) {
          rank_from = i;
          file_from = file_to;
          return !king_into_check(rank_from, file_from, rank_to, file_to);
        }
        if (curr.color == !turn)
          break;
      }

      return 0;
    } else { // ambiguous move
      // todo: implement
      return 1;
    }
  }
}

/**
 * Returns whether a queen move is legal (1 = legal). Same notations apply as in
 * is_legal_move().
 * @param input move notation
 * @param output rank of figure before move
 * @param output file of figure before me
 * @param output rank of figure after move
 * @param output file of figure after move
 */
int Chess_Board::handle_queen(std::string move, int &rank_from, int &file_from,
                              int &rank_to, int &file_to) {
  if (move.find('x') != std::string::npos) { // capture case
    // todo: implement
    return 1;
  } else {                    // no capture case
    if (move.length() == 3) { // unambiguous move
      int file = file_to_int(move[1]);
      int rank = 8 - (move[2] - '0');

      rank_to = rank;
      file_to = file;

      // copied mostly from handle_rook() and handle_bishop() (as their moves
      // are identical)

      // file traversal, left of destination square
      for (int i = file_to - 1; i >= 0; i--) {
        Figure curr = board[rank_to][i];
        if (curr.color == turn && curr.type == QUEEN_TYPE) {
          rank_from = rank_to;
          file_from = i;
          return !king_into_check(rank_from, file_from, rank_to, file_to);
        }
        if (curr.color == !turn)
          break;
      }

      // file traversal, right of destination square
      for (int i = file_to + 1; i < 8; i++) {
        Figure curr = board[rank_to][i];
        if (curr.color == turn && curr.type == QUEEN_TYPE) {
          rank_from = rank_to;
          file_from = i;
          return !king_into_check(rank_from, file_from, rank_to, file_to);
        }
        if (curr.color == !turn)
          break;
      }

      // rank traversal, bottom of destination square
      for (int i = rank_to + 1; i < 8; i++) {
        Figure curr = board[i][rank_to];
        if (curr.color == turn && curr.type == QUEEN_TYPE) {
          rank_from = i;
          file_from = file_to;
          return !king_into_check(rank_from, file_from, rank_to, file_to);
        }
        if (curr.color == !turn)
          break;
      }

      // rank traversal, top of destination square
      for (int i = rank_to - 1; i >= 0; i--) {
        Figure curr = board[i][rank_to];
        if (curr.color == turn && curr.type == QUEEN_TYPE) {
          rank_from = i;
          file_from = file_to;
          return !king_into_check(rank_from, file_from, rank_to, file_to);
        }
        if (curr.color == !turn)
          break;
      }

      // diagonal traversal, bottom-left
      for (int i = rank_to + 1, j = file_to - 1; i < 8 && j >= 0; i++, j--) {
        Figure curr = board[i][j];
        if (curr.color == turn && curr.type == QUEEN_TYPE) {
          rank_from = i;
          file_from = j;
          return !king_into_check(rank_from, file_from, rank_to, file_to);
        }
        if (curr.color == !turn)
          break;
      }

      // diagonal traversal, bottom-right

      for (int i = rank_to + 1, j = file_to + 1; i < 8 && j < 8; i++, j++) {
        Figure curr = board[i][j];
        if (curr.color == turn && curr.type == QUEEN_TYPE) {
          rank_from = i;
          file_from = j;
          return !king_into_check(rank_from, file_from, rank_to, file_to);
        }
        if (curr.color == !turn)
          break;
      }

      // diagonal traversal, top-left

      for (int i = rank_to - 1, j = file_to - 1; i >= 0 && j >= 0; i--, j--) {
        Figure curr = board[i][j];
        if (curr.color == turn && curr.type == QUEEN_TYPE) {
          rank_from = i;
          file_from = j;
          return !king_into_check(rank_from, file_from, rank_to, file_to);
        }
        if (curr.color == !turn)
          break;
      }

      // diagonal traversal, top-right
      for (int i = rank_to - 1, j = file_to + 1; i >= 0 && j < 8; i--, j++) {
        Figure curr = board[i][j];
        if (curr.color == turn && curr.type == QUEEN_TYPE) {
          rank_from = i;
          file_from = j;
          return !king_into_check(rank_from, file_from, rank_to, file_to);
        }
        if (curr.color == !turn)
          break;
      }
      return 0;
    } else { // ambiguous move
      // todo: implement
      return 1;
    }
  }
}

/**
 * Returns whether a king move is legal (1 = legal). Same notations apply as in
 * is_legal_move(). Note that castling is handled in a separate function:
 * handle_castling()
 * @param input move notation
 * @param output rank of figure before move
 * @param output file of figure before me
 * @param output rank of figure after move
 * @param output file of figure after move
 */
int Chess_Board::handle_king(std::string move, int &rank_from, int &file_from,
                             int &rank_to, int &file_to) {
  if (move.find('x') != std::string::npos) { // capture case
    // todo: implement
    return 1;
  } else { // no capture case
  }

  return 1;
}

/**
 * Returns whether a castling move is legal (1 = legal). Same notations apply as
 * in is_legal_move().
 * @param input move notation
 * @param output rank of figure before move
 * @param output file of figure before me
 * @param output rank of figure after move
 * @param output file of figure after move
 */
int Chess_Board::handle_castling(std::string move, int &rank_from,
                                 int &file_from, int &rank_to, int &file_to) {
  const int KING_SIDE_FILE = 6;
  const int QUEEN_SIDE_FILE = 2;

  file_from = 4;
  rank_from = (turn == WHITE) ? 7 : 0;
  rank_to = rank_from;

  // Helper lambda to check if path is clear
  auto isPathClear = [&](std::initializer_list<int> files) -> bool {
    for (int file : files) {
      if (!board[rank_to][file].empty) {
        return false;
      }
    }
    return true;
  };

  if (move == "O-O") { // king side castle
    file_to = KING_SIDE_FILE;
    if (!isPathClear({5, 6}))
      return 0;

    // Special case: Check if the king traversed a file that would have put him
    // in check
    return !king_moved[turn] && !rook_moved[turn][1] &&
           !king_into_check(rank_from, file_from, rank_to, file_to) &&
           !king_into_check(rank_from, file_from, rank_to, file_to - 1);

  } else if (move == "O-O-O") { // queen side castle
    file_to = QUEEN_SIDE_FILE;
    if (!isPathClear({1, 2, 3}))
      return 0;

    // Special case: Check if the king traversed a file that would have put him
    // in check
    return !king_moved[turn] && !rook_moved[turn][0] &&
           !king_into_check(rank_from, file_from, rank_to, file_to) &&
           !king_into_check(rank_from, file_from, rank_to, file_to + 1);
  }

  return 0; // invalid move format
}

/**
 * Returns whether the king will be in check after the referenced figure has
 * moved.
 * @param input rank of figure before move
 * @param input file of figure before me
 * @param input rank of figure after move
 * @param input file of figure after move
 */
int Chess_Board::king_into_check(int rank_from, int file_from, int rank_to,
                                 int file_to) {
  // Update the board to after-move state
  Figure fig_from = board[rank_from][file_from];
  Figure fig_to = board[rank_to][file_to];
  board[rank_to][file_to] = fig_from;
  board[rank_from][file_from] = empty;

  int king_rank = king_pos[turn][0];
  int king_file = king_pos[turn][1];

  // Check for attacks along ranks and files (rooks and queens)
  bool isInCheck =
      is_under_straight_attack(king_rank, king_file, 1, 0) ||  // Down
      is_under_straight_attack(king_rank, king_file, -1, 0) || // Up
      is_under_straight_attack(king_rank, king_file, 0, 1) ||  // Right
      is_under_straight_attack(king_rank, king_file, 0, -1);   // Left

  // Check for attacks along diagonals (bishops and queens)
  isInCheck =
      isInCheck ||
      is_under_diagonal_attack(king_rank, king_file, 1, 1) ||  // Down-right
      is_under_diagonal_attack(king_rank, king_file, 1, -1) || // Down-left
      is_under_diagonal_attack(king_rank, king_file, -1, 1) || // Up-right
      is_under_diagonal_attack(king_rank, king_file, -1, -1);  // Up-left

  // Check for pawn attacks
  isInCheck = isInCheck || is_under_pawn_attack(king_rank, king_file);

  // Check for knight attacks
  isInCheck = isInCheck || is_under_knight_attack(king_rank, king_file);

  // Clean up board to before-move state
  board[rank_to][file_to] = fig_to;
  board[rank_from][file_from] = fig_from;

  return isInCheck ? 1 : 0;
}

/**
 * Checks if a square is under attack by a rook or queen along a straight
 * line.
 */
int Chess_Board::is_under_straight_attack(int rank, int file, int delta_rank,
                                          int delta_file) {
  int r = rank + delta_rank;
  int f = file + delta_file;

  while (r >= 0 && r < 8 && f >= 0 && f < 8) {
    Figure curr = board[r][f];
    if (curr.color == turn)
      break; // Friendly piece blocks
    if (curr.color == !turn) {
      return (curr.type == ROOK_TYPE || curr.type == QUEEN_TYPE);
    }
    r += delta_rank;
    f += delta_file;
  }
  return false;
}

/**
 * Checks if a square is under attack by a bishop or queen along a diagonal.
 */
int Chess_Board::is_under_diagonal_attack(int rank, int file, int delta_rank,
                                          int delta_file) {
  int r = rank + delta_rank;
  int f = file + delta_file;

  while (r >= 0 && r < 8 && f >= 0 && f < 8) {
    Figure curr = board[r][f];
    if (curr.color == turn)
      break; // Friendly piece blocks
    if (curr.color == !turn) {
      return (curr.type == BISHOP_TYPE || curr.type == QUEEN_TYPE);
    }
    r += delta_rank;
    f += delta_file;
  }
  return false;
}

/**
 * Checks if a square is under attack by a pawn.
 */
int Chess_Board::is_under_pawn_attack(int rank, int file) {
  if (turn == WHITE) {
    if (rank - 1 >= 0 && file - 1 >= 0) {
      Figure curr = board[rank - 1][file - 1];
      if (curr.color == BLACK && curr.type == PAWN_TYPE)
        return true;
    }
    if (rank - 1 >= 0 && file + 1 < 8) {
      Figure curr = board[rank - 1][file + 1];
      if (curr.color == BLACK && curr.type == PAWN_TYPE)
        return true;
    }
  } else {
    if (rank + 1 < 8 && file - 1 >= 0) {
      Figure curr = board[rank + 1][file - 1];
      if (curr.color == WHITE && curr.type == PAWN_TYPE)
        return true;
    }
    if (rank + 1 < 8 && file + 1 < 8) {
      Figure curr = board[rank + 1][file + 1];
      if (curr.color == WHITE && curr.type == PAWN_TYPE)
        return true;
    }
  }
  return false;
}

/**
 * Checks if a square is under attack by a knight.
 */
int Chess_Board::is_under_knight_attack(int rank, int file) {
  const int knight_moves[8][2] = {{2, 1}, {2, -1}, {-2, 1}, {-2, -1},
                                  {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};

  for (const auto &move : knight_moves) {
    int r = rank + move[0];
    int f = file + move[1];
    if (r >= 0 && r < 8 && f >= 0 && f < 8) {
      Figure curr = board[r][f];
      if (curr.color == !turn && curr.type == KNIGHT_TYPE)
        return true;
    }
  }
  return false;
}

/**
 * Converts a file character (a-h) to an integer (0-7).
 */
int Chess_Board::file_to_int(char file) { return file - 'a'; }

/**
 * Returns whether the current character is referring to a file or rank (1 =
 * file).
 * @param input input character notation
 * @param output 1 if character notation is referring to file, else 0
 */
int Chess_Board::is_file(char char_notation) { return isdigit(char_notation); }

/**
 * Returns 1 if figure moves own king into check (1 = check, 0 = no check, -1
 * = figure not found, -2 = other figure is blocking path). Returns
 * non-negative integer if and only if bishop figure has been found
 * @param input  int type of figure
 * @param output int
 * @param output rank of figure before move
 * @param output file of figure before move
 * @param output rank of figure after move
 * @param output file of figure after move
 */
int Chess_Board::figure_moves_into_check(int figure_type, int &rank_from,
                                         int &file_from, int &rank_to,
                                         int &file_to) {
  Figure curr = board[rank_from][file_from];
  if (curr.color == turn) {
    if (curr.type == figure_type)
      return !king_into_check(rank_from, file_from, rank_to, file_to);
    else
      return -2;
  } else if (curr.color != turn && curr.empty != 0) {
    return -2;
  }
  return -1;
}

int Chess_Board::check_diagonals(int figure_type, int &rank_from,
                                 int &file_from, int &rank_to, int &file_to) {
  // diagonal traversal, bottom-left
  for (int i = rank_to + 1, j = file_to - 1; i < 8 && j >= 0; i++, j--) {
    rank_from = i;
    file_from = j;
    int ret_move_legal = figure_moves_into_check(figure_type, rank_from,
                                                 file_from, rank_to, file_to);
    if (ret_move_legal < 0)
      break;
    else
      return ret_move_legal;
  }

  // diagonal traversal, bottom-right
  for (int i = rank_to + 1, j = file_to + 1; i < 8 && j < 8; i++, j++) {
    rank_from = i;
    file_from = j;
    int ret_move_legal = figure_moves_into_check(figure_type, rank_from,
                                                 file_from, rank_to, file_to);
    if (ret_move_legal < 0)
      break;
    else
      return ret_move_legal;
  }

  // diagonal traversal, top-left
  for (int i = rank_to - 1, j = file_to - 1; i >= 0 && j >= 0; i--, j--) {
    rank_from = i;
    file_from = j;
    int ret_move_legal = figure_moves_into_check(figure_type, rank_from,
                                                 file_from, rank_to, file_to);
    if (ret_move_legal < 0)
      break;
    else
      return ret_move_legal;
  }

  // diagonal traversal, top-right
  for (int i = rank_to - 1, j = file_to + 1; i >= 0 && j < 8; i--, j++) {
    rank_from = i;
    file_from = j;
    int ret_move_legal = figure_moves_into_check(figure_type, rank_from,
                                                 file_from, rank_to, file_to);
    if (ret_move_legal < 0)
      break;
    else
      return ret_move_legal;
  }

  return -1;
}