#include "../include/hpce.hpp"
#include "../include/pgn_reader.hpp"
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <string>

/**
 * Default constructor. Initializes board and variagbles and prints the board.
 */
ChessBoard::ChessBoard() {
  turn = 0;

  initBoard();
  playMove("e4");
  playMove("e5");

  playMove("Nf3");
  playMove("Nc6");

  playMove("Bb5");
  playMove("a6");

  playMove("O-O");
  playMove("d6");

  playMove("a3");
  playMove("Bd7");

  playMove("a4");
  playMove("Qe7");

  playMove("a5");
  playMove("O-O-O");

  printBoard();
}

/**
 * Default deconstructor.
 */
ChessBoard::~ChessBoard() {}

/**
 * Tries playing a move and updates the board if the move is legal. Returns
 * whether a move is legal (1 = legal).
 * @param input move notation
 */
int ChessBoard::playMove(std::string move) {
  int rank_from = 0, file_from = 0, rank_to = 0, file_to = 0;

  int isLegal = isLegalMove(move, rank_from, file_from, rank_to, file_to);
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
int ChessBoard::printBoard() {
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
void ChessBoard::initBoard() {
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
int ChessBoard::getScore() {
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
int ChessBoard::isLegalGame(PGN_Chess_Game game) {
  turn = 0;

  initBoard();

  std::vector<Move> move_sequence = game.getMoveSequence();

  // TODO: Implement error logic
  for (Move move : move_sequence)
    playMove(move.move_notation);

  return 0;
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
int ChessBoard::isLegalMove(std::string move, int &rank_from, int &file_from,
                            int &rank_to, int &file_to) {
  // If first letter is uppercase, it is a not a pawn
  if (isupper(move[0])) {
    switch (move[0]) {
    case 'N':
      return handleKnight(move, rank_from, file_from, rank_to, file_to);
    case 'B':
      return handleBishop(move, rank_from, file_from, rank_to, file_to);
    case 'R':
      return handleRook(move, rank_from, file_from, rank_to, file_to);
    case 'Q':
      return handleQueen(move, rank_from, file_from, rank_to, file_to);
    case 'K':
      return handleKing(move, rank_from, file_from, rank_to, file_to);
    case 'O':
      return handleCastling(move, rank_from, file_from, rank_to, file_to);
    default:
      return 0;
    }
  } else {
    return handlePawn(move, rank_from, file_from, rank_to, file_to);
  }
  return 0;
}

/**
 * Returns whether a pawn move is legal (1 = legal). Same notations apply as in
 * isLegalMove().
 * @param input move notation
 * @param output rank of figure before move
 * @param output file of figure before me
 * @param output rank of figure after move
 * @param output file of figure after move
 */
int ChessBoard::handlePawn(std::string move, int &rank_from, int &file_from,
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
               !kingIntoCheck(rank_from, file_from, rank_to, file_to);
      }
    } else if (rank == 3 && turn == 1) { // for black
      if (board[3][file].empty == 1 && board[2][file].empty == 1) {
        Figure pos = board[1][file];
        rank_from = 1;
        file_from = file;
        rank_to = rank;
        file_to = file;
        return pos.type == 0 && pos.color == 1 &&
               !kingIntoCheck(rank_from, file_from, rank_to, file_to);
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
             !kingIntoCheck(rank_from, file_from, rank_to, file_to);
    }

    return 0;
  }
}

/**
 * Returns whether a knight move is legal (1 = legal). Same notations apply as
 * in isLegalMove().
 * @param input move notation
 * @param output rank of figure before move
 * @param output file of figure before me
 * @param output rank of figure after move
 * @param output file of figure after move
 */
int ChessBoard::retrieveKnightLocation(std::string move, int &rank_from,
                                       int &file_from, int &rank_to,
                                       int &file_to) {
  // iterate over possible knight positions (copied lots of code from
  // kingIntoCheck) bottom cases
  int r, f;
  r = rank_to + 2;
  f = file_to;

  if (r < 8) {
    if (f + 1 < 8) {
      Figure curr = board[r][f + 1];
      if (curr.color == turn && curr.type == KNIGHT_TYPE) {
        rank_from = r;
        file_from = f + 1;
        return !kingIntoCheck(rank_from, file_from, rank_to, file_to);
      }
    }
    if (f - 1 >= 0) {
      Figure curr = board[r][f - 1];
      if (curr.color == turn && curr.type == KNIGHT_TYPE) {
        rank_from = r;
        file_from = f - 1;
        return !kingIntoCheck(rank_from, file_from, rank_to, file_to);
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
        return !kingIntoCheck(rank_from, file_from, rank_to, file_to);
      }
    }
    if (f - 1 >= 0) {
      Figure curr = board[r][f - 1];
      if (curr.color == turn && curr.type == KNIGHT_TYPE) {
        rank_from = r;
        file_from = f - 1;
        return !kingIntoCheck(rank_from, file_from, rank_to, file_to);
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
        return !kingIntoCheck(rank_from, file_from, rank_to, file_to);
      }
    }
    if (r - 1 >= 0) {
      Figure curr = board[r - 1][f];
      if (curr.color == turn && curr.type == KNIGHT_TYPE) {
        rank_from = r - 1;
        file_from = f;
        return !kingIntoCheck(rank_from, file_from, rank_to, file_to);
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
        return !kingIntoCheck(rank_from, file_from, rank_to, file_to);
      }
    }
    if (r - 1 >= 0) {
      Figure curr = board[r - 1][f];
      if (curr.color == turn && curr.type == KNIGHT_TYPE) {
        rank_from = r - 1;
        file_from = f;
        return !kingIntoCheck(rank_from, file_from, rank_to, file_to);
      }
    }
  }

  return 0;
}

/**
 * Returns whether a knight move is legal (1 = legal). Same notations apply as
 * in isLegalMove().
 * @param input move notation
 * @param output rank of figure before move
 * @param output file of figure before me
 * @param output rank of figure after move
 * @param output file of figure after move
 */
int ChessBoard::handleKnight(std::string move, int &rank_from, int &file_from,
                             int &rank_to, int &file_to) {
  if (move.find('x') != std::string::npos) { // capture case

    if (move.length() == 4) { // unambiguous move
      int file = file_to_int(move[2]);
      int rank = 8 - (move[3] - '0');

      rank_to = rank;
      file_to = file;
    }

    return 1;
  } else {                    // no capture case
    if (move.length() == 3) { // unambiguous move

      int file = file_to_int(move[1]);
      int rank = 8 - (move[2] - '0');

      rank_to = rank;
      file_to = file;
      return 1;
    } else { // ambiguous move
      // todo: implement
      return 1;
    }
  }
}

/**
 * Returns whether a bishop move is legal (1 = legal). Same notations apply as
 * in isLegalMove().
 * @param input move notation
 * @param output rank of figure before move
 * @param output file of figure before me
 * @param output rank of figure after move
 * @param output file of figure after move
 */
int ChessBoard::handleBishop(std::string move, int &rank_from, int &file_from,
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

      // diagonal traversal, bottom-left
      for (int i = rank_to + 1, j = file_to - 1; i < 8 && j >= 0; i++, j--) {
        rank_from = i;
        file_from = j;
        int ret_move_legal =
            is_bishop_move_into_check(rank_from, file_from, rank_to, file_to);
        if (ret_move_legal < 0)
          break;
        else
          return ret_move_legal;
      }

      // diagonal traversal, bottom-right

      for (int i = rank_to + 1, j = file_to + 1; i < 8 && j < 8; i++, j++) {
        rank_from = i;
        file_from = j;
        int ret_move_legal =
            is_bishop_move_into_check(rank_from, file_from, rank_to, file_to);
        if (ret_move_legal < 0)
          break;
        else
          return ret_move_legal;
      }

      // diagonal traversal, top-left
      for (int i = rank_to - 1, j = file_to - 1; i >= 0 && j >= 0; i--, j--) {
        rank_from = i;
        file_from = j;
        int ret_move_legal =
            is_bishop_move_into_check(rank_from, file_from, rank_to, file_to);
        if (ret_move_legal < 0)
          break;
        else
          return ret_move_legal;
      }

      // diagonal traversal, top-right
      for (int i = rank_to - 1, j = file_to + 1; i >= 0 && j < 8; i--, j++) {
        rank_from = i;
        file_from = j;
        int ret_move_legal =
            is_bishop_move_into_check(rank_from, file_from, rank_to, file_to);
        if (ret_move_legal < 0)
          break;
        else
          return ret_move_legal;
      }
      return 0;
    } else { // disambiguous case, no capture
      // Moving piece is uniquely identified by specifying the piece's letter,
      // followed by (in descending order of preference):
      //
      // 1. the file of departure (if they differ)
      // 2. the rank of departure (if the files are the same but the ranks
      // differ)
      //
      // If neither file nor rank alone is sufficient to identify the piece
      // (such as when three or more pieces of the same type can move to the
      // same square), then both are specified (double disambiguation).

      if (move.length() == 4) { // file or rank disambiguouation
        int uses_file = is_file(move[1]);
        int file = file_to_int(move[2]);
        int rank = 8 - (move[3] - '0');

        rank_to = rank;
        file_to = file;

        if (uses_file) {
          file_from = file_to_int(move[1]);

          int dist = file_from - file_to;

          rank_from = rank_to + dist;
          Figure curr = board[rank_from][file_from];
          if (curr.color == turn && curr.type == BISHOP_TYPE) {
            return !kingIntoCheck(rank_from, file_from, rank_to, file_to);
          }

          rank_from = rank_to - dist;
          curr = board[rank_from][file_from];
          if (curr.color == turn && curr.type == BISHOP_TYPE) {
            return !kingIntoCheck(rank_from, file_from, rank_to, file_to);
          }

          return 0;

        } else {
          rank_from = 8 - (move[2] - '0');

          int dist = rank_from - rank_to;

          file_from = file_to + dist;
          Figure curr = board[rank_from][file_from];
          if (curr.color == turn && curr.type == BISHOP_TYPE) {
            return !kingIntoCheck(rank_from, file_from, rank_to, file_to);
          }

          file_from = file_to - dist;
          curr = board[rank_from][file_from];
          if (curr.color == turn && curr.type == BISHOP_TYPE) {
            return !kingIntoCheck(rank_from, file_from, rank_to, file_to);
          }
        }

      } else { // both from rank and file locations are in notation
        file_from = file_to_int(move[1]);
        rank_from = 8 - (move[2] - '0');

        file_to = file_to_int(move[3]);
        rank_to = 8 - (move[4] - '0');

        Figure curr = board[rank_from][file_from];
        if (curr.color == turn && curr.type == BISHOP_TYPE) {
          return !kingIntoCheck(rank_from, file_from, rank_to, file_to);
        }
      }

      return 0;
    }
  }
}

/**
 * Returns whether a rook move is legal (1 = legal). Same notations apply as in
 * isLegalMove().
 * @param input move notation
 * @param output rank of figure before move
 * @param output file of figure before me
 * @param output rank of figure after move
 * @param output file of figure after move
 */
int ChessBoard::handleRook(std::string move, int &rank_from, int &file_from,
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

      // copied mostly from kingIntoCheck()

      // file traversal, left of destination square
      for (int i = file_to - 1; i >= 0; i--) {
        Figure curr = board[rank_to][i];
        if (curr.color == turn && curr.type == ROOK_TYPE) {
          rank_from = rank_to;
          file_from = i;
          return !kingIntoCheck(rank_from, file_from, rank_to, file_to);
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
          return !kingIntoCheck(rank_from, file_from, rank_to, file_to);
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
          return !kingIntoCheck(rank_from, file_from, rank_to, file_to);
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
          return !kingIntoCheck(rank_from, file_from, rank_to, file_to);
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
 * isLegalMove().
 * @param input move notation
 * @param output rank of figure before move
 * @param output file of figure before me
 * @param output rank of figure after move
 * @param output file of figure after move
 */
int ChessBoard::handleQueen(std::string move, int &rank_from, int &file_from,
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

      // copied mostly from handleRook() and handleBishop() (as their moves are
      // identical)

      // file traversal, left of destination square
      for (int i = file_to - 1; i >= 0; i--) {
        Figure curr = board[rank_to][i];
        if (curr.color == turn && curr.type == QUEEN_TYPE) {
          rank_from = rank_to;
          file_from = i;
          return !kingIntoCheck(rank_from, file_from, rank_to, file_to);
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
          return !kingIntoCheck(rank_from, file_from, rank_to, file_to);
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
          return !kingIntoCheck(rank_from, file_from, rank_to, file_to);
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
          return !kingIntoCheck(rank_from, file_from, rank_to, file_to);
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
          return !kingIntoCheck(rank_from, file_from, rank_to, file_to);
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
          return !kingIntoCheck(rank_from, file_from, rank_to, file_to);
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
          return !kingIntoCheck(rank_from, file_from, rank_to, file_to);
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
          return !kingIntoCheck(rank_from, file_from, rank_to, file_to);
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
 * isLegalMove(). Note that castling is handled in a separate function:
 * handleCastling()
 * @param input move notation
 * @param output rank of figure before move
 * @param output file of figure before me
 * @param output rank of figure after move
 * @param output file of figure after move
 */
int ChessBoard::handleKing(std::string move, int &rank_from, int &file_from,
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
 * in isLegalMove().
 * @param input move notation
 * @param output rank of figure before move
 * @param output file of figure before me
 * @param output rank of figure after move
 * @param output file of figure after move
 */
int ChessBoard::handleCastling(std::string move, int &rank_from, int &file_from,
                               int &rank_to, int &file_to) {
  if (move == "O-O") { // king side castle
    if (turn == WHITE) {
      rank_from = 7;
      file_from = 4;
      rank_to = 7;
      file_to = 6;
      Figure l = board[7][5];
      Figure r = board[7][6];
      if (!l.empty || !r.empty)
        return 0;
    } else {
      rank_from = 0;
      file_from = 4;
      rank_to = 0;
      file_to = 6;
      Figure l = board[0][5];
      Figure r = board[0][6];
      if (!l.empty || !r.empty)
        return 0;
    }
    // we have to check all squares that the king would move to are not in check
    return !king_moved[turn] && !rook_moved[turn][1] &&
           !kingIntoCheck(rank_from, file_from, rank_to, file_to) &&
           !kingIntoCheck(rank_from, file_from, rank_to, file_to - 1);
  } else if (move == "O-O-O") { // queen side castle
    if (turn == WHITE) {
      rank_from = 7;
      file_from = 4;
      rank_to = 7;
      file_to = 2;
      Figure l = board[7][1];
      Figure m = board[7][2];
      Figure r = board[7][3];
      if (!l.empty || !m.empty || !r.empty)
        return 0;
    } else {
      rank_from = 0;
      file_from = 4;
      rank_to = 0;
      file_to = 2;
      Figure l = board[0][1];
      Figure m = board[0][2];
      Figure r = board[0][3];
      if (!l.empty || !m.empty || !r.empty)
        return 0;
    }
    // we have to check all squares that the king would move to are not in check
    return !king_moved[turn] && !rook_moved[turn][0] &&
           !kingIntoCheck(rank_from, file_from, rank_to, file_to) &&
           !kingIntoCheck(rank_from, file_from, rank_to, file_to + 1);
  }

  return 0;
}

/**
 * Returns whether the king will be in check after the referenced figure has
 * moved.
 * @param input rank of figure before move
 * @param input file of figure before me
 * @param input rank of figure after move
 * @param input file of figure after move
 */
int ChessBoard::kingIntoCheck(int rank_from, int file_from, int rank_to,
                              int file_to) {

  // Essentially, we have to see whether the king is being targeted by a capable
  // enemy piece on the same rank, file or diagonal. We do this by iterating the
  // fields in the same rank, file or diagonal and stop when we find either an
  // enemy piece or a friendly one. Additionally, we check enemy knights at
  // possible locations

  // First, we update the board after the move and then do the iteration
  // procedure.
  int isInCheck = 0;

  // Update the board to after-move state
  Figure fig_from = board[rank_from][file_from];
  Figure fig_to = board[rank_to][file_to];
  board[rank_to][file_to] = fig_from;
  board[rank_from][file_from] = empty;

  int king_rank = king_pos[turn][0];
  int king_file = king_pos[turn][1];

  // file traversal, left of king
  for (int i = king_file - 1; i >= 0; i--) {
    Figure curr = board[king_rank][i];
    if (curr.color == turn)
      break;
    if (curr.color == !turn &&
        (curr.type == ROOK_TYPE || curr.type == QUEEN_TYPE)) {
      isInCheck = 1;
      goto cleanup;
    }
  }

  // file traversal, right of king
  for (int i = king_file + 1; i < 8; i++) {
    Figure curr = board[king_rank][i];
    if (curr.color == turn)
      break;
    if (curr.color == !turn &&
        (curr.type == ROOK_TYPE || curr.type == QUEEN_TYPE)) {
      isInCheck = 1;
      goto cleanup;
    }
  }

  // rank traversal, bottom of king
  for (int i = king_rank + 1; i < 8; i++) {
    Figure curr = board[i][king_file];
    if (curr.color == turn)
      break;
    if (curr.color == !turn &&
        (curr.type == ROOK_TYPE || curr.type == QUEEN_TYPE)) {
      isInCheck = 1;
      goto cleanup;
    }
  }

  // rank traversal, top of king
  for (int i = king_rank - 1; i >= 0; i--) {
    Figure curr = board[i][king_file];
    if (curr.color == turn)
      break;
    if (curr.color == !turn &&
        (curr.type == ROOK_TYPE || curr.type == QUEEN_TYPE)) {
      isInCheck = 1;
      goto cleanup;
    }
  }

  // diagonal traversal, bottom-left of king
  // Special case: check if white pawn attacks black king
  if (turn == BLACK && king_rank + 1 < 8 && king_file - 1 >= 0) {
    Figure attackPawn = board[king_rank + 1][king_file - 1];
    if (attackPawn.type == PAWN_TYPE && attackPawn.color == WHITE) {
      isInCheck = 1;
      goto cleanup;
    }
  }

  for (int i = king_rank + 1, j = king_file - 1; i < 8 && j >= 0; i++, j--) {
    Figure curr = board[i][j];
    if (curr.color == turn)
      break;
    if (curr.color == !turn &&
        (curr.type == BISHOP_TYPE || curr.type == QUEEN_TYPE)) {
      isInCheck = 1;
      goto cleanup;
    }
  }

  // diagonal traversal, bottom-right of king
  // Special case: check if white pawn attacks black king
  if (turn == BLACK && king_rank + 1 < 8 && king_file + 1 < 8) {
    Figure attackPawn = board[king_rank + 1][king_file + 1];
    if (attackPawn.type == PAWN_TYPE && attackPawn.color == WHITE) {
      isInCheck = 1;
      goto cleanup;
    }
  }

  for (int i = king_rank + 1, j = king_file + 1; i < 8 && j < 8; i++, j++) {
    Figure curr = board[i][j];
    if (curr.color == turn)
      break;
    if (curr.color == !turn &&
        (curr.type == BISHOP_TYPE || curr.type == QUEEN_TYPE)) {
      isInCheck = 1;
      goto cleanup;
    }
  }

  // diagonal traversal, top-left of king
  // Special case: check if black pawn attacks white king
  if (turn == WHITE && king_rank - 1 >= 0 && king_file - 1 >= 0) {
    Figure attackPawn = board[king_rank - 1][king_file - 1];
    if (attackPawn.type == PAWN_TYPE && attackPawn.color == BLACK) {
      isInCheck = 1;
      goto cleanup;
    }
  }

  for (int i = king_rank - 1, j = king_file - 1; i >= 0 && j >= 0; i--, j--) {
    Figure curr = board[i][j];
    if (curr.color == turn)
      break;
    if (curr.color == !turn &&
        (curr.type == BISHOP_TYPE || curr.type == QUEEN_TYPE)) {
      isInCheck = 1;
      goto cleanup;
    }
  }

  // diagonal traversal, top-right of king
  // Special case: check if black pawn attacks white king
  if (turn == WHITE && king_rank - 1 >= 0 && king_file + 1 < 8) {
    Figure attackPawn = board[king_rank - 1][king_file + 1];
    if (attackPawn.type == PAWN_TYPE && attackPawn.color == BLACK) {
      isInCheck = 1;
      goto cleanup;
    }
  }

  for (int i = king_rank - 1, j = king_file + 1; i >= 0 && j < 8; i--, j++) {
    Figure curr = board[i][j];
    if (curr.color == turn)
      break;
    if (curr.color == !turn &&
        (curr.type == BISHOP_TYPE || curr.type == QUEEN_TYPE)) {
      isInCheck = 1;
      goto cleanup;
    }
  }

  // Check enemy knight locations

  // bottom cases
  int r, f;
  r = king_rank + 2;
  f = king_file;

  if (r < 8) {
    if (f + 1 < 8) {
      Figure curr = board[r][f + 1];
      if (curr.color == !turn && curr.type == KNIGHT_TYPE) {
        isInCheck = 1;
        goto cleanup;
      }
    }
    if (f - 1 >= 0) {
      Figure curr = board[r][f - 1];
      if (curr.color == !turn && curr.type == KNIGHT_TYPE) {
        isInCheck = 1;
        goto cleanup;
      }
    }
  }

  // top cases
  r = king_rank - 2;
  f = king_file;
  if (r >= 0) {
    if (f + 1 < 8) {
      Figure curr = board[r][f + 1];
      if (curr.color == !turn && curr.type == KNIGHT_TYPE) {
        isInCheck = 1;
        goto cleanup;
      }
    }
    if (f - 1 >= 0) {
      Figure curr = board[r][f - 1];
      if (curr.color == !turn && curr.type == KNIGHT_TYPE) {
        isInCheck = 1;
        goto cleanup;
      }
    }
  }

  // left cases
  r = king_rank;
  f = king_file - 2;
  if (f >= 0) {
    if (r + 1 < 8) {
      Figure curr = board[r + 1][f];
      if (curr.color == !turn && curr.type == KNIGHT_TYPE) {
        isInCheck = 1;
        goto cleanup;
      }
    }
    if (r - 1 >= 0) {
      Figure curr = board[r - 1][f];
      if (curr.color == !turn && curr.type == KNIGHT_TYPE) {
        isInCheck = 1;
        goto cleanup;
      }
    }
  }

  // right cases
  r = king_rank;
  f = king_file + 2;
  if (f < 8) {
    if (r + 1 < 8) {
      Figure curr = board[r + 1][f];
      if (curr.color == !turn && curr.type == KNIGHT_TYPE) {
        isInCheck = 1;
        goto cleanup;
      }
    }
    if (r - 1 >= 0) {
      Figure curr = board[r - 1][f];
      if (curr.color == !turn && curr.type == KNIGHT_TYPE) {
        isInCheck = 1;
        goto cleanup;
      }
    }
  }

// Clean up board to before-move state
cleanup:
  board[rank_to][file_to] = fig_to;
  board[rank_from][file_from] = fig_from;

  return isInCheck;
}

int ChessBoard::file_to_int(char file) {
  switch (file) {
  case 'a':
    return 0;
  case 'b':
    return 1;
  case 'c':
    return 2;
  case 'd':
    return 3;
  case 'e':
    return 4;
  case 'f':
    return 5;
  case 'g':
    return 6;
  case 'h':
    return 7;
  default:
    exit(1);
  }
}

/**
 * Returns whether the current character is referring to a file or rank (1 =
 * file).
 * @param input input character notation
 * @param output 1 if character notation is referring to file, else 0
 */
int ChessBoard::is_file(char char_notation) { return isdigit(char_notation); }

/**
 * Returns 1 if bishop moves own king into check (1 = check, 0 = no check, -1 =
 * figure not found, -2 = other figure is blocking path).
 * Returns non-negative integer if and only if bishop figure has been found
 * @param output int
 * @param output rank of figure before move
 * @param output file of figure before move
 * @param output rank of figure after move
 * @param output file of figure after move
 */
int ChessBoard::is_bishop_move_into_check(int &rank_from, int &file_from,
                                          int &rank_to, int &file_to) {
  Figure curr = board[rank_from][file_from];
  if (curr.color == turn) {
    if (curr.type == BISHOP_TYPE)
      return !kingIntoCheck(rank_from, file_from, rank_to, file_to);
    else
      return -2;
  } else if (curr.color != turn && curr.empty != 0) {
    return -2;
  }
  return -1;
}
