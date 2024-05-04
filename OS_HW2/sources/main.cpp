#include "../libs/os.h"
#include <iostream>
#include <fstream>
using namespace std;
int main(){
    bool save_exists = false;
    bool bash_open = false;
    do
    {
        string choice;
        cout<<"For opening bash write -> myBash\nFor closing the program write -> exit "<<endl;
        cin>>choice;
        if(choice=="myBash"){
            bash_open = true;
        }else if(choice == "exit"){
            exit(1);
        }else{
            bash_open = false;
        }
    } while (!bash_open);
    

    try
    {
        OS myos;
        ifstream in("./saves/disk_space.txt");
        ifstream in_data_rf("./saves/regular_data.txt");
        if(in.is_open() && in_data_rf.is_open()){
            save_exists = true;
            in.close();in_data_rf.close();
        }
        if(save_exists){
            cout<<"Loading save.."<<endl;
            myos.loadFromFile();
        }
        
        while(true){
            myos.bash();
        }


    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}