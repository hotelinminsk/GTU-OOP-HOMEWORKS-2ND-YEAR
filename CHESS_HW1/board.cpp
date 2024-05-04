#include "board.h"
#include "piece.h"
#include <sstream>
#include "fstream"
#include <ctime>

Board::Board(){
    if(create_vector_init(board_vec)){
        isWhitesTurn = true;
        white_sc=0,black_sc=0;
    }else{
        cerr<<"Memmory allocation error on Board constructor."<<endl;
        exit(-1);
    }
}


bool Board::create_vector_init(vector<vector<Piece> >& vec){
    for(int i=0;i<8;i++){
        vector<Piece> row;
        for(int j=0;j<8;j++){
            if(i==0 || i== 7){
                //if coordinats are row 1 or row 8
                if(j==0 || j== 7){
                if(i==7){
                    //White rook
                    Piece new_piece(Color::White,Type::Rook);
                    row.push_back(new_piece);
                }else{
                    //Black rook
                    Piece new_piece(Color::Black,Type::Rook);
                    row.push_back(new_piece);
                }
                }
                else if(j==1 || j== 6){
                    //Knight white
                    if(i==7){
                    Piece new_piece(Color::White,Type::Knight);
                    row.push_back(new_piece);
                    }else{
                        Piece new_piece(Color::Black,Type::Knight);
                        row.push_back(new_piece);
                    }

                }else if(j==2 || j== 5){
                    //Bisop
                    if(i==7){
                        Piece new_piece(Color::White,Type::Bishop);
                        row.push_back(new_piece);
                    }else{
                        Piece new_piece(Color::Black,Type::Bishop);
                        row.push_back(new_piece);
                    }
                }else if(j == 3 || j== 4){
                    if(j==3){
                        if(i==7){
                            Piece new_p(Color::White,Type::Queen);
                            row.push_back(new_p);
                        }else{
                           Piece new_p(Color::Black,Type::Queen);
                            row.push_back(new_p); 
                        }
                    }else{
                        if(i==7){
                            Piece new_p(Color::White,Type::King);
                            row.push_back(new_p);
                        }else{
                            Piece new_p(Color::Black,Type::King);
                            row.push_back(new_p);
                        }
                    }
                }
                

            } else if(i==1 || i== 6){
                if(i==6){
                    Piece new_p(Color::White,Type::Pawn);
                    row.push_back(new_p);
                }else{
                    Piece new_p(Color::Black,Type::Pawn);
                    row.push_back(new_p);
                }
            }else{
                //empty pieces
                Piece new_piece(Color::Empty,Type::Empty);
                row.push_back(new_piece);
            }
        }
        vec.push_back(row);
    }

    if(vec.size() == 8){
        return true;

    }else{
        return false;
    }
    
}

bool Board::regex_control(string input){
	char one = input[0];
	int two = input[1] - '0';
	char three = input[2];
	int four = input[3] - '0';

	if(one>= 'a' && one<= 'h'){
		if(two >= 1 && two<= 8){
			if(three >= 'a' && three <= 'h'){
				if(four>=1 && four <= 8){
						return true;
					}else{return false;}
			}else{return false;}			
		}else{return false;}
	}else{return false;}

//a2a4 97 to 122
}
coord Board::takeInput(){
    string temp;
    bool suggest_flag = false;
    bool error_flag = false;
    coord input_positions;
    do{
        if(suggest_flag){
            cout<<"Write the suggested move : "<<endl;
        }
        else if(!error_flag){
            cout<<"Give the move in chess notation(ex:e2e4,for suggestion write : suggestion,for closing the board write : close): ";

        }else{
            cout<<"\nPlease give the input again,use chess notation(ex:b2b4): ";
        }
        cin>>temp;
        transform(temp.begin(),temp.end(),temp.begin(),::tolower);
        suggest_flag = false;
        if(regex_control(temp)){
            int orginal_piece_col = temp[0] - 97;
            int orginal_piece_row = temp[1] - '0' -1;

            //check if given position occupys an piece
            if(board_vec[orginal_piece_row][orginal_piece_col].getType() != Type::Empty){
                //check if the turn is right for player
                if(isWhitesTurn && board_vec[orginal_piece_row][orginal_piece_col].getColor() == Color::White){
                    input_positions.og_col = orginal_piece_col;
                    input_positions.og_row = orginal_piece_row;
                    input_positions.want_col = temp[2] - 97;
                    input_positions.want_row = temp[3] - '0' - 1;
                    error_flag = false;
                }else if(!isWhitesTurn && board_vec[orginal_piece_row][orginal_piece_col].getColor() == Color::Black){
                    input_positions.og_col = orginal_piece_col;
                    input_positions.og_row = orginal_piece_row;
                    input_positions.want_col = temp[2] -97;
                    input_positions.want_row = temp[3] - '0' - 1;
                    
                    error_flag =false;
                }else{
                    cout<<"This is "<<((isWhitesTurn == true) ? "white's ": "black's ");
                    cout<<"turn but you tried to move an "<<((board_vec[orginal_piece_row][orginal_piece_col].getColor() == Color::White)? "white piece.":"black piece.");
                    error_flag = true;
                }
            }else{
                cerr<<"You cant move an empty square.\n";
                error_flag = true;
            }
        }else if(temp=="suggestion"){
            string suggest  = suggestion();
            cout<<"Suggest move is : "<<suggest<<endl;
            suggest_flag = true;
        }else if(temp=="save"){
            input_positions.og_col = -1,input_positions.og_row = 15;
            error_flag = false;
        }else if(temp=="close"){
            input_positions.og_col = -2,input_positions.og_row = 15;
            error_flag = false;
        }
        else{
            cerr<<"Invalid notation,try again.(for suggestion type : suggestion)";
            error_flag = true;
        }
    }while(error_flag || suggest_flag);
    return input_positions;
}

double Board::calculate_Minus(const vector< vector<Piece> >& vec,Color piece_color){
    Color enemy_color;
    Piece og_piece(Color::Empty,Type::Empty);
    Piece dest_piece(Color::Empty,Type::Empty);

    double minus_points = 0;

    if(piece_color == Color::White){
        enemy_color = Color::Black;
    }else{
        enemy_color = Color::White;
    }
    coord move_coord;
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            if(vec[i][j].getColor() == piece_color){
                og_piece = vec[i][j];
                for(int k= 0;k<8;k++){
                    for(int m=0;m<8;m++){
                        if(vec[k][m].getColor() == enemy_color){
                            dest_piece = vec[k][m];

                            move_coord.og_row = k,move_coord.og_col = m;
                            move_coord.want_row = i,move_coord.want_col = j;

                            if(is_move_valid_ai(move_coord)){
                                switch (og_piece.getType())
                                {
                                case Type::Pawn :
                                    minus_points -= 1/2.0;
                                    break;
                                case Type::Bishop:
                                case Type::Knight:
                                    minus_points -= 3/2.0;
                                    break;
                                case Type::Queen:
                                    minus_points -= 9/2,0;
                                    break;
                                case Type::Rook:
                                    minus_points -= 5/2,0;
                                    break;
                                case Type::King:
                                    minus_points -= -999;
                                    break;
                                default:
                                    break;
                                }
                            }
                        }
                    }
                }
            }

        }
    }


    return minus_points;
}

