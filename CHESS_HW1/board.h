#ifndef BOARD_H
#define BOARD_H
#include <iostream>
#include <vector>
#include <regex>
#include "piece.h"
using namespace std;
std::ostream& operator<<(std::ostream& os,const Type& type);
class Board{
    public:
    Board();
    bool save_board();
    void load_Board();
    coord takeInput();
    void gameLoop();
    bool returnTurn()const;
    void end_gamePrint(int);
    bool write_to_bestScore(int);
    void calculate_points();
    bool is_get_checked(const vector<vector<Piece> >&,coord king_coordinates);
    bool is_move_valid_ai(const coord);
    coord king_finder(Color king_color);
    bool is_moveValid(const coord);
    void move_piece(const coord);
    bool is_checkMate();
    bool regex_control(string);
    string suggestion();
    bool is_square_threaten(const bool&,const Color&);
    bool king_move_checker(coord king);
    bool can_block_threat(coord king_threat);
    int endBoardPoints();
    bool is_CastlingValid(const Piece& king,const Piece& rook,const coord& piece_coords,const bool& queen_side_castling);

    void print_board();
    string moveToString();

    double calculate_Minus(const vector< vector<Piece> >& vec,Color);

    private:

    bool is_whiteChecked,is_blackChecked;
    bool create_vector_init(vector<vector<Piece> >& init_vector);

    

    int game_point;
    vector<vector<Piece> > board_vec;
    
    bool isWhitesTurn;
    static string score_filename;
    static string save_file_name;
    int white_sc,black_sc;
};

#endif
