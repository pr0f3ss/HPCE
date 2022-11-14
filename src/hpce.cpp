#include "hpce.h"
#include <iostream>
#include <cctype>
#include <string>
#include <cstdlib>

/**
 * Default constructor. Initializes board and variables and prints the board.
 */
ChessBoard::ChessBoard(){
    turn = 0;

    initBoard();
    printBoard();
    playMove("e4");
    printBoard();
    playMove("e5");
    printBoard();
    playMove("Nf3");
    printBoard();
    playMove("Nc6");
    printBoard();
    
}

/**
 * Default deconstructor.
 */
ChessBoard::~ChessBoard(){

}

/**
 * Plays a move and updates the board if the move is legal. Returns whether a move is legal (1 = legal).
 * @param input move notation
 */
int ChessBoard::playMove(std::string move){
    int rank_from = 0, file_from = 0, rank_to = 0, file_to = 0;

    int isLegal = isLegalMove(move, rank_from, file_from, rank_to, file_to);
    if(isLegal){
        // board from-position gets empty and to-position gets updated 
        board[rank_to][file_to] = board[rank_from][file_from];
        board[rank_from][file_from] = empty;

        turn = !turn; 
        return 1;
    }
    return 0;
}

/**
 * Prints the board to stdout for debugging purposes.
 */
int ChessBoard::printBoard(){
    for(int i=0; i<board_size; i++){
        std::cout << "|";
        for(int j=0; j<board_size; j++){
            std::cout << board[i][j].symbol << "|";
        }
        std::cout << "\n________________\n";
    }
}

/**
 * Initializes the board.
 */
void ChessBoard::initBoard(){

    for(int j=0; j<board_size; j++){
        board[1][j] = b_pawn;
        board[6][j] = w_pawn; 
    }

    for(int i=2; i<6; i++){
        for(int j=0; j<board_size; j++){
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
}

/**
 * Returns the current game score (in standard notation).
 */
int ChessBoard::getScore(){
    int score = 0;

    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            Figure curr = board[i][j];
            score += curr.color ? -curr.value : curr.value; 
        }
    }
}

/**
 * Returns whether a move is legal (1 = legal). If the move is legal, we pass the relevant rank and file positions for the move to be played.
 * Note that there are no guarantees for the rank and file positions if the move is illegal.
 * @param input move notation
 * @param output rank of figure before move 
 * @param output file of figure before me
 * @param output rank of figure after move
 * @param output file of figure after move
 */
int ChessBoard::isLegalMove(std::string move, int &rank_from, int &file_from, int &rank_to, int &file_to){
    // If first letter is uppercase, it is a not a pawn
    if(isupper(move[0])){
        switch(move[0]) {
            case 'N': return handleKnight(move, rank_from, file_from, rank_to, file_to);
            case 'B': return handleBishop(move, rank_from, file_from, rank_to, file_to);
            case 'R': return handleRook(move, rank_from, file_from, rank_to, file_to);
            case 'Q': return handleQueen(move, rank_from, file_from, rank_to, file_to);
            case 'K': return handleKing(move, rank_from, file_from, rank_to, file_to);
            default:  return 0;
        }
    }else{
        return handlePawn(move, rank_from, file_from, rank_to, file_to);
    }
    return 0;
}

/**
 * Returns whether a pawn move is legal (1 = legal). Same notations apply as in isLegalMove().
 * @param input move notation
 * @param output rank of figure before move 
 * @param output file of figure before me
 * @param output rank of figure after move
 * @param output file of figure after move
 */
int ChessBoard::handlePawn(std::string move, int &rank_from, int &file_from, int &rank_to, int &file_to){
    if(move[1] == 'x'){
        // todo: implement
        return 1;
    }else{ // pawn moves normally
        int file = file_to_int(move[0]);
        int rank = 8 - (move[1] - '0');

        // Special case: Moving forward two squares
        if(rank == 4 && turn == 0){ // for white
            if(board[4][file].empty==1 && board[5][file].empty==1){
                Figure pos = board[6][file];
                rank_from = 6;
                file_from = file;
                rank_to = rank;
                file_to = file;
                return pos.type == 0 && pos.color == 0 && kingIntoCheck(rank_from, file_from, rank_to, file_to);
            }
            return 0;
        }else if(rank == 3 && turn == 1){ // for black
            if(board[3][file].empty==1 && board[2][file].empty==1){
                Figure pos = board[1][file];
                rank_from = 1;
                file_from = file;
                rank_to = rank;
                file_to = file;
                return pos.type == 0 && pos.color == 1 && kingIntoCheck(rank_from, file_from, rank_to, file_to);
            }
            return 0;
        }else{ // Pawn moves forward one square
            // Special case: Promotion
            if(rank == 0 || rank == 7){
                char promotion_piece = move[2];
                // todo: implement
            }
            if(board[rank][file].empty==1){
                int direction = turn ? -1 : 1;
                rank_from = rank+direction;
                file_from = file;
                rank_to = rank;
                file_to = file;
                Figure pos = board[rank_from][file_from];
                return pos.type == 0 && kingIntoCheck(rank_from, file_from, rank_to, file_to);
            }
        }
        return 0;
    }
}

