#ifndef _HPCE_H // include guard
#define _HPCE_H

#include <string>

#define board_size 8
#define players 2 
#define dimension 2

#define WHITE 0
#define BLACK 1

#define EMPTY_TYPE -1
#define PAWN_TYPE 0
#define BISHOP_TYPE 1
#define KNIGHT_TYPE 2
#define ROOK_TYPE 3
#define QUEEN_TYPE 4
#define KING_TYPE 5

struct Figure{
        int value;
        int type;
        int color;
        int empty;
        char symbol;
};

class ChessBoard{

    struct Figure w_pawn = {1,0,0,0,'p'};
    struct Figure b_pawn = {1,0,1,0,'P'};
    struct Figure w_bishop = {3,1,0,0,'b'};
    struct Figure b_bishop = {3,1,1,0,'B'};
    struct Figure w_knight = {3,2,0,0,'n'};
    struct Figure b_knight = {3,2,1,0,'N'};
    struct Figure w_rook = {5,3,0,0,'r'};
    struct Figure b_rook = {5,3,1,0,'R'};
    struct Figure w_queen = {9,4,0,0,'q'};
    struct Figure b_queen = {9,4,1,0,'Q'};
    struct Figure w_king = {0,5,0,0,'k'};
    struct Figure b_king = {0,5,1,0,'K'};
    struct Figure empty = {0,-1,0,1,' '};

    Figure board[board_size][board_size];
    int turn;
    int king_pos[players][dimension];

    public:
        ChessBoard(void);
        ~ChessBoard(void);
        int playMove(std::string move);
        int printBoard();
        int getScore();

    private:
        void initBoard();
        int isLegalMove(std::string move, int &rank_from, int &file_from, int &rank_to, int &file_to);
        
        int handlePawn(std::string move, int &rank_from, int &file_from, int &rank_to, int &file_to);
        int handleKnight(std::string move, int &rank_from, int &file_from, int &rank_to, int &file_to);
        int handleBishop(std::string move, int &rank_from, int &file_from, int &rank_to, int &file_to);
        int handleRook(std::string move, int &rank_from, int &file_from, int &rank_to, int &file_to);
        int handleQueen(std::string move, int &rank_from, int &file_from, int &rank_to, int &file_to);
        int handleKing(std::string move, int &rank_from, int &file_from, int &rank_to, int &file_to);

        int kingIntoCheck(int rank_from, int file_from, int rank_to, int file_to);
        int file_to_int(char file);
};

#endif