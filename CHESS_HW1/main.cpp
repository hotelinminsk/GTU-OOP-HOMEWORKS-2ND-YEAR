#include "board.h"
#include "fstream"
const regex chess_not("[a-h][1-8][a-h][1-8]");

int game_menu(){
     int choice;
    bool error = false;
    do
    {
        if (!error)
        {
            cout<<"\n1)Play Game\n2)See Best Score\n3)Load Another Board\n4)Exit ";
        }else{
            cout<<"Invalid input try again : ";
        }

        cin>>choice;
        if(choice<1 && choice > 4){
            error = true;
        }else{error=false;}
        
    } while (error);
    
    return choice;
}
void showBestScore(const string filename){
    ifstream input(filename,ios::in);
    if(input.is_open()){
        string line;
        while(getline(input,line)){
            cout<<"Best Board\n"<<line<<endl;
        }
    }
}

int main(){
    int choice;
    string input_data;
    Board game_board;
    coord move_coord;
    bool errorFLAG = false;
    bool save_flag = false;
    bool suggest_flag = false;
    do
    {
        choice = game_menu();
        if(choice == 1){
            game_board.gameLoop();
        }else if(choice == 2){
            showBestScore("score_file.txt");
        }else if(choice == 3){
            game_board.load_Board();
            cout<<"Loaded board's player turn: "<<((game_board.returnTurn() == true) ? "White\n":"Black\n");
        }
        //oyunu oyna.
        

    } while (choice != 4);
    





    return 0;
}