/**
 * Returns whether a knight move is legal (1 = legal). Same notations apply as in isLegalMove().
 * @param input move notation
 * @param output rank of figure before move 
 * @param output file of figure before me
 * @param output rank of figure after move
 * @param output file of figure after move
 */
int ChessBoard::handleKnight(std::string move, int &rank_from, int &file_from, int &rank_to, int &file_to){
     if(move[1] == 'x'){
        // todo: implement
        return 1;
    }else{
        if(move.length()==3){ // unambiguous move

            int file = file_to_int(move[1]);
            int rank = 8 - (move[2] - '0');

            rank_to = rank;
            file_to = file;

            // iterate over possible knight positions (copied lots of code from kingIntoCheck)
            // bottom cases
            int r, f;
            r = rank_to+2;
            f = file_to;

            if(r < 8){
                if(f + 1 < 8){
                    Figure curr = board[r][f+1];
                    if(curr.color == turn && curr.type == KNIGHT_TYPE){
                        rank_from = r;
                        file_from = f+1;
                        return kingIntoCheck(rank_from, file_from, rank_to, file_to);
                    }
                }
                if(f - 1 >= 0){
                    Figure curr = board[r][f-1];
                    if(curr.color == turn && curr.type == KNIGHT_TYPE){
                        rank_from = r;
                        file_from = f-1;
                        return kingIntoCheck(rank_from, file_from, rank_to, file_to);
                    }
                }
            }

            // top cases
            r = rank_to-2;
            f = file_to;
            if(r >= 0){
                if(f + 1 < 8){
                    Figure curr = board[r][f+1];
                    if(curr.color == turn && curr.type == KNIGHT_TYPE){
                        rank_from = r;
                        file_from = f+1;
                        return kingIntoCheck(rank_from, file_from, rank_to, file_to);
                    }
                }
                if(f - 1 >= 0){
                    Figure curr = board[r][f-1];
                    if(curr.color == turn && curr.type == KNIGHT_TYPE){
                        rank_from = r;
                        file_from = f-1;
                        return kingIntoCheck(rank_from, file_from, rank_to, file_to);
                    }
                }
            }

            // left cases
            r = rank_to;
            f = file_to-2;
            if(f >= 0){
                if(r + 1 < 8){
                    Figure curr = board[r+1][f];
                    if(curr.color == turn && curr.type == KNIGHT_TYPE){
                        rank_from = r+1;
                        file_from = f;
                        return kingIntoCheck(rank_from, file_from, rank_to, file_to);
                    }
                }
                if(r - 1 >= 0){
                    Figure curr = board[r-1][f];
                    if(curr.color == turn && curr.type == KNIGHT_TYPE){
                        rank_from = r-1;
                        file_from = f;
                        return kingIntoCheck(rank_from, file_from, rank_to, file_to);
                    }
                }
            }

            // right cases
            r = rank_to;
            f = file_to+2;
            if(f < 8){
                if(r + 1 < 8){
                    Figure curr = board[r+1][f];
                    if(curr.color == turn && curr.type == KNIGHT_TYPE){
                        rank_from = r+1;
                        file_from = f;
                        return kingIntoCheck(rank_from, file_from, rank_to, file_to);
                    }
                }
                if(r - 1 >= 0){
                    Figure curr = board[r-1][f];
                    if(curr.color == turn && curr.type == KNIGHT_TYPE){
                        rank_from = r-1;
                        file_from = f;
                        return kingIntoCheck(rank_from, file_from, rank_to, file_to);
                    }
                }
            }

            return 0;

        }else{ //ambiguous move

        }
    }
}


/**
 * Returns whether a bishop move is legal (1 = legal). Same notations apply as in isLegalMove().
 * @param input move notation
 * @param output rank of figure before move 
 * @param output file of figure before me
 * @param output rank of figure after move
 * @param output file of figure after move
 */
