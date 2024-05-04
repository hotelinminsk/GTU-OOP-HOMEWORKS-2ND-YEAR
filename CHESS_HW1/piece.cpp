#include "piece.h"


using namespace std;

std::ostream& operator<<(std::ostream& os,const Color& color){
    switch (color)
    {
    case Color::White:
        os<<"white";
        break;
    case Color::Black:
        os<<"black";
        break;
    case Color::Empty:
        os<<"empty";
        break;
    default:
        break;
    }

    return os;
}

std::ostream& operator<<(std::ostream& os,const Type& type) {
    switch (type)
    {
    case Type::Pawn:
        os<<"Pawn";
        break;
    case Type::Bishop:
        os<<"Bishop";
        break;
    case Type::King:
        os<<"King";
        break;
    case Type::Knight:
        os<<"Knight";
        break;
    case Type::Queen:
        os<<"Queen";
        break;
    case Type::Rook:
        os<<"Rook";
        break;
    case Type::Empty:
        os<<"Empty";
        break;
    default:
        break;
    }
    return os;
}

char Piece::getSymbol()const{return symbol;}

Type Piece::getType()const{return p_type;}
Color Piece::getColor()const{return p_color;}

void Piece::promote(){
	bool error_flag = false;
	cout<<"You can promote your pawn!"<<endl;
	char choice;
	//case difference
	do{
		if(!error_flag){
			cout<<"Which piece you wanna promote your pawn?"<<endl;
			cout<<"For bishop -> b\nFor rook -> r\nFor quuen -> q\nFor knight -> k\nGive input : ";
		
		}else{
		cerr<<"Please give valid input.(for bishop = b,queen = q,rook = r)";
		}
		cin >> choice;
		if(choice == 'b' || choice == 'q' || choice == 'r' || choice == 'k'){
			error_flag = false;
		}else if(choice == 'B' || choice == 'Q' || choice == 'R' || choice == 'K'){
			error_flag = false;
			choice += 32;
		}
	}while(error_flag);
	
	
	if(this->getColor() == Color::White){
		switch(choice){
		
		case 'b':
			this->p_type = Type::Bishop;
			this->symbol = 'B';
			break;
		
		case 'q':
			this->p_type = Type::Queen;
			this->symbol = 'Q';
			break;
		
		case 'r':
			this->p_type = Type::Rook;
			this->symbol = 'R';
			break;
		
		case 'k':
			this->p_type = Type::Knight;
			this->symbol = 'K';
			break;
		}
		
		
	}else{
	
		switch(choice){
		
		case 'b':
			this->p_type = Type::Bishop;
			this->symbol = 'b';
			break;
		
		case 'q':
			this->p_type = Type::Queen;
			this->symbol = 'q';
			break;
		
		case 'r':
			this->p_type = Type::Rook;
			this->symbol = 'r';
			break;
		case 'k':
			this->p_type = Type::Knight;
			this->symbol = 'k';
			break;
		}
	}
	
}

Piece& Piece::operator=(const Piece& other){
    this->symbol = other.symbol;
    this->is_alive = other.is_alive;
    this->p_color = other.p_color;
    this->p_type = other.p_type;
    this->play_count = other.play_count;

    return *this;
}
Piece::Piece(const Piece& other){
    this->is_alive = other.is_alive;
    this->p_color = other.p_color;
    this->p_type = other.p_type;
    this->play_count = other.play_count;
    this->symbol = other.symbol;
}
void Piece::kill(){
    this->is_alive = false;
    this->p_color = Color::Empty;
    this->p_type = Type::Empty;
    this->play_count = 0;
    this->symbol = '.';
}
Piece::Piece(Color s_color,Type s_type):play_count(0){
    if(s_type == Type::Empty){
        is_alive = false;
    }else{is_alive=true;}

    if(s_type == Type::Bishop){
        if(s_color == Color::Black){
            this->p_type = s_type;
            this->symbol = 'b';
            this->p_color = s_color;
        }else{
            this->p_type = s_type;

            this->p_color = s_color;
            this->symbol = 'B';
        }
        
    }else if(s_type == Type::Empty){
            this->p_type = s_type;

        this->p_color = s_color;
        this->symbol = '.';
    }else if(s_type == Type::King){
        if(s_color == Color::Black){
            this->p_type = s_type;

            this->p_color = s_color;
            this->symbol = 'k';
        }else{
            this->p_type = s_type;

            this->p_color = s_color;
            this->symbol = 'K';
        }
        
    }else if(s_type == Type::Knight){
        if(s_color == Color::Black){
            this->p_type = s_type;

            this->p_color = s_color;
            this->symbol = 'n';
        }else{
            this->p_type = s_type;

            this->p_color = s_color;
            this->symbol = 'N';
        }
        
    }else if(s_type == Type::Pawn){
        if(s_color == Color::Black){
            this->p_type = s_type;

            this->p_color = s_color;
            this->symbol = 'p';
        }else{
            this->p_type = s_type;

            this->p_color = s_color;
            this->symbol = 'P';
        }
        
    }else if(s_type == Type::Queen){
        if(s_color == Color::Black){
            this->p_type = s_type;

            this->p_color = s_color;
            this->symbol = 'q';
        }else{
            this->p_type = s_type;

            this->p_color = s_color;
            this->symbol = 'Q';
        }
        
    }else if(s_type == Type::Rook){
        if(s_color == Color::Black){
            this->p_type = s_type;

            this->p_color = s_color;
            this->symbol = 'r';
        }else{
            this->p_type = s_type;

            this->p_color = s_color;
            this->symbol = 'R';
        }
    }
}
