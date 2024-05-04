#ifndef OS_H
#define OS_H
#include "file.h"
struct FileInfo;
class OS{
    private:
        static const string operands[];
        
        const size_t disk_space;
        size_t used_space; 
        string current_path;

        shared_ptr<Directory> root;
        shared_ptr< Directory> currentDir;
        
        void saveFileToFile(const shared_ptr<File>& file,ofstream& fileout,ofstream& outregular);
        void readit(vector<FileInfo>& fi);       
        void loadBasicInfoFromFile(vector<FileInfo>& files, ifstream& in,ifstream& in_data);

        vector<string> tokenize_path(string);

        vector<string> tokenize_string(string);
        bool is_op_correct(string op);
        void operate(string input,vector<string>params);
        void change_Path();
    public:
        void saveToFile();
        void loadFromFile();

        static int parentdummy ;

        void removeInvalidLinkers(const shared_ptr<File>& file);
        

        OS();
        void bash();
        bool loadOS();
        bool saveOS();
};




#endif