int ChessBoard::handleBishop(std::string move, int &rank_from, int &file_from, int &rank_to, int &file_to){
    return 0;
}

/**
 * Returns whether a rook move is legal (1 = legal). Same notations apply as in isLegalMove().
 * @param input move notation
 * @param output rank of figure before move 
 * @param output file of figure before me
 * @param output rank of figure after move
 * @param output file of figure after move
 */
int ChessBoard::handleRook(std::string move, int &rank_from, int &file_from, int &rank_to, int &file_to){
    return 0;
}

/**
 * Returns whether a queen move is legal (1 = legal). Same notations apply as in isLegalMove().
 * @param input move notation
 * @param output rank of figure before move 
 * @param output file of figure before me
 * @param output rank of figure after move
 * @param output file of figure after move
 */
int ChessBoard::handleQueen(std::string move, int &rank_from, int &file_from, int &rank_to, int &file_to){
    return 0;
}

/**
 * Returns whether a king move is legal (1 = legal). Same notations apply as in isLegalMove().
 * @param input move notation
 * @param output rank of figure before move 
 * @param output file of figure before me
 * @param output rank of figure after move
 * @param output file of figure after move
 */
int ChessBoard::handleKing(std::string move, int &rank_from, int &file_from, int &rank_to, int &file_to){
    return 0;
}



/**
 * Returns whether the king will be in check after the referenced figure has moved.
 * @param input rank of figure before move 
 * @param input file of figure before me
 * @param input rank of figure after move
 * @param input file of figure after move
 */