string Board::suggestion(){
    string best_move_string;
    stringstream ss_object;

    vector<int> equal_scores_int;
    vector<string> equal_scores_str;


    double best_score = -999;
    vector< vector< Piece> > copyVec = board_vec;
    Piece og_piece(Color::Empty,Type::Empty),dest_piece(Color::Empty,Type::Empty);
    coord move_coord;
    if(isWhitesTurn){
        for(int i=0;i<8;i++){
            for(int j=0;j<8;j++){
                if(board_vec[i][j].getColor() == Color::White){
                    for(int k=0;k<8;k++){
                        for(int m=0;m<8;m++){
                            if(board_vec[k][m].getColor() != Color::White){
                                move_coord.og_row =i,move_coord.og_col = j;
                                move_coord.want_row=k,move_coord.want_col = m;
                                if(is_move_valid_ai(move_coord)){
                                    double temp_score=0;
                                    
                                    og_piece = copyVec[move_coord.og_row][move_coord.og_col];
                                    dest_piece = copyVec[move_coord.want_row][move_coord.want_col];
                                    if(copyVec[move_coord.want_row][move_coord.want_col].getType() != Type::Empty){
                                        switch (dest_piece.getType())
                                        {
                                        case Type::Pawn :
                                            temp_score += 1;
                                            break;
                                        case Type::Bishop:
                                            temp_score += 3;
                                            break;
                                        case Type::Knight:
                                            temp_score += 3;
                                            break;
                                        case Type::Rook:
                                            temp_score += 5;
                                            break;
                                        case Type::Queen:
                                            temp_score += 9;
                                            break;
                                        case Type::King:
                                            temp_score += 9999;
                                            break;                                            
                                        default:
                                            break;
                                        }                                                   //
                                    }else{
                                        temp_score += 0;
                                    }
                                    copyVec[move_coord.want_row][move_coord.want_col] = og_piece;
                                    copyVec[move_coord.og_row][move_coord.og_col] = dest_piece;
                                    copyVec[move_coord.og_row][move_coord.og_col].kill();

                                    double minus = calculate_Minus(copyVec,og_piece.getColor());

                                    temp_score += minus;
                                    if(temp_score >= best_score){
                                       
                                        best_score = temp_score;
                                        // og row i og col j  destination row k dest col m olur
                                        //simdi bunları stringe çevirmeliyiz
                                        //col 0 1 2 3 4 5 6 7
                                        //    a b c d e f g h
                                        ss_object.str("");
                                        char col1 = j + 97;
                                        int row1 = i+1;
                                        char col2 = m + 97;
                                        int row2 = k+1; 
                                        ss_object << col1 << row1<< col2<< row2;
                                        best_move_string = ss_object.str();
                                        equal_scores_int.push_back(best_score);
                                        equal_scores_str.push_back(best_move_string);
                                    } 

                                }           
                            }
                        }
                    }
                }
            }
        }
    }else{
        
         for(int i=0;i<8;i++){
            for(int j=0;j<8;j++){
                if(board_vec[i][j].getColor() == Color::Black){
                    for(int k=0;k<8;k++){
                        for(int m=0;m<8;m++){
                            if(board_vec[k][m].getColor() != Color::Black){
                                move_coord.og_row =i,move_coord.og_col = j;
                                move_coord.want_row=k,move_coord.want_col = m;
                                if(is_move_valid_ai(move_coord)){
                                    double temp_score=0;
                                    og_piece = copyVec[move_coord.og_row][move_coord.og_col];
                                    dest_piece = copyVec[move_coord.want_row][move_coord.want_col];
                                    if(copyVec[move_coord.want_row][move_coord.want_col].getType() != Type::Empty){
                                        switch (dest_piece.getType())
                                        {
                                        case Type::Pawn :
                                            temp_score += 1;
                                            break;
                                        case Type::Bishop:
                                            temp_score += 3;
                                            break;
                                        case Type::Knight:
                                            temp_score += 3;
                                            break;
                                        case Type::Rook:
                                            temp_score += 5;
                                            break;
                                        case Type::Queen:
                                            temp_score += 9;
                                            break;
                                        case Type::King:
                                            temp_score += 9999;
                                            break;                                            
                                        default:
                                            break;
                                        }
                                    }
                                    copyVec[move_coord.want_row][move_coord.want_col] = og_piece;
                                    copyVec[move_coord.og_row][move_coord.og_col] = dest_piece;
                                    copyVec[move_coord.og_row][move_coord.og_col].kill();

                                    double minus = calculate_Minus(copyVec,og_piece.getColor());

                                    temp_score += minus;
                                    if(temp_score > best_score){
                                        
                                        best_score = temp_score;
                                        // og row i og col j  destination row k dest col m olur
                                        //simdi bunları stringe çevirmeliyiz
                                        //col 0 1 2 3 4 5 6 7
                                        //    a b c d e f g h
                                        ss_object.str("");
                                        char col1 = j + 97;
                                        int row1 = i+1;
                                        char col2 = m + 97;
                                        int row2 = k+1; 
                                        ss_object << col1 << row1<< col2<< row2;
                                        best_move_string = ss_object.str();
                                        equal_scores_int.push_back(best_score);
                                        equal_scores_str.push_back(best_move_string);
                                    } 

                                }           
                            }
                        }
                    }
                }
            }
    }
    }

    if(!equal_scores_str.empty()){
        srand(time(0));
        int random_index = rand() % equal_scores_str.size();
        best_move_string = equal_scores_str[random_index];
    }

    return best_move_string;
}

coord Board::king_finder(Color king_color){
    coord king_coordinates;
    king_coordinates.og_col=-1,king_coordinates.og_row=-1;
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            if(board_vec[i][j].getColor() == king_color && board_vec[i][j].getType() == Type::King){
                king_coordinates.og_row = i, king_coordinates.og_col = j;
                break;
            }
        }
    }
    return king_coordinates;
}


