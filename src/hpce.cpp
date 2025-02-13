#include "../include/hpce.hpp"
#include "../include/pgn_reader.hpp"
#include <algorithm>
#include <array>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <string>

/**
 * Default constructor. Initializes board and variagbles and prints the board.
 */
Chess_Board::Chess_Board() {
  turn = 0;

  en_passant_target[0] = -1; // Initialize to invalid value
  en_passant_target[1] = -1;

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

  if (!is_legal_move(move, rank_from, file_from, rank_to, file_to)) {
    return 0; // Move is not legal
  }

  // Update the board
  update_board(rank_from, file_from, rank_to, file_to);

  // Handle en passant target update
  if (move.length() == 2 &&
      (rank_to == 3 || rank_to == 4)) { // Pawn moves two squares
    int en_passant_rank = turn == WHITE ? rank_to + 1 : rank_to - 1;
    update_en_passant_target(en_passant_rank, file_to);
  } else {
    reset_en_passant_target(); // Reset en passant target after the next move
  }

  // Handle castling updates
  if (move == "O-O" || move == "O-O-O") {
    handle_castling_update(move);
  }

  // Update king/rook move flags
  update_move_flags(move[0], file_from);

  // Switch turns
  turn = !turn;

  return 1; // Move is legal
}

/**
 * Private method that functions identically to the public play_move() method,
 * but passes the file and rank params as references.
 */
int Chess_Board::play_move(std::string move, int &rank_from, int &file_from,
                           int &rank_to, int &file_to) {
  rank_from = 0, file_from = 0, rank_to = 0, file_to = 0;

  if (!is_legal_move(move, rank_from, file_from, rank_to, file_to)) {
    return 0; // Move is not legal
  }

  // Update the board
  update_board(rank_from, file_from, rank_to, file_to);

  // Handle en passant target update
  if (move.length() == 2 &&
      (rank_to == 3 || rank_to == 4)) { // Pawn moves two squares
    int en_passant_rank = turn == WHITE ? rank_to + 1 : rank_to - 1;
    update_en_passant_target(en_passant_rank, file_to);
  } else {
    reset_en_passant_target(); // Reset en passant target after the next move
  }

  // Handle castling updates
  if (move == "O-O" || move == "O-O-O") {
    handle_castling_update(move);
  }

  // Update king/rook move flags
  update_move_flags(move[0], file_from);

  // Switch turns
  turn = !turn;

  return 1; // Move is legal
}

/**
 * Updates king/rook move flags.
 */
void Chess_Board::update_move_flags(char piece, int file_from) {
  if (piece == 'K') {
    king_moved[turn] = 1;
  } else if (piece == 'R') {
    rook_moved[turn][(file_from == 0) ? 0 : 1] = 1;
  }
}

/**
 * Updates the board after a move is played.
 */
void Chess_Board::update_board(int rank_from, int file_from, int rank_to,
                               int file_to) {
  // Handle en passant capture
  if (is_en_passant_target(rank_to, file_to)) {
    // Remove the captured pawn (which is one rank behind the target square)
    int captured_rank = turn == WHITE ? rank_to + 1 : rank_to - 1;
    board[captured_rank][file_to] = empty;
  }

  // std::cout << rank_from << " " << file_from << "\n";
  // std::cout << rank_to << " " << file_to << "\n";

  board[rank_to][file_to] = board[rank_from][file_from];
  board[rank_from][file_from] = empty;
}

/**
 * Promotes the pawn piece to specified type.
 */
void Chess_Board::promote_piece(char figure_char, int rank_from, int file_from,
                                int rank_to, int file_to) {
  int figure_type;

  switch (figure_char) {
  case 'N':
    figure_type = KNIGHT_TYPE;
  case 'B':
    figure_type = BISHOP_TYPE;
  case 'R':
    figure_type = ROOK_TYPE;
  case 'Q':
    figure_type = QUEEN_TYPE;
  default:
    figure_type = EMPTY_TYPE;
  }

  Figure curr = board[rank_from][file_from];
  curr.type = figure_type;
}

/**
 * Handles castling move updates.
 */