int ChessBoard::kingIntoCheck(int rank_from, int file_from, int rank_to, int file_to){

    // Essentially, we have to see whether the king is being targeted by a capable enemy piece on the same rank, file or diagonal.
    // We do this by iterating the fields in the same rank, file or diagonal and stop when we find either an enemy piece or a friendly one.
    // Additionally, we check enemy knights at possible locations

    // First, we update the board after the move and then do the iteration procedure.
    int isInCheck = 0;

    // Update the board to after-move state
    Figure fig_from = board[rank_from][file_from];
    Figure fig_to = board[rank_to][file_to];
    board[rank_to][file_to] = fig_from;
    board[rank_from][file_from] = empty;

    int king_rank = king_pos[turn][0];
    int king_file = king_pos[turn][1];
    
    // file traversal, left of king
    for(int i=king_file-1; i>=0; i--){
        Figure curr = board[king_rank][i];
        if(curr.color == turn) break;    
        if(curr.color == !turn && (curr.type == ROOK_TYPE || curr.type == QUEEN_TYPE)){
            isInCheck = 1;
            goto cleanup;
        }
    }

    // file traversal, right of king
    for(int i=king_file+1; i<8; i++){
        Figure curr = board[king_rank][i];
        if(curr.color == turn) break;
        if(curr.color == !turn && (curr.type == ROOK_TYPE || curr.type == QUEEN_TYPE)){
            isInCheck = 1;
            goto cleanup;
        }
    }

    // rank traversal, bottom of king
    for(int i=king_rank+1; i<8; i++){
        Figure curr = board[i][king_file];
        if(curr.color == turn) break;    
        if(curr.color == !turn && (curr.type == ROOK_TYPE || curr.type == QUEEN_TYPE)){
            isInCheck = 1;
            goto cleanup;
        }
    }

    // rank traversal, top of king
    for(int i=king_rank-1; i>=0; i--){
        Figure curr = board[i][king_file];
        if(curr.color == turn) break;    
        if(curr.color == !turn && (curr.type == ROOK_TYPE || curr.type == QUEEN_TYPE)){
            isInCheck = 1;
            goto cleanup;
        }
    }

    // diagonal traversal, bottom-left of king

    // Special case: check if white pawn attacks black king
    if(turn == BLACK && king_rank+1 < 8 && king_file-1 >= 0){
        Figure attackPawn = board[king_rank+1][king_file-1];
        if(attackPawn.type == PAWN_TYPE && attackPawn.color == WHITE){
            isInCheck = 1;
            goto cleanup;
        }
    }

    for(int i=king_rank+1; i<8; i++){
        for(int j=king_file-1; j>=0; j--){
            Figure curr = board[i][j];
            if(curr.color == turn) break;    
            if(curr.color == !turn && (curr.type == BISHOP_TYPE || curr.type == QUEEN_TYPE)){
                isInCheck = 1;
                goto cleanup;
            }
        }
    }

    // diagonal traversal, bottom-right of king

    // Special case: check if white pawn attacks black king
    if(turn == BLACK && king_rank+1 < 8 && king_file+1 < 8){
        Figure attackPawn = board[king_rank+1][king_file+1];
        if(attackPawn.type == PAWN_TYPE && attackPawn.color == WHITE){
            isInCheck = 1;
            goto cleanup;
        }
    }

    for(int i=king_rank+1; i<8; i++){
        for(int j=king_file+1; j<8; j++){
            Figure curr = board[i][j];
            if(curr.color == turn) break;    
            if(curr.color == !turn && (curr.type == BISHOP_TYPE || curr.type == QUEEN_TYPE)){
                isInCheck = 1;
                goto cleanup;
            }
        }
    }

    // diagonal traversal, top-left of king

    // Special case: check if black pawn attacks white king
    if(turn == WHITE && king_rank-1 >= 0 && king_file-1 >= 0){
        Figure attackPawn = board[king_rank-1][king_file-1];
        if(attackPawn.type == PAWN_TYPE && attackPawn.color == BLACK){
            isInCheck = 1;
            goto cleanup;
        }
    }

    for(int i=king_rank-1; i>=0; i--){
        for(int j=king_file-1; j>=0; j--){
            Figure curr = board[i][j];
            if(curr.color == turn) break;    
            if(curr.color == !turn && (curr.type == BISHOP_TYPE || curr.type == QUEEN_TYPE)){
                isInCheck = 1;
                goto cleanup;
            }
        }
    }

    // diagonal traversal, top-right of king

    // Special case: check if black pawn attacks white king
    if(turn == WHITE && king_rank-1 >= 0 && king_file+1 < 8){
        Figure attackPawn = board[king_rank-1][king_file+1];
        if(attackPawn.type == PAWN_TYPE && attackPawn.color == BLACK){
            isInCheck = 1;
            goto cleanup;
        }
    }

    for(int i=king_rank-1; i>=0; i--){
        for(int j=king_file+1; j<8; j++){
            Figure curr = board[i][j];
            if(curr.color == turn) break;    
            if(curr.color == !turn && (curr.type == BISHOP_TYPE || curr.type == QUEEN_TYPE)){
                isInCheck = 1;
                goto cleanup;
            }
        }
    }


    // Check enemy knight locations

    // bottom cases
    int r, f;
    r = king_rank+2;
    f = king_file;

    if(r < 8){
        if(f + 1 < 8){
            Figure curr = board[r][f+1];
            if(curr.color == !turn && curr.type == KNIGHT_TYPE){
                isInCheck = 1;
                goto cleanup;
            }
        }
        if(f - 1 >= 0){
            Figure curr = board[r][f-1];
            if(curr.color == !turn && curr.type == KNIGHT_TYPE){
                isInCheck = 1;
                goto cleanup;
            }
        }
    }

    // top cases
    r = king_rank-2;
    f = king_file;
    if(r >= 0){
        if(f + 1 < 8){
            Figure curr = board[r][f+1];
            if(curr.color == !turn && curr.type == KNIGHT_TYPE){
                isInCheck = 1;
                goto cleanup;
            }
        }
        if(f - 1 >= 0){
            Figure curr = board[r][f-1];
            if(curr.color == !turn && curr.type == KNIGHT_TYPE){
                isInCheck = 1;
                goto cleanup;
            }
        }
    }

    // left cases
    r = king_rank;
    f = king_file-2;
    if(f >= 0){
        if(r + 1 < 8){
            Figure curr = board[r+1][f];
            if(curr.color == !turn && curr.type == KNIGHT_TYPE){
                isInCheck = 1;
                goto cleanup;
            }
        }
        if(r - 1 >= 0){
            Figure curr = board[r-1][f];
            if(curr.color == !turn && curr.type == KNIGHT_TYPE){
                isInCheck = 1;
                goto cleanup;
            }
        }
    }

    // right cases
    r = king_rank;
    f = king_file+2;
    if(f < 8){
        if(r + 1 < 8){
            Figure curr = board[r+1][f];
            if(curr.color == !turn && curr.type == KNIGHT_TYPE){
                isInCheck = 1;
                goto cleanup;
            }
        }
        if(r - 1 >= 0){
            Figure curr = board[r-1][f];
            if(curr.color == !turn && curr.type == KNIGHT_TYPE){
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



int ChessBoard::file_to_int(char file){
    switch(file) {
        case 'a': return 0;
        case 'b': return 1;
        case 'c': return 2;
        case 'd': return 3;
        case 'e': return 4;
        case 'f': return 5;
        case 'g': return 6;
        case 'h': return 7;
        default:  exit(1);
    }
}