bool Board::king_move_checker(coord king){
vector<vector<Piece> > copyVEC = board_vec;
Piece og_king = copyVEC[king.og_row][king.og_col];
Piece dest_piece = copyVEC[king.want_row][king.want_col];
copyVEC[king.want_row][king.want_col] = og_king;
copyVEC[king.og_row][king.og_col] = dest_piece;
if(dest_piece.returnAlive() == true){
    copyVEC[king.og_row][king.og_col].kill();
}

// new king positions on copyVEC is king.wantrow and king.wantcol
coord new_king_coords;
new_king_coords.og_row = king.want_row;
new_king_coords.og_col = king.want_col;

bool move_invalid = is_get_checked(copyVEC,new_king_coords);

return move_invalid;
}
bool Board::is_square_threaten(const bool& is_quuen_side,const Color& k_color){
    coord king_way;
    Color enemy_color;
    switch (k_color)
    {
    case Color::White:
        enemy_color = Color::Black;
        king_way.og_row = 0;
        king_way.og_col = 4;
        break;
    case Color::Black:
        enemy_color = Color::White;
        king_way.og_row  = 7;
        king_way.og_col = 4;
        break;
    default:
        break;
    }
    if(is_quuen_side){
        //then king will move -2 col and rook will move +2 col
        int col_step = -1;
        for(int i=0;i<8;i++){
            for(int j=0;j<8;j++){
                if(board_vec[i][j].getColor() == enemy_color){
                    int curr_col = king_way.og_col + col_step;
                    int cur_row = king_way.og_row;
                    coord move_coord;
                    move_coord.og_row = i,move_coord.og_col = j;
                    move_coord.want_col = curr_col,move_coord.want_row = cur_row;
                    if(is_move_valid_ai(move_coord)){
                        //there is atleast one move that threats kings way
                        cout<<"Can't play rok because king's way is threaten."<<endl;
                        return false;
                    }else{
                        move_coord.want_col += col_step;
                        if(is_move_valid_ai(move_coord)){
                            cout<<"Can't play rok because king's way is threaten."<<endl;
                            return false;
                        }else{


                        }
                    }
                }
            } 
        }

        return true;

    }else{
        int col_step = 1;
        for(int i=0;i<8;i++){
            for(int j=0;j<8;j++){
                if(board_vec[i][j].getColor() == enemy_color){
                    int curr_col = king_way.og_col + col_step;
                    int curr_row = king_way.og_row;
                    coord move_coord;
                    move_coord.og_row = i,move_coord.og_col = j;
                    move_coord.want_row = curr_row,move_coord.want_col = curr_col;
                    if(is_move_valid_ai(move_coord)){
                        cout<<"Can't play rok because king's way is threaten. "<<endl;
                        return false;
                    }else{
                        move_coord.want_col += col_step;
                        if(is_move_valid_ai(move_coord)){
                            cout<<"Can't play rok because king's way is threaten. "<<endl;
                            return false;
                        }
                        else{}
                    }
                }
            }
        }

        return true;
    }
    

}

bool Board::can_block_threat(coord king_threat){
// threat is threat eater want row and want col
//king is og col and og row
//look between them and check if any of our pieces can go these positions in valid moves
int row_diff = abs(king_threat.og_row-king_threat.want_row);
int col_diff = abs(king_threat.og_col-king_threat.want_col);
Color king_color = board_vec[king_threat.og_row][king_threat.og_col].getColor();
int row_step = ((king_threat.want_row - king_threat.og_row)>0 ? 1:(king_threat.want_row - king_threat.og_row)<0 ? -1:0);
int col_step = ((king_threat.want_col - king_threat.og_col)>0 ? 1:(king_threat.want_col - king_threat.og_col)<0 ? -1:0);
bool can_block = false;

for(int i=0;i<8;i++){
    for(int j=0;j<8;j++){
        if(board_vec[i][j].getColor() == king_color){
            //these are our pieces now we have to check for each step to any of these pieces
            //can go to that square
            int curr_row = king_threat.og_row + row_step;
            int curr_col = king_threat.og_col + col_step;
            while(curr_row != king_threat.want_row && curr_col != king_threat.want_col){
                // i and j our piece's coordinats
                //and we have to check for steps
                coord each_step;
                each_step.og_row = i,each_step.og_col = j;
                each_step.want_row = curr_row,each_step.want_col = curr_col;
                if(is_move_valid_ai(each_step) == true){
                    //bu durumda taşımız aradaki boşluğa gidebiliyor
                    can_block = true;
                    break;
                }else{
                    can_block = false;
                }

                curr_row += row_step;
                curr_col += col_step;
            }
        }
    }
}


return can_block;
}