void Chess_Board::handle_castling_update(std::string move) {
  if (move == "O-O") { // King-side castling
    king_moved[turn] = 1;
    rook_moved[turn][1] = 1;
    int rank = (turn == WHITE) ? 7 : 0;
    board[rank][7] = empty;
    board[rank][5] = (turn == WHITE) ? w_rook : b_rook;
  } else if (move == "O-O-O") { // Queen-side castling
    king_moved[turn] = 1;
    rook_moved[turn][0] = 1;
    int rank = (turn == WHITE) ? 7 : 0;
    board[rank][0] = empty;
    board[rank][3] = (turn == WHITE) ? w_rook : b_rook;
  }
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
 * Returns the one-hot encoded input sequence for the specified game.
 * It is structured as follows:
 * == 64 Input tokens of length 112 for each board location ==
 * 8 one-hot vectors of length 12 for current and past 7 positions
 * En passant and castling information
 * # positions since last capture, pawn move or castle / 100
 * 8 bools denoting if board position is repetition of position in 8 last moves
 */
Input_Sequence Chess_Board::get_input_sequence(PGN_Chess_Game &game) {
  Input_Sequence sequence;
  std::vector<Move> moves = game.get_move_sequence();
  int num_moves = moves.size();
  int i = 0, last_special_move = 0;
  int rank_from, file_from, rank_to, file_to;
  std::string curr_move;

  turn = 0;
  init_board();

  // Play ahead until last POS_LENGTH moves remain
  while (i + POS_LENGTH < num_moves) {
    curr_move = moves[i].move_notation;

    play_move(moves[i].move_notation);
    if (is_special(curr_move))
      last_special_move = i;

    // Save board history
    board_history.insert(board_history.begin(), board);
    if (board_history.size() > 7)
      board_history.pop_back();

    i++;
  }

  // Capture board states for last POS_LENGTH moves
  for (int j = 0; j < POS_LENGTH && i < num_moves; i++, j++) {
    curr_move = moves[i].move_notation;

    play_move(curr_move, rank_from, file_from, rank_to, file_to);
    if (is_special(curr_move))
      last_special_move = i;

    std::array<std::array<std::array<int, INPUT_TOKEN_LENGTH>, board_size>,
               board_size>
        board_tokens = {};

    // Compute values for each board position (x, y)
    for (int x = 0; x < board_size; x++) {
      for (int y = 0; y < board_size; y++) {
        std::array<int, INPUT_TOKEN_LENGTH> token = {};

        // 8 one-hot vectors for the last 8 board positions
        for (int k = 0;
             k < std::min(8, static_cast<int>(board_history.size()) + 1); k++) {
          std::array<int, NUM_FIGURES * 2> piece_vector =
              get_input_token(x, y, k);

          std::copy(piece_vector.begin(), piece_vector.end(),
                    token.begin() + k * 12);
        }

        // En passant information
        token[96] = is_en_passant_target(rank_to, file_to);

        // Save current turn
        int reset_turn = turn;

        // Define castling parameters for both sides
        std::array<std::tuple<int, int, int, int, std::string>, 4>
            castling_moves = {
                std::make_tuple(0, 4, 0, 6, "O-O"),   // White kingside
                std::make_tuple(0, 4, 0, 2, "O-O-O"), // White queenside
                std::make_tuple(7, 4, 7, 6, "O-O"),   // Black kingside
                std::make_tuple(7, 4, 7, 2, "O-O-O")  // Black queenside
            };

        // Loop through castling moves
        for (int i = 0; i < 4; i++) {
          turn = (i < 2) ? 0 : 1; // White for first two, Black for last two
          auto [rank_from, file_from, rank_to, file_to, notation] =
              castling_moves[i];
          token[97 + i] =
              handle_castling(notation, rank_from, file_from, rank_to, file_to);
        }

        // Reset turn
        turn = reset_turn;

        // Moves since last special move
        token[101] = static_cast<float>(i - last_special_move) / 100.0f;

        // Repetition history for last 8 moves
        for (int k = 0; k < std::min(8, static_cast<int>(board_history.size()));
             k++) {
          token[102 + k] = (board_history[k][x][y] == board[x][y]);
        }

        board_tokens[x][y] = token;
      }
    }

    sequence.board_tokens.push_back(board_tokens);

    // Save board history
    board_history.insert(board_history.begin(), board);
    if (board_history.size() > 7)
      board_history.pop_back();
  }

  return sequence;
}

/**
 * Returns the one-hot encoded snapshot of the board.
 */
std::array<std::array<std::array<int, NUM_FIGURES * 2>, board_size>, board_size>
Chess_Board::get_board_snapshot() {
  std::array<std::array<std::array<int, NUM_FIGURES * 2>, board_size>,
             board_size>
      snapshot = {};

  for (int i = 0; i < board_size; i++) {
    for (int j = 0; j < board_size; j++) {
      snapshot[i][j] = get_input_token(i, j, 0);
    }
  }

  return snapshot;
}

/**
 * Returns a one-hot encoding vector for the piece at position (i, j) k moves
 * ago, where k == 0: Current board.
 */
std::array<int, NUM_FIGURES * 2> Chess_Board::get_input_token(int i, int j,
                                                              int k) {
  std::array<int, NUM_FIGURES * 2> B = {0};
  int history_len = board_history.size();

  std::array<std::array<Figure, board_size>, board_size> ref_board =
      k == 0 ? board : board_history[history_len - k - 1];

  if (!ref_board[i][j].empty) {
    int type = ref_board[i][j].type;
    int color = ref_board[i][j].color; // 0 for white, 1 for black
    B[type + (color * NUM_FIGURES)] = 1;
  }
  return B;
}

/**
 * Returns true if referenced move is a capture, a pawn move, or a castling
 * move.
 */
bool Chess_Board::is_special(const std::string &move) {
  // Check for castling moves
  if (move == "O-O" || move == "O-O-O") {
    return true;
  }

  // Check for captures (contains 'x')
  if (move.find('x') != std::string::npos) {
    return true;
  }

  // Check for pawn move (no uppercase letters)
  for (char c : move) {
    if (std::isupper(c)) {
      return false; // Contains uppercase -> Not a pawn move
    }
  }

  return true; // If no uppercase letters, it's a pawn move
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
  // for (Move move : move_sequence)
  // play_move(move.move_notation);

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
  bool is_capture = move.find('x') != std::string::npos;

  if (is_capture) { // Capture case
    file_to = file_to_int(move[2]);
    rank_to = 8 - (move[3] - '0');

    // Handle en passant
    if (is_en_passant_target(rank_to, file_to)) {
      rank_from = turn == WHITE ? rank_to + 1 : rank_to - 1;
      file_from = file_to_int(move[0]);
      return !king_into_check(rank_from, file_from, rank_to, file_to);
    }

    // Handle regular capture
    rank_from = turn == WHITE ? rank_to + 1 : rank_to - 1;
    file_from = file_to_int(move[0]);
    Figure target = board[rank_to][file_to];
    return target.color == !turn && target.type != 0 &&
           !king_into_check(rank_from, file_from, rank_to, file_to);
  } else { // Non-capture case
    file_to = file_to_int(move[0]);
    rank_to = 8 - (move[1] - '0');

    // Handle double-square move (only on starting rank)
    if ((turn == WHITE && rank_to == 4) || (turn == BLACK && rank_to == 3)) {
      int start_rank = turn == WHITE ? 6 : 1;
      int mid_rank = turn == WHITE ? 5 : 2;
      if (board[rank_to][file_to].empty && board[mid_rank][file_to].empty) {
        rank_from = start_rank;
        file_from = file_to;
        return !king_into_check(rank_from, file_from, rank_to, file_to);
      }
    }

    // Handle single-square move
    int direction = turn == WHITE ? 1 : -1;
    rank_from = rank_to + direction;
    file_from = file_to;

    if (board[rank_to][file_to].empty) {
      // Handle promotion
      if (rank_to == 0 || rank_to == 7) {
        char promotion_piece =
            move.length() > 2 ? move[3] : 'Q'; // Default to queen
        promote_piece(promotion_piece, rank_from, file_from, rank_to, file_to);
        return !king_into_check(rank_from, file_from, rank_to, file_to);
      }
      return !king_into_check(rank_from, file_from, rank_to, file_to);
    }

    return 0; // Illegal move
  }
}

/**
 * Updates the en passant target square after a pawn moves two squares forward.
 */
void Chess_Board::update_en_passant_target(int rank, int file) {
  en_passant_target[0] = rank;
  en_passant_target[1] = file;
}

/**
 * Resets the en passant target square (e.g., after the next move).
 */
void Chess_Board::reset_en_passant_target() {
  en_passant_target[0] = -1;
  en_passant_target[1] = -1;
}

/**
 * Returns whether the given square is the en passant target.
 */
int Chess_Board::is_en_passant_target(int rank, int file) {
  return en_passant_target[0] == rank && en_passant_target[1] == file;
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
  bool is_capture = move.find('x') != std::string::npos;

  if (is_capture) { // Capture case
    file_to = file_to_int(move[2]);
    rank_to = 8 - (move[3] - '0');
  } else { // Non-capture case
    file_to = file_to_int(move[1]);
    rank_to = 8 - (move[2] - '0');
  }

  // Handle unambiguous moves (e.g., "Rae1")
  if (move.length() == 4 && is_file(move[1])) { // File disambiguation
    file_from = file_to_int(move[1]);
    return check_straight_line(ROOK_TYPE, rank_from, file_from, rank_to,
                               file_to);
  } else if (move.length() == 4 && isdigit(move[1])) { // Rank disambiguation
    rank_from = 8 - (move[1] - '0');
    return check_straight_line(ROOK_TYPE, rank_from, file_from, rank_to,
                               file_to);
  } else if (move.length() == 5) { // Both file and rank disambiguation
    file_from = file_to_int(move[1]);
    rank_from = 8 - (move[2] - '0');
    return check_straight_line(ROOK_TYPE, rank_from, file_from, rank_to,
                               file_to);
  }

  // Handle unambiguous moves (e.g., "Re1")
  if (move.length() == 3) {
    return check_straight_line(ROOK_TYPE, rank_from, file_from, rank_to,
                               file_to);
  }

  return 0; // Illegal move
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
  bool is_capture = move.find('x') != std::string::npos;

  if (is_capture) { // Capture case
    file_to = file_to_int(move[2]);
    rank_to = 8 - (move[3] - '0');
  } else { // Non-capture case
    file_to = file_to_int(move[1]);
    rank_to = 8 - (move[2] - '0');
  }

  // Handle unambiguous moves (e.g., "Qae1" or "Q1e1")
  if (move.length() == 4) {
    if (is_file(move[1])) { // File disambiguation (e.g., "Qae1")
      file_from = file_to_int(move[1]);
      return find_queen_on_file(file_from, rank_from, rank_to, file_to);
    } else if (isdigit(move[1])) { // Rank disambiguation (e.g., "Q1e1")
      rank_from = 8 - (move[1] - '0');
      return find_queen_on_rank(rank_from, file_from, rank_to, file_to);
    }
  } else if (move.length() ==
             5) { // Both file and rank disambiguation (e.g., "Qa1e1")
    file_from = file_to_int(move[1]);
    rank_from = 8 - (move[2] - '0');
    return check_queen_path(rank_from, file_from, rank_to, file_to);
  }

  // Handle unambiguous moves (e.g., "Qe1")
  if (move.length() == 3) {
    return find_queen(rank_from, file_from, rank_to, file_to);
  }

  return 0; // Illegal move
}

/**
 * Searches for the queen on the specified file that can legally move to
 * (rank_to, file_to).
 */
int Chess_Board::find_queen_on_file(int file_from, int &rank_from, int rank_to,
                                    int file_to) {
  // Check both directions along the file (up and down)
  for (int dir : {1, -1}) {
    int r = rank_to + dir;
    while (r >= 0 && r < 8) {
      Figure curr = board[r][file_from];
      if (curr.type != 0) {
        if (curr.color == turn && curr.type == QUEEN_TYPE) {
          rank_from = r;
          return check_queen_path(rank_from, file_from, rank_to, file_to);
        }
        break; // Path is blocked
      }
      r += dir;
    }
  }
  return 0; // No queen found on the specified file
}

/**
 * Searches for the queen on the specified rank that can legally move to
 * (rank_to, file_to).
 */
int Chess_Board::find_queen_on_rank(int rank_from, int &file_from, int rank_to,
                                    int file_to) {
  // Check both directions along the rank (left and right)
  for (int dir : {1, -1}) {
    int f = file_to + dir;
    while (f >= 0 && f < 8) {
      Figure curr = board[rank_from][f];
      if (curr.type != 0) {
        if (curr.color == turn && curr.type == QUEEN_TYPE) {
          file_from = f;
          return check_queen_path(rank_from, file_from, rank_to, file_to);
        }
        break; // Path is blocked
      }
      f += dir;
    }
  }
  return 0; // No queen found on the specified rank
}

/**
 * Searches for the queen that can legally move to (rank_to, file_to).
 */
int Chess_Board::find_queen(int &rank_from, int &file_from, int rank_to,
                            int file_to) {
  // Check all 8 possible directions from the target square
  const int directions[8][2] = {
      {1, 0}, {-1, 0}, {0, 1},  {0, -1}, // Straight directions
      {1, 1}, {1, -1}, {-1, 1}, {-1, -1} // Diagonal directions
  };

  for (const auto &dir : directions) {
    int r = rank_to + dir[0];
    int f = file_to + dir[1];

    while (r >= 0 && r < 8 && f >= 0 && f < 8) {
      Figure curr = board[r][f];
      if (curr.type != 0) {
        if (curr.color == turn && curr.type == QUEEN_TYPE) {
          rank_from = r;
          file_from = f;
          return check_queen_path(rank_from, file_from, rank_to, file_to);
        }
        break; // Path is blocked
      }
      r += dir[0];
      f += dir[1];
    }
  }

  return 0; // No queen found
}

/**
 * Checks if a queen can move from (rank_from, file_from) to (rank_to, file_to)
 * without putting the king in check.
 */
int Chess_Board::check_queen_path(int &rank_from, int &file_from, int rank_to,
                                  int file_to) {
  // Check if the move is along a straight line or diagonal
  int rank_diff = abs(rank_to - rank_from);
  int file_diff = abs(file_to - file_from);
  if (rank_diff != file_diff && rank_from != rank_to && file_from != file_to) {
    return 0; // Not a straight line or diagonal
  }

  // Determine the direction of movement
  int rank_dir = (rank_to > rank_from) ? 1 : (rank_to < rank_from) ? -1 : 0;
  int file_dir = (file_to > file_from) ? 1 : (file_to < file_from) ? -1 : 0;

  // Traverse the path to ensure it's clear
  int r = rank_from + rank_dir;
  int f = file_from + file_dir;
  while (r != rank_to || f != file_to) {
    Figure curr = board[r][f];
    if (curr.type != 0) {
      return 0; // Path is blocked
    }
    r += rank_dir;
    f += file_dir;
  }

  // Check the target square
  Figure target = board[rank_to][file_to];
  if (target.color == turn && target.type != 0) {
    return 0; // Cannot capture own piece
  }

  // Ensure the move doesn't put the king in check
  return !king_into_check(rank_from, file_from, rank_to, file_to);
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
  bool is_capture = move.find('x') != std::string::npos;

  if (is_capture) { // Capture case
    file_to = file_to_int(move[2]);
    rank_to = 8 - (move[3] - '0');
  } else { // Non-capture case
    file_to = file_to_int(move[1]);
    rank_to = 8 - (move[2] - '0');
  }

  // Determine the king's current position
  rank_from = king_pos[turn][0];
  file_from = king_pos[turn][1];

  // Check if the move is within one square in any direction
  int rank_diff = abs(rank_to - rank_from);
  int file_diff = abs(file_to - file_from);
  if (rank_diff > 1 || file_diff > 1) {
    return 0; // Illegal move (king can only move one square)
  }

  // Check if the target square is occupied by a friendly piece
  Figure target = board[rank_to][file_to];
  if (target.color == turn && target.type != 0) {
    return 0; // Illegal move (cannot capture own piece)
  }

  // Check if the move puts the king in check
  if (king_into_check(rank_from, file_from, rank_to, file_to)) {
    return 0; // Illegal move (king cannot move into check)
  }

  return 1; // Legal move
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
 * Returns 1 if figure moves own king into check (1 = no check, 0 = check, -1
 * = position empty, -2 = other figure is blocking path). Returns
 * non-negative integer if and only if bishop figure has been found
 * @param input  int type of figure
 * @param output int
 * @param output rank of figure before move
 * @param output file of figure before move
 * @param output rank of figure after move
 * @param output file of figure after move
 */
int Chess_Board::figure_move_is_legal(int figure_type, int &rank_from,
                                      int &file_from, int &rank_to,
                                      int &file_to) {
  Figure curr = board[rank_from][file_from];
  if (curr.color == turn && curr.type == figure_type)
    return !king_into_check(rank_from, file_from, rank_to, file_to);
  else if (curr.empty)
    return -1;

  return -2;
}

/**
 * Checks if a rook can move along a straight line from (rank_from, file_from)
 * to (rank_to, file_to) without putting the king in check.
 */
int Chess_Board::check_straight_line(int figure_type, int &rank_from,
                                     int &file_from, int rank_to, int file_to) {
  // Check if the move is along a straight line
  if (rank_from != rank_to && file_from != file_to) {
    return 0; // Not a straight line
  }

  // Determine the direction of movement
  int rank_dir = (rank_to > rank_from) ? 1 : (rank_to < rank_from) ? -1 : 0;
  int file_dir = (file_to > file_from) ? 1 : (file_to < file_from) ? -1 : 0;

  // Traverse the path to ensure it's clear
  int r = rank_from + rank_dir;
  int f = file_from + file_dir;
  while (r != rank_to || f != file_to) {
    Figure curr = board[r][f];
    if (curr.type != 0) {
      return 0; // Path is blocked
    }
    r += rank_dir;
    f += file_dir;
  }

  // Check the target square
  Figure target = board[rank_to][file_to];
  if (target.color == turn && target.type != 0) {
    return 0; // Cannot capture own piece
  }

  // Ensure the move doesn't put the king in check
  return !king_into_check(rank_from, file_from, rank_to, file_to);
}

int Chess_Board::check_diagonals(int figure_type, int &rank_from,
                                 int &file_from, int &rank_to, int &file_to) {
  // diagonal traversal, bottom-left
  for (int i = rank_to + 1, j = file_to - 1; i < 8 && j >= 0; i++, j--) {
    rank_from = i;
    file_from = j;
    int ret_move_legal = figure_move_is_legal(figure_type, rank_from, file_from,
                                              rank_to, file_to);
    if (ret_move_legal == -1)
      continue;
    else if (ret_move_legal < -1)
      break;
    else
      return ret_move_legal;
  }

  // diagonal traversal, bottom-right
  for (int i = rank_to + 1, j = file_to + 1; i < 8 && j < 8; i++, j++) {
    rank_from = i;
    file_from = j;
    int ret_move_legal = figure_move_is_legal(figure_type, rank_from, file_from,
                                              rank_to, file_to);
    if (ret_move_legal == -1)
      continue;
    else if (ret_move_legal < -1)
      break;
    else
      return ret_move_legal;
  }

  // diagonal traversal, top-left
  for (int i = rank_to - 1, j = file_to - 1; i >= 0 && j >= 0; i--, j--) {
    rank_from = i;
    file_from = j;
    int ret_move_legal = figure_move_is_legal(figure_type, rank_from, file_from,
                                              rank_to, file_to);
    if (ret_move_legal == -1)
      continue;
    else if (ret_move_legal < -1)
      break;
    else
      return ret_move_legal;
  }

  // diagonal traversal, top-right
  for (int i = rank_to - 1, j = file_to + 1; i >= 0 && j < 8; i--, j++) {
    rank_from = i;
    file_from = j;
    int ret_move_legal = figure_move_is_legal(figure_type, rank_from, file_from,
                                              rank_to, file_to);
    if (ret_move_legal == -1)
      continue;
    else if (ret_move_legal < -1)
      break;
    else
      return ret_move_legal;
  }

  return -1;
}