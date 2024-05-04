#ifndef PIECE_H
#define PIECE_H
#include <iostream>

 struct coordinats {
    int og_col,og_row,want_col,want_row;
};

typedef struct coordinats coord;

 enum  class Color{
    White,
    Black,
    Empty
};
enum class Type{
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King,
    Empty
};
std::ostream& operator<<(std::ostream& os,const Color& color);
class Piece{
    public:
    Piece(Color s_color,Type s_type);
    Piece(const Piece&);
    void promote();
    char getSymbol()const;
    Type getType()const;
    int return_playCount()const{return play_count;};
    Color getColor()const;
    void moveInc(){play_count++;};
    bool returnAlive()const{return is_alive;};
    void kill();
    Piece& operator=(const Piece& other);
    private:
    char symbol;
    Type p_type;
    Color p_color;
    int play_count;
    bool is_alive;
};

#endif