bool Board::is_checkMate(){
    coordinats white_king = king_finder(Color::White);
    coordinats black_king = king_finder(Color ::Black);
    bool white_has_secureMove = false;
    bool black_has_secureMove = false;

    int offsets[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},            {0, 1},
        {1, -1}, {1, 0}, {1, 1}
    };
    //first check for white 
    if(isWhitesTurn){
    if(is_whiteChecked){
        
         for(int i=0;i<8;i++){
            int king_new_row = white_king.og_row + offsets[i][0];
            int king_new_col = white_king.og_col + offsets[i][1];
            if( (king_new_col > -1 && king_new_col < 8) && (king_new_row > -1 && king_new_row < 8)){
                if(board_vec[king_new_row][king_new_col].getColor() == Color::White ){
                //king's way blocked by an ally piece 
                white_has_secureMove = false;
                }else{
                    white_king.want_row = king_new_row;
                    white_king.want_col = king_new_col;
                    if(!king_move_checker(white_king)){
                        white_has_secureMove = true;
                        return false;
                        //in this case we have atleast one valid secure move for king.
                        break;
                    }else{
                        white_has_secureMove = false; // burada dur
                    }
                }
            }

        }

        coord king_threatCoord1;
        king_threatCoord1.want_row = white_king.og_row;
        king_threatCoord1.want_col = white_king.og_col; 

        if(!white_has_secureMove){
            for(int i=0;i<8;i++){
                for(int j=0;j<8;j++){
                    if(board_vec[i][j].getColor() == Color::Black){
                        king_threatCoord1.og_row = i,king_threatCoord1.og_col = j;
                        if(is_move_valid_ai(king_threatCoord1) == true){
                            //this move is valid then this piece is a threat
                            for(int k=0;k<8;k++){
                                for(int m=0;m<8;m++){
                                    if(board_vec[k][m].getColor() == Color::White){
                                        coord threat_eater_coord;
                                        threat_eater_coord.want_row = i,threat_eater_coord.want_col = j;
                                        threat_eater_coord.og_row = k,threat_eater_coord.og_col = m;
                                        if(is_move_valid_ai(threat_eater_coord) == true){
                                            return false;
                                        }

                                    }
                                }
                            }
                            coord last = king_finder(Color::Black);
                            last.want_row = i;
                            last.want_col = j;
                            if(can_block_threat(last) == true){
                                return false;
                            }else{
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    }else{
        if(is_blackChecked){
            for(int i=0;i<8;i++){
                int king_new_row = black_king.og_row + offsets[i][0];
                int king_new_col = black_king.og_col + offsets[i][1];
                if( (king_new_col > -1 && king_new_col < 8) && (king_new_row > -1 && king_new_row < 8)){
                black_king.want_row = king_new_row;
                black_king.want_col = king_new_col;
                if(board_vec[king_new_row][king_new_col].getColor() == Color::Black ){
                //king's way blocked by an ally piece 
                black_has_secureMove = false;
                }
                else if(!king_move_checker(black_king)){
                    black_has_secureMove = true;
                    return false;
                    //in this case we have atleast one valid secure move for king.
                    break;
                }else{
                    black_has_secureMove = false; // burada dur
                }
            }
            }
 
            coord king_threatCoord1;
            king_threatCoord1.want_row = black_king.og_row;
            king_threatCoord1.want_col = black_king.og_col; 


            if(!black_has_secureMove){
                for(int i=0;i<8;i++){
                    for(int j=0;j<8;j++){
                        if(board_vec[i][j].getColor() == Color::White){
                            king_threatCoord1.og_row = i,king_threatCoord1.og_col = j;
                            if(is_move_valid_ai(king_threatCoord1) == true){
                                // then this piece in boardvec i j is a threat!
                                //can any of our pieces eat this threat?
                                for(int k= 0;k<8;k++){
                                    for(int m=0;m<8;m++){
                                        if(board_vec[k][m].getColor() == Color::Black){
                                            coord threat_eater_coord;
                                            threat_eater_coord.want_row = i,threat_eater_coord.want_col = j;
                                            threat_eater_coord.og_row=k,threat_eater_coord.og_col=m;
                                            if(is_move_valid_ai(threat_eater_coord) == true){
                                                return false;
                                            }
                                        }
                                    }
                                }
                                coord last = king_finder(Color::Black);
                                last.want_row = i;
                                last.want_col = j;
                                if(can_block_threat(last) == true){
                                return false;
                                //this means at least one of our pieces can block.
                                }else{
                                return true;                }
                            }
                        }
                    }
                }
                return true;
            }
        }
    }

    return false;

};

int Board::endBoardPoints(){
    //FIRST FOR WHITE
    //WE HAVE WHITE POINTS AFTER GAME END
    int white_p_temp = white_sc;
    int black_p_temp = black_sc;
    //now we have to check for each piece to see if any of them is under threat

    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            if(board_vec[i][j].getColor() == Color::White){
                for(int k=0;k<8;k++){
                    for(int m=0;m<8;m++){
                        if(board_vec[k][m].getColor() == Color::Black){
                            coord threat_p;
                            threat_p.og_row = k,threat_p.og_col =m;
                            threat_p.want_row = i,threat_p.want_col = j;
                            if(is_move_valid_ai(threat_p) == true){
                                switch (board_vec[i][j].getType())
                                {
                                case Type::Pawn :
                                    white_p_temp -= 1/2;
                                    break;
                                case Type::Knight:
                                case Type::Bishop:
                                    white_p_temp -= 3/2;
                                    break;
                                case Type::Rook:
                                    white_p_temp -= 5/2;
                                    break;
                                case Type::Queen:
                                    white_p_temp -= 9/2;
                                    break;
                                default:
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    //last white score is white_p_temp;
    
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            if(board_vec[i][j].getColor() == Color::Black){
                for(int k=0;k<8;k++){
                    for(int m=0;m<8;m++){
                        if(board_vec[k][m].getColor() == Color::White){
                            coord threat_p;
                            threat_p.og_row = k,threat_p.og_col =m;
                            threat_p.want_row = i,threat_p.want_col = j;
                            if(is_move_valid_ai(threat_p) == true){
                                switch (board_vec[i][j].getType())
                                {
                                case Type::Pawn :
                                    black_p_temp -= 1/2;
                                    break;
                                case Type::Knight:
                                case Type::Bishop:
                                    black_p_temp -= 3/2;
                                    break;
                                case Type::Rook:
                                    black_p_temp -= 5/2;
                                    break;
                                case Type::Queen:
                                    black_p_temp -= 9/2;
                                    break;
                                default:
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    white_sc = white_p_temp,black_sc = black_p_temp;

    return (white_sc > black_sc ? white_sc: black_sc);
}
bool Board::is_move_valid_ai(const coord piece_coord){
    bool position_available = true;
    Color enemy_color,piece_color;
    Piece empty_piece(Color::Empty,Type::Empty);
    vector<vector<Piece> > copy_vec = board_vec;
    Piece og_piece(board_vec[piece_coord.og_row][piece_coord.og_col]);
    Piece dest_piece(board_vec[piece_coord.want_row][piece_coord.want_col]);

    if(og_piece.getColor() == Color::White){
        enemy_color = Color::Black;
        piece_color = Color::White;
    }else{
        enemy_color = Color::White;
        piece_color = Color::Black;
    }

    if(dest_piece.getColor() != Color::Empty && dest_piece.getColor() != enemy_color){
        return false;
    }

    if(og_piece.getType() == Type::Knight){
        int dx,dy;
        dx = abs(piece_coord.og_col - piece_coord.want_col);
        dy = abs(piece_coord.og_row - piece_coord.want_row);
        if( (dx == 2 && dy == 1) || (dx==1 && dy==2)){
           
            return true;
        }else{
            return false;
        }
    }
    else if(og_piece.getType() == Type::Rook){
        // Check if the move is either horizontal or vertical
        if (piece_coord.og_row != piece_coord.want_row && piece_coord.og_col != piece_coord.want_col) {
        return false;
        }

    int row_step = (piece_coord.og_row < piece_coord.want_row) ? 1 : (piece_coord.og_row > piece_coord.want_row) ? -1 : 0;
    int col_step = (piece_coord.og_col < piece_coord.want_col) ? 1 : (piece_coord.og_col > piece_coord.want_col) ? -1 : 0;

    int curr_row = piece_coord.og_row + row_step;
    int curr_col = piece_coord.og_col + col_step;

    while (curr_row != piece_coord.want_row || curr_col != piece_coord.want_col) {
        if (board_vec[curr_row][curr_col].getType() != Type::Empty) {
            return false;
        }
        curr_row += row_step;
        curr_col += col_step;
    }
    return true;
    }
    else if(og_piece.getType() == Type::Bishop){
        bool road_blocked = false;
        int diff_row = abs(piece_coord.og_row - piece_coord.want_row);
        int diff_col = abs(piece_coord.og_col - piece_coord.want_col);
        if(diff_row != diff_col){
            return false;
        }

        int row_step = ((piece_coord.og_row > piece_coord.want_row)? -1:1);
        int col_step = ((piece_coord.og_col > piece_coord.want_col)? -1:1);

        int curr_row = piece_coord.og_row + row_step;
        int curr_col = piece_coord.og_col + col_step;

        while(curr_row != piece_coord.want_row && curr_col != piece_coord.want_col){
            if(board_vec[curr_row][curr_col].getType() != Type::Empty){
                road_blocked = true;
                break;
            }
            curr_row += row_step;
            curr_col += col_step;
        }

        if(!road_blocked){
            return true;
        }else{
            return false;
        }
        
    }
    else if(og_piece.getType() == Type::Queen){
        int row_diff = abs(piece_coord.og_row - piece_coord.want_row);
        int col_diff = abs(piece_coord.og_col - piece_coord.want_col);
            
        if(col_diff != 0 && row_diff != 0 && row_diff != col_diff){
            //checks for rook movements and bishop movements together.
            return false;
        }

        int row_step = ((piece_coord.og_row > piece_coord.want_row)? -1: ((piece_coord.want_row > piece_coord.og_row) ? 1:0 ));
        int col_step = ((piece_coord.og_col > piece_coord.want_col)? -1:((piece_coord.want_col > piece_coord.og_col)? 1:0));

        int curr_row = piece_coord.og_row + row_step;
        int curr_col = piece_coord.og_col + col_step;

        while(curr_col != piece_coord.want_col || curr_row != piece_coord.want_row){
            if(board_vec[curr_row][curr_col].getType() != Type::Empty){
                return false;
            }
            curr_row += row_step;
            curr_col += col_step;
        }
        return true;

    }else if(og_piece.getType() == Type::King){
    int row_diff = abs(piece_coord.og_row - piece_coord.want_row);
    int col_diff = abs(piece_coord.og_col - piece_coord.want_col);

    if (row_diff > 1 || col_diff > 1 || (row_diff == 0 && col_diff == 0)) {
        return false;
    }


    return true;

    }else if(og_piece.getType() == Type::Pawn){
        int row_step =0,col_step =0;
        if(piece_color == Color::White){
            row_step = -1;
            if(piece_coord.want_row >= piece_coord.og_row){
                return false;
            }
        }else{
            row_step = 1;
            if(piece_coord.want_row <= piece_coord.og_row){
                return false;
            }
        }

        int row_diff = abs(piece_coord.og_row - piece_coord.want_row);
        int col_diff = abs(piece_coord.og_col - piece_coord.want_col);

        if(row_diff == 2 && col_diff == 0){

            if(og_piece.return_playCount() > 0){
                return false;
            }else{

                if(board_vec[piece_coord.og_row + row_step][piece_coord.og_col].getType() != Type::Empty){
                    return false;
                }else{
                    return true;
                }
            }
        }else if(col_diff == 1 && row_diff == 1){
            //can only move diagonally when there is an enemy 
            if(board_vec[piece_coord.want_row][piece_coord.want_col].getColor() == enemy_color){
                return true;
            }else{
                return false;
            }
        }else if(col_diff==0 && row_diff == 1){
            if(board_vec[piece_coord.og_row + row_step][piece_coord.og_col].getColor() == piece_color || board_vec[piece_coord.og_row + row_step][piece_coord.og_col].getColor() == enemy_color){
                return false;
            }else{
                return true;
            }
        }else{
            return false;
        }
    }else{
        return false;
    }
}
bool Board::is_get_checked(const vector<vector<Piece> >& copyvec,coord king_coord){
    bool check = false;
    Color enemy_color;
    if(copyvec[king_coord.og_row][king_coord.og_col].getColor() == Color::White){
        enemy_color = Color::Black;
    }else{
        enemy_color = Color::White;
    }

    coord coordinates_for_move;
    coordinates_for_move.want_row = king_coord.og_row;
    coordinates_for_move.want_col = king_coord.og_col;
    for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
        if (copyvec[i][j].getColor() == enemy_color) {
            coordinates_for_move.og_row = i;
            coordinates_for_move.og_col = j;
            if (is_move_valid_ai(coordinates_for_move)) {
                check = true;
                break;
            } else {
                check = false;
            }
        }
    }
}
    return check;
}
bool Board::is_CastlingValid(const Piece& king,const Piece& rook,const coord& piece_coords,const bool& queen_side_castling){
    bool road_blocked = false;
    if(king.return_playCount() != 0 && rook.return_playCount() != 0){
        cout<<"Cannot play after rok  king and rook moved already."<<endl;
        return false;
    }else{
        //we know that rook and king not played 
        //now we have to check if there is an piece between them
        int col_step = (piece_coords.want_col>piece_coords.og_col ? 1:-1);
        int curr_col = piece_coords.og_col + col_step;
        while(curr_col != piece_coords.want_col){
            if(board_vec[piece_coords.og_row][curr_col].getType() != Type::Empty){
                road_blocked = true;
                break;
            }
            curr_col += col_step;
        }
    }

    if(!road_blocked){
       if(is_square_threaten(queen_side_castling,king.getColor())){
        return false;
       }else{
        return true;
       }
    }else{
        cout<<"Cant rok when there is pieces in the way!"<<endl;
        return false;
    }
}

bool Board::is_moveValid(const coord piece_coord){
    bool position_available = true;
    bool queen_side_castling = false;
    Color enemy_color,piece_color;
    Piece empty_piece(Color::Empty,Type::Empty);
    vector<vector<Piece> > copy_vec = board_vec;
    Piece og_piece(board_vec[piece_coord.og_row][piece_coord.og_col]);
    Piece dest_piece(board_vec[piece_coord.want_row][piece_coord.want_col]);


    if(og_piece.getColor() == Color::White){
        enemy_color = Color::Black;
        piece_color = Color::White;
    }else{
        enemy_color = Color::White;
        piece_color = Color::Black;
    }

    if(dest_piece.getColor() != Color::Empty && dest_piece.getColor() != enemy_color){
        if(dest_piece.getType() == Type::King && og_piece.getType() == Type::Rook){
            //CASTLİNG CHECK ONE
            cout<<"Castling check1"<<endl;
            if(piece_coord.og_col == 0){
                queen_side_castling = true;
            }else{
                queen_side_castling = false;
            }

            if(is_CastlingValid(dest_piece,og_piece,piece_coord,queen_side_castling)){
                return true;
            }else{
                return false;
            }

        }else if(dest_piece.getType() == Type::Rook && og_piece.getType() == Type::King){
            if(piece_coord.want_col == 0){
                queen_side_castling = true;
            }else{
                queen_side_castling = false;
            }
            if(is_CastlingValid(og_piece,dest_piece,piece_coord,queen_side_castling)){
                return true;
            }else{
                return false;
            }

        }else{
            cout<<"Position is occupied by an ally piece.\n";
            return false;

        }
        
    }

    if(og_piece.getType() == Type::Knight){
        int dx,dy;
        dx = abs(piece_coord.og_col - piece_coord.want_col);
        dy = abs(piece_coord.og_row - piece_coord.want_row);
        if( (dx == 2 && dy == 1) || (dx==1 && dy==2)){
            ///burada kaldik
            //first changing moved pieces in a copy vector to see 
            //new board is getitng checked or not
            copy_vec[piece_coord.want_row][piece_coord.want_col] = og_piece;
            copy_vec[piece_coord.og_row][piece_coord.og_col] = dest_piece;
            if(dest_piece.getColor() != Color::Empty){
                dest_piece.kill(); // we make sure that piece is killed
            }
            coord king_coor = king_finder(piece_color);
            if(king_coor.og_col == -1 || king_coor.og_row == -1){
                cerr<<"Cant find king!";
                exit(1);
            }
            if(is_get_checked(copy_vec,king_coor)==false){
                return true;
            }else{
                cout<<"King still in check"<<endl;
                if(piece_color  == Color::White){
                    if(is_whiteChecked){
                    cout<<"\nYour king is still in check!"<<endl; 
                    return false;   
                    }else{
                    cout<<"\nThis move put your king in check,try another!"<<endl;
                    return false;
                    }
                }else{
                    if(is_blackChecked){
                    cout<<"\nYour king is still in check!"<<endl;    
                    return false;
                    }else{
                    cout<<"\nThis move put your king in check,try another!"<<endl;
                    return false;
                    }
                }
                return false;
            }
        }else{
            cerr<<"Knight can only move L shape!"<<endl;
            return false;
        }
    }
    else if(og_piece.getType() == Type::Rook){
        bool road_blocked = false;
         if (piece_coord.og_row != piece_coord.want_row && piece_coord.og_col != piece_coord.want_col) {
        cout << "Invalid move for rook. Rooks can only move horizontally or vertically." << endl;
        return false;
        }

        // Check for any obstructing pieces along the path
        int row_step = (piece_coord.og_row < piece_coord.want_row) ? 1 : (piece_coord.og_row > piece_coord.want_row) ? -1 : 0;
        int col_step = (piece_coord.og_col < piece_coord.want_col) ? 1 : (piece_coord.og_col > piece_coord.want_col) ? -1 : 0;

        int curr_row = piece_coord.og_row + row_step;
        int curr_col = piece_coord.og_col + col_step;

        while (curr_row != piece_coord.want_row || curr_col != piece_coord.want_col) {
            if (board_vec[curr_row][curr_col].getType() != Type::Empty) {
                cout << "There is an  piece in the path of the rook." << endl;
                road_blocked =  true;
                break;
            }
            curr_row += row_step;
            curr_col += col_step;
        }
        

        if(!road_blocked){
            copy_vec[piece_coord.og_row][piece_coord.og_col] = dest_piece;
            copy_vec[piece_coord.want_row][piece_coord.want_col] = og_piece;
            if(dest_piece.returnAlive() == true){
                copy_vec[piece_coord.og_row][piece_coord.og_col].kill();
            }

            coord king_coords = king_finder(piece_color);
            if(king_coords.og_row == -1 || king_coords.og_col == -1){
                cerr<<"Cant find king!"<<endl;
                exit(1);
            }
            if(is_get_checked(copy_vec,king_coords) == false){
                //can move
                return true;
            }else{
                if(piece_color  == Color::White){
                    if(is_whiteChecked){
                    cout<<"\nYour king is still in check!"<<endl;    
                    }else{
                    cout<<"\nThis move put your king in check,try another!"<<endl;
                    }
                }else{
                    if(is_blackChecked){
                    cout<<"\nYour king is still in check!"<<endl;    
                    }else{
                    cout<<"\nThis move put your king in check,try another!"<<endl;
                    }
                }
                return false;
            }
        }else{
            return false;
        }
    }
    else if(og_piece.getType() == Type::Bishop){
        bool road_blocked = false;
        int diff_row = abs(piece_coord.og_row - piece_coord.want_row);
        int diff_col = abs(piece_coord.og_col - piece_coord.want_col);
        if(diff_row != diff_col){
            cout<<"Bishop can only move diagonally! "<<endl;
            return false;
        }

        int row_step = ((piece_coord.og_row > piece_coord.want_row)? -1:1);
        int col_step = ((piece_coord.og_col > piece_coord.want_col)? -1:1);

        int curr_row = piece_coord.og_row + row_step;
        int curr_col = piece_coord.og_col + col_step;

        while(curr_row != piece_coord.want_row && curr_col != piece_coord.want_col){
            if(board_vec[curr_row][curr_col].getType() != Type::Empty){
                road_blocked = true;
                break;
            }
            curr_row += row_step;
            curr_col += col_step;
        }

        if(!road_blocked){
            coord king_coord  = king_finder(piece_color);

            if(king_coord.og_row == -1 || king_coord.og_col == -1){
                //cant find ally king, something off with board terminate the program
                exit(1);
            }else{
                copy_vec[piece_coord.want_row][piece_coord.want_col] = og_piece;
                copy_vec[piece_coord.og_row][piece_coord.og_col] = dest_piece;
                if(copy_vec[piece_coord.og_row][piece_coord.og_col].returnAlive() == true){
                    copy_vec[piece_coord.og_row][piece_coord.og_col].kill();
                    //we making sure that changed piece is dead after eating.
                }
                coord king_coords = king_finder(piece_color);
                if(king_coords.og_row == -1 || king_coords.og_col == -1){
                    cerr<<"Cant find king!"<<endl;
                    exit(1);
                }
                if(is_get_checked(copy_vec,king_coords) == false){
                    //can move
                    return true;
                }else{
                    if(piece_color  == Color::White){
                    if(is_whiteChecked){
                    cout<<"\nYour king is still in check!"<<endl;    
                    }else{
                    cout<<"\nThis move put your king in check,try another!"<<endl;
                    }
                }else{
                    if(is_blackChecked){
                    cout<<"\nYour king is still in check!"<<endl;    
                    }else{
                    cout<<"\nThis move put your king in check,try another!"<<endl;
                    }
                }
                return false;
                    }
            }
        }else{
            cout<<"Bishop's way blocked by an piece."<<endl;
            return false;
        }
    }
    else if(og_piece.getType() == Type::Queen){
        int row_diff = abs(piece_coord.og_row - piece_coord.want_row);
        int col_diff = abs(piece_coord.og_col - piece_coord.want_col);
            
        if(col_diff != 0 && row_diff != 0 && row_diff != col_diff){
            //checks for rook movements and bishop movements together.
            cout<<"Invalid moveset for quuen."<<endl;
            return false;
        }

        int row_step = ((piece_coord.og_row > piece_coord.want_row)? -1: ((piece_coord.want_row > piece_coord.og_row) ? 1:0 ));
        int col_step = ((piece_coord.og_col > piece_coord.want_col)? -1:((piece_coord.want_col > piece_coord.og_col)? 1:0));

        int curr_row = piece_coord.og_row + row_step;
        int curr_col = piece_coord.og_col + col_step;

        while(curr_col != piece_coord.want_col || curr_row != piece_coord.want_row){
            if(board_vec[curr_row][curr_col].getType() != Type::Empty){
                cout<<"Queen's way blocked by a piece."<<endl;
                return false;
            }
            curr_row += row_step;
            curr_col += col_step;
        }
        copy_vec[piece_coord.og_row][piece_coord.og_col] = dest_piece;
        copy_vec[piece_coord.want_row][piece_coord.want_col] = og_piece;
        if(dest_piece.returnAlive() == true){
                copy_vec[piece_coord.og_row][piece_coord.og_col].kill();
            }

        coord king_coords = king_finder(piece_color);
        if(king_coords.og_row == -1 || king_coords.og_col == -1){
            cerr<<"Cant find king!"<<endl;
            exit(1);
            }
        if(is_get_checked(copy_vec,king_coords) == false){
            //can move
            return true;
        }else{
            if(piece_color  == Color::White){
                    if(is_whiteChecked){
                    cout<<"\nYour king is still in check!"<<endl;    
                    }else{
                    cout<<"\nThis move put your king in check,try another!"<<endl;
                    }
                }else{
                    if(is_blackChecked){
                    cout<<"\nYour king is still in check!"<<endl;    
                    }else{
                    cout<<"\nThis move put your king in check,try another!"<<endl;
                    }
                }
                return false;
        }

    }else if(og_piece.getType() == Type::King){
        int row_diff = (piece_coord.og_row - piece_coord.want_row);
        int col_diff = (piece_coord.og_col - piece_coord.want_col);

        if(row_diff > 1 || col_diff > 1){
            cout<<"King can only move to squares within radius of 1."<<endl;
            return false;
        }
        copy_vec[piece_coord.og_row][piece_coord.og_col] = dest_piece;
        copy_vec[piece_coord.want_row][piece_coord.want_col] = og_piece;
        if(dest_piece.returnAlive() == true){
                copy_vec[piece_coord.og_row][piece_coord.og_col].kill();
            }

        coord king_coords;
        king_coords.og_row = piece_coord.want_row;
        king_coords.og_col = piece_coord.want_col;
        if(is_get_checked(copy_vec,king_coords) == false){
            //can move
            return true;
        }else{
            if(piece_color  == Color::White){
                    if(is_whiteChecked){
                    cout<<"\nYour king is still in check!"<<endl;    
                    }else{
                    cout<<"\nThis move put your king in check,try another!"<<endl;
                    }
                }else{
                    if(is_blackChecked){
                    cout<<"\nYour king is still in check!"<<endl;    
                    }else{
                    cout<<"\nThis move put your king in check,try another!"<<endl;
                    }
                }
            return false;
        }

    }else if(og_piece.getType() == Type::Pawn){
        int row_step =0,col_step =0;
        if(piece_color == Color::White){
            row_step = -1;
            if(piece_coord.want_row >= piece_coord.og_row){
                cout<<"Pawn's cant move backwards!"<<endl;
                return false;
            }
        }else{
            row_step = 1;
            if(piece_coord.want_row <= piece_coord.og_row){
                cout<<"Pawn's cant move backwards!"<<endl;
                return false;
            }
        }

        int row_diff = abs(piece_coord.og_row - piece_coord.want_row);
        int col_diff = abs(piece_coord.og_col - piece_coord.want_col);

        if(row_diff == 2 && col_diff == 0){
            if(og_piece.return_playCount() > 0){
                cout<<"Pawn can only move 2 squares when its not played before."<<endl;
                return false;
            }else{
                if(board_vec[piece_coord.og_row + row_step][piece_coord.og_col].getType() != Type::Empty){
                    cout << " Pawn's way blocked by an piece."<<endl;
                    return false;
                }else{
                    //Pawn can move 2 squares forward but we have to check if its get checked 
                    copy_vec[piece_coord.want_row][piece_coord.want_col] = og_piece;
                    copy_vec[piece_coord.og_row][piece_coord.og_col] = dest_piece;
                    if(copy_vec[piece_coord.og_row][piece_coord.og_col].returnAlive() != false){
                        copy_vec[piece_coord.og_row][piece_coord.og_col].kill();
                    }

                    coord king_coordinates;
                    king_coordinates=king_finder(piece_color);
                    if(king_coordinates.og_col == -1 || king_coordinates.og_row == -1){
                        cerr<<"Cant find king! Terminating the program."<<endl;
                        exit(1);
                    }

                    if(is_get_checked(copy_vec,king_coordinates) == false){
                        return true;
                    }
                    else{
                        if(piece_color  == Color::White){
                        if(is_whiteChecked){
                        cout<<"\nYour king is still in check!"<<endl;    
                            }else{
                            cout<<"\nThis move put your king in check,try another!"<<endl;
                            }   
                        }else{
                            if(is_blackChecked){
                            cout<<"\nYour king is still in check!"<<endl;    
                            }else{
                            cout<<"\nThis move put your king in check,try another!"<<endl;
                            }
                            }
                    return false;
                    }
                }
            }
        }else if(col_diff == 1 && row_diff == 1){
            //can only move diagonally when there is an enemy 
            if(board_vec[piece_coord.want_row][piece_coord.want_col].getColor() == enemy_color){
                copy_vec[piece_coord.want_row][piece_coord.want_col] = og_piece;
                    copy_vec[piece_coord.og_row][piece_coord.og_col] = dest_piece;
                    if(copy_vec[piece_coord.og_row][piece_coord.og_col].returnAlive() != false){
                        copy_vec[piece_coord.og_row][piece_coord.og_col].kill();
                    }

                    coord king_coordinates;
                    king_coordinates=king_finder(piece_color);
                    if(king_coordinates.og_col == -1 || king_coordinates.og_row == -1){
                        cerr<<"Cant find king! Terminating the program."<<endl;
                        exit(1);
                    }

                    if(is_get_checked(copy_vec,king_coordinates) == false){
                        return true;
                    }
                    else{
                        if(piece_color  == Color::White){
                        if(is_whiteChecked){
                        cout<<"\nYour king is still in check!"<<endl;    
                        }else{
                        cout<<"\nThis move put your king in check,try another!"<<endl;
                        }
                            }else{
                            if(is_blackChecked){
                            cout<<"\nYour king is still in check!"<<endl;    
                        }else{
                        cout<<"\nThis move put your king in check,try another!"<<endl;
                        }
                        }
                            return false;
                    }
            }else{
                cout<<"Pawn cant move diagonally when there is none enemy it can eat."<<endl;
                return false;
            }
        }else if(col_diff==0 && row_diff == 1){
            if(board_vec[piece_coord.og_row + row_step][piece_coord.og_col].getColor() != Color::Empty){
                cout<<"Pawn's way blocked by a piece.\n";
                return false;
            }else{
                copy_vec[piece_coord.want_row][piece_coord.want_col] = og_piece;
                    copy_vec[piece_coord.og_row][piece_coord.og_col] = dest_piece;
                    if(copy_vec[piece_coord.og_row][piece_coord.og_col].returnAlive() != false){
                        copy_vec[piece_coord.og_row][piece_coord.og_col].kill();
                    }

                    coord king_coordinates;
                    king_coordinates=king_finder(piece_color);
                    if(king_coordinates.og_col == -1 || king_coordinates.og_row == -1){
                        cerr<<"Cant find king! Terminating the program."<<endl;
                        exit(1);
                    }

                    if(is_get_checked(copy_vec,king_coordinates) == false){
                        return true;
                    }
                    else{
                            if(piece_color  == Color::White){
                        if(is_whiteChecked){
                        cout<<"\nYour king is still in check!"<<endl;    
                        }else{
                        cout<<"\nThis move put your king in check,try another!"<<endl;
                        }
                        }else{
                            if(is_blackChecked){
                            cout<<"\nYour king is still in check!"<<endl;    
                            }else{
                            cout<<"\nThis move put your king in check,try another!"<<endl;
                            }
                        }
                        return false;
                    }
            }
        }else{
            cout<<"Invalid moveset for pawn."<<endl;
            return false;
        }
    }else{
        return false;
    }
}


void Board::move_piece(const coord move_coords){
    Piece og_piece(board_vec[move_coords.og_row][move_coords.og_col]);
    Piece dest_piece(board_vec[move_coords.want_row][move_coords.want_col]);
    
    bool is_castling = false;
    
    if(og_piece.getColor() == dest_piece.getColor()){
        if(og_piece.getType() == Type::King && dest_piece.getType() == Type::Rook){
            is_castling = true;
        }else if(og_piece.getType() == Type::Rook && dest_piece.getType() == Type::King){
            is_castling = true;
        }else{
            is_castling = false;
        }
    }
    og_piece.moveInc();    
    if(!is_castling){
    if(dest_piece.getType() != Type::Empty){
        if(og_piece.getColor() == Color::White){
            switch (dest_piece.getType())
            {
            case Type::Pawn:
                white_sc += 1;
                break;
            case Type::Knight:
                white_sc += 3;
                break;
            case Type::Bishop:
                white_sc += 3;
                break;
            case Type::Rook:
                white_sc += 5;
                break;
            case Type::Queen:
                white_sc += 9;
                break;
        
            default:
                break;
            }
            
        }else{
            switch (dest_piece.getType())
                {
                case Type::Pawn:
                black_sc += 1;
                break;
            case Type::Knight:
                black_sc += 3;
                break;
            case Type::Bishop:
                black_sc += 3;
                break;
            case Type::Rook:
                black_sc += 5;
                break;
            case Type::Queen:
                black_sc += 9;
                break;
        
            default:
                break;
            }
        }
    }
    
    if(og_piece.getType() == Type::Pawn){
    	if(og_piece.getColor() == Color::White){
    		if(move_coords.want_row == 0){
    			//pawn promotion
    			og_piece.promote();
    		}
    	
    	}else{
    		if(move_coords.want_row == 7){
    			og_piece.promote();
    		}
    	}
    }
    
    
    
    board_vec[move_coords.want_row][move_coords.want_col] = og_piece;
    board_vec[move_coords.og_row][move_coords.og_col] = dest_piece;

    if(board_vec[move_coords.og_row][move_coords.og_col].returnAlive() != false){
        board_vec[move_coords.og_row][move_coords.og_col].kill();
    }
    }
    else{
        dest_piece.moveInc();
        //it is a castling

        bool queen_castling = false;
        if(og_piece.getType() == Type::King){
            //in this case  a2 d3
            if(move_coords.want_col == 0){
                //queenside 
                queen_castling = true;
            }else{
                queen_castling = false;
            }
            if(queen_castling){
                board_vec[move_coords.og_row][move_coords.og_col - 2] = og_piece;
                board_vec[move_coords.og_row][move_coords.og_col].kill();

                board_vec[move_coords.want_row][move_coords.want_col + 3] = dest_piece;
                board_vec[move_coords.want_row][move_coords.want_col].kill();
            }else{
                board_vec[move_coords.og_row][move_coords.og_col + 2] = og_piece;
                board_vec[move_coords.og_row][move_coords.og_col].kill();

                board_vec[move_coords.want_row][move_coords.want_col - 2] = dest_piece;
                board_vec[move_coords.want_row][move_coords.want_col].kill();
            }

        }else{
            if(move_coords.og_col == 0){
                queen_castling = true;
            }else{
                queen_castling = false;
            }

            if(queen_castling){
                board_vec[move_coords.want_row][move_coords.want_col -2 ] = dest_piece;
                board_vec[move_coords.want_row][move_coords.want_col].kill();

                board_vec[move_coords.og_row][move_coords.og_col + 3] = og_piece;
                board_vec[move_coords.og_row][move_coords.og_col].kill();

            }else{
                board_vec[move_coords.want_row][move_coords.want_col + 2] = dest_piece;
                board_vec[move_coords.want_row][move_coords.want_col].kill();

                board_vec[move_coords.og_row][move_coords.og_col - 2 ] = og_piece;
                board_vec[move_coords.og_row][move_coords.og_col].kill();


            }
        }


    }
    isWhitesTurn = !isWhitesTurn;
}
void Board::gameLoop(){
        bool save_flag = false;
        bool game_ended = false;
        bool close_flag = false;
        coord input_cords;
        do
        {
            print_board();
            do{
                input_cords = takeInput();
                if(input_cords.og_col == -1 && input_cords.og_row == 15){
                    save_flag = true;
                    break;
                }else if(input_cords.og_col == -2 && input_cords.og_row == 15){
                    close_flag = true;
                    break;
                }
            }while(is_moveValid(input_cords) == false);
            if(save_flag){
                break;
            }
            if(close_flag)break;
            black_sc += 15;
            move_piece(input_cords);
            if(isWhitesTurn){
               if(is_get_checked(board_vec,king_finder(Color::White))){
                //check if white is mat
                is_whiteChecked = true;
                cout<<"White king is checked!"<<endl;
               }else{is_whiteChecked = false;}
            }else{
                if(is_get_checked(board_vec,king_finder(Color::Black))){
                cout<<"Black king is checked!"<<endl;
                is_blackChecked = true;
                
               }else{is_blackChecked = false;}
            }
            if(is_whiteChecked || is_blackChecked){ 
                if(is_checkMate()){
                    game_ended = true;
                    //end board pointsi yazdır
                    if(is_whiteChecked){
                        cout<<"Black has won!"<<endl;
                    }else if (is_blackChecked){
                        cout<<"White has won!"<<endl;
                    }
                    break;
                }else{
                    game_ended = false;
                };
            }
            /* code */
        } while (!game_ended);
        if(game_ended){
            int most_score = endBoardPoints();
            end_gamePrint(most_score);
        }

        if(save_flag){
            this->save_board();
        }
    
}
bool Board::returnTurn()const{
return isWhitesTurn;}
void Board::load_Board(){
    ifstream in_file(Board::save_file_name,ios::binary);
    if(in_file.is_open()){
    	in_file.read(reinterpret_cast<char*>(&isWhitesTurn),sizeof(bool));
        in_file.read(reinterpret_cast<char*>(&white_sc),sizeof(int));
        in_file.read(reinterpret_cast<char*>(&black_sc),sizeof(int));
        for(auto& row: board_vec){
            in_file.read(reinterpret_cast<char*>(row.data()),row.size() * sizeof(Piece));

        }

        in_file.close();
        cout<<"Board loaded from file : "<<Board::save_file_name<<" succesfully!"<<endl;
        
    }else{
        cerr<<"Cant open file : "<<Board::save_file_name<<" ."<<endl;
    }
}
void Board::end_gamePrint(int score){ // tested works
    ifstream input_file("bestscore.txt",ios::in);
    
    string line;
    if(getline(input_file,line)){
        //there is a line in txt
        string token;
        istringstream ss(line);
        ss>>token>>token>>token;

        int existing_score = stoi(token);
        input_file.close();
        if(score>existing_score){
            ofstream output_file(Board::score_filename,ios::out);
            output_file<<"Best Score: "<<score<<endl;
            int count = 1;
            
            for(const auto& row:board_vec){
                output_file<<count++<<" |";
                for(const auto& piece:row){
                    output_file<<piece.getSymbol()<<" ";
                }
                output_file<<"\n";
            }
                output_file<<"  |________________\n";
                output_file<<"  |a b c d e f g h\n";
                output_file.close();       
            
        }
    }else{
        input_file.close();
        ofstream output_file("bestscore.txt",ios::out);
        output_file<<"Best Score: "<<score<<endl;
        int count = 1;
        for(const auto row:board_vec){
            output_file<<count++<<" |";
            for(const auto piece:row){
                output_file<<piece.getSymbol()<<" ";
            }
            output_file<<"\n";
        }
        output_file<<"  |________________\n";
        output_file<<"  |a b c d e f g h\n";

        output_file.close();
    }

}
bool Board::save_board(){
    ofstream savefile(Board::save_file_name,ios::binary);
    if(savefile.is_open()){
    	savefile.write(reinterpret_cast<const char*>(&isWhitesTurn),sizeof(bool));
        savefile.write(reinterpret_cast<const char*>(&white_sc),sizeof(int));
        savefile.write(reinterpret_cast<const char*>(&black_sc),sizeof(int));    
        for(const auto& row:board_vec){
            savefile.write(reinterpret_cast<const char*>(row.data()),row.size() * sizeof(Piece));
        }
        savefile.close();
        cout<<"Board saved to file: "<<Board::save_file_name<<endl;
        return true;
    }else{
        cerr<<"Error oppening save file: "<<Board::save_file_name<<endl;
        return false;
    }
}
void Board::print_board(){
int count = 1;
for(auto row:board_vec){
    cout<<count++<<" |";
    for(auto col:row){
        cout<<col.getSymbol()<<" ";
    }
    cout<<"\n";
}
cout<<"  |________________\n";
cout<<"  |a b c d e f g h\n";
}

string Board::score_filename = "score_file.txt";
string Board::save_file_name = "save_file.dat";
