#include "../libs/os.h"
#include <sstream>
#include <stack>
#include <fstream>
#include <unistd.h>
const string OS::operands[] ={
    "mkdir", // implemented working
    "ls", // implemented for directory
    "cp", // implemented working
    "rm", // implemented workingg
    "link", // implemented
    "cd", // implemented
    "cat", // implemented
    "rmdir", // implemented
    "exit", //end
};
struct FileInfo{
    string name;
    char type;
    size_t size;
    string date_time;
    int unique;
    int parentunique;
    int linkedinuqiue;
    size_t contentsSize;
    vector<char> data;

};
int OS::parentdummy = -1;
void OS::saveToFile(){
   
    const string filename = "./saves/disk_space.txt";
    const string rf_outdata = "./saves/regular_data.txt";
    
    ofstream file(filename,ios::out | ios::binary);
    ofstream regulardata(rf_outdata,ios::out | ios::binary);
    if(!regulardata.is_open() || !file.is_open()){
        cerr<<"Error opening save file."<<endl;
        throw invalid_argument("Cant open the regular_data.txt or disk_space.txt");
    }
    saveFileToFile(root,file,regulardata);



    file.close();
    regulardata.close();
}


void OS::saveFileToFile(const shared_ptr<File>& file, ofstream& out, ofstream& regular_data_out) {
    out << file->getName() << endl;
    out << file->getType() << endl;
    out << file->getSize() << endl;
    out << file->getDT() << endl;
    out << file->getUnique() << endl;

    if (file->getParent() != nullptr) {
        out << file->getParent()->getUnique() << endl;
    } else {
        out << OS::parentdummy << endl;
    }

    if (auto l_f = dynamic_pointer_cast<LinkerFile>(file)) {
        if (auto linkedFile = l_f->getLinked()) {
            if (linkedFile->getUnique() == file->getUnique()) {
                out << OS::parentdummy << endl;  
            } else {
                out << linkedFile->getUnique() << endl;
                out << "0" << endl;  
            }
        } else {
            cerr << "Error: LinkerFile has a null linked file." << endl;
            
        }
    } else {
        out << OS::parentdummy << endl;  // dummy linker -1
    }

    if (auto d_f = dynamic_pointer_cast<Directory>(file)) {
        out << d_f->getContents().size() << endl;
    } else if (auto r_f = dynamic_pointer_cast<RegularFile>(file)) {
        out << r_f->getData().size() << endl;
        regular_data_out.write(r_f->getData().data(), r_f->getData().size());
    }
    
    if (auto d_f = dynamic_pointer_cast<Directory>(file)) {
        for (auto& x : d_f->getContents()) {
            saveFileToFile(x, out, regular_data_out);
        }
    }
}

void OS::loadBasicInfoFromFile(vector<FileInfo>& files, ifstream& in, ifstream& in_data) {
    while (true) {
        FileInfo newfile;
        string saat;
        string tarih;
        if (!(in >> newfile.name >> newfile.type >> newfile.size >> tarih >> saat >> newfile.unique)) {
            
            break;
        }
        tarih = tarih + " " +saat;
        newfile.date_time = tarih;
        
        if (newfile.type == 'F') {
            newfile.data.resize(newfile.size);
            in_data.read(newfile.data.data(), newfile.size);
        }

        in >> newfile.parentunique >> newfile.linkedinuqiue >> newfile.contentsSize;

        files.push_back(newfile);
    }

    readit(files);
}




void OS::loadFromFile(){
    ifstream in("./saves/disk_space.txt",ios::in | ios::binary);
    ifstream in_data_rf("./saves/regular_data.txt",ios::in | ios::binary);


    if(!in.is_open() || !in_data_rf.is_open()){
        throw runtime_error("Cant open filesystem save files.");
    }

    root->getContents().clear();
    vector<FileInfo> fileifos;
    loadBasicInfoFromFile(fileifos,in,in_data_rf);

    in.close();
}

void OS::readit(vector<FileInfo>& fi){
    vector<shared_ptr<File>> createdOnes;
   
    for(auto start = fi.begin();start != fi.end();start++){
        if(start->parentunique == -1){
            root->setdt(start->date_time);
            root->setSize(start->size);
            createdOnes.push_back(root);
        }else{
            shared_ptr<Directory> p_ptr;
            shared_ptr<File> link_ptr;
            for(auto& x: createdOnes){
                if(x->getUnique() == start->parentunique){
                    p_ptr = dynamic_pointer_cast<Directory>(x);
                }else if(start->type == 'L'){
                    if(x->getUnique() == start->linkedinuqiue){
                        link_ptr = x;
                    }
                }
            }
            
            
            if(start->type == 'D'){
                
                auto newdir = make_shared<Directory>(start->name,p_ptr,'D',start->size);
                newdir->setdt(start->date_time);
                newdir->setUniq(start->unique);
                createdOnes.push_back(newdir);
            }else if(start->type == 'F'){
                auto newf = make_shared<RegularFile>(start->name,p_ptr,start->size);
                newf->setUniq(start->unique);
                newf->setdt(start->date_time);
                for(auto m : start->data){
                    newf->setData(m);
                }
                createdOnes.push_back(newf);
            }else{
                auto newlinker = make_shared<LinkerFile>(start->name,p_ptr,link_ptr,start->size);
                newlinker->setdt(start->date_time);
                newlinker->setUniq(start->unique);
                newlinker->set_lk_unique(start->linkedinuqiue);
                createdOnes.push_back(newlinker);
            }
        }
    }
    for(auto first= createdOnes.begin();first != createdOnes.end();first++){
        for(auto second = first + 1;second != createdOnes.end();second++ ){
            if((*first)->getUnique() == (*second)->getParentUniq()){
                dynamic_pointer_cast<Directory>((*first))->add_data(*second);
            }
            if(auto m = dynamic_pointer_cast<LinkerFile>(*second)){
                if((*first)->getUnique() == m->getLinkedUnique()){
                    m->setLinked(*first);
                }
            }

        }
    }

    root->calculateSIZE();
}



OS::OS():disk_space(10*1024*1024){
    
    root = make_shared<Directory>("root",nullptr,'D',1024);
    used_space  = 1024;
    currentDir = root;
    current_path = "root/";

}

vector<string> OS::tokenize_string(string input){
    istringstream  iss(input);
    string token;
    vector<string> tokens;
    while(iss>>token){
        tokens.push_back(token);
    }
    return tokens;
}
bool OS::is_op_correct(string op){
    bool correct = false;
    for(auto x:operands){
        if(op == x){
            correct = true;
            break;
        }
    }

    return correct;
}
vector<string> OS::tokenize_path(string parameter){
    vector<string> pathTokens;
    istringstream tokenStream(parameter);
    string token;
    while (getline(tokenStream, token, '/')) {
        if (!token.empty()) {
            pathTokens.push_back(token);
        }
    }
    return pathTokens;
}
void OS::change_Path(){
    istringstream iss;
    stack<string> reversed_name;

    if(currentDir->getParent() == nullptr){
        current_path = currentDir->getName() + "/";
        
    }else{
        string temp_string = "";
        shared_ptr<Directory> tempptr = currentDir;
        while(tempptr != nullptr){
            reversed_name.push(tempptr->getName());
            reversed_name.push("/");
        
            tempptr = tempptr->getParent();
        }

        while(!reversed_name.empty()){
            temp_string += reversed_name.top();
            reversed_name.pop();
        }
        current_path = temp_string;

    }
}

shared_ptr<LinkerFile> copyLinker(const LinkerFile& baseLinker){
    shared_ptr<LinkerFile> newL = make_shared<LinkerFile>(baseLinker.getName(),nullptr,baseLinker.getLinked(),24);     
    return newL;
}
shared_ptr<RegularFile> copyFile(const RegularFile& baseFile){
    shared_ptr<RegularFile> newF = make_shared<RegularFile>(baseFile.getName(),nullptr,baseFile.getSize());
    for(auto a : baseFile.getData()){
        newF->setData(a);
    }
    return newF;
}

shared_ptr<Directory> copyDirectory(const Directory& baseDir) {
    shared_ptr<Directory> newDir = make_shared<Directory>(baseDir.getName() + "_cpy",nullptr,'D',baseDir.getSize());
    for (const auto& file : baseDir.getContents()) {
        if (file->getType() == 'F') {
            shared_ptr<RegularFile> newF = copyFile(*(dynamic_pointer_cast<RegularFile>(file)));
            newF->setParentPTR(newDir);
            newDir->add_data(newF);
        } else if (file->getType() == 'L') {
            shared_ptr<LinkerFile> newL = copyLinker(*(dynamic_pointer_cast<LinkerFile>(file)));
            newL->setParentPTR(newDir);
            newDir->add_data(newL);
        } else if (file->getType() == 'D') {
            shared_ptr<Directory> newD = copyDirectory(*(dynamic_pointer_cast<Directory>(file)));
            newD->setParentPTR(newDir);
            newDir->add_data(newD);
        }
    }

    return newDir;
}


void OS::operate(string input,vector<string>params){
    if(input == "mkdir"){
        for(auto x : currentDir->getContents()){
            if(x->getName() == params[0]){
              cerr<<"Cant create directory: "<<params[0]<<" is exists."<<endl;
              return;  
            }
        }

        if(root->getSize() + 1024 >= disk_space){
            cerr<<"Disk space is full you have to remove some files."<<endl;
            return;
        }
        if(currentDir->getType() != 'D'){
            cerr<<"Cant create a folder inside a non dir file."<<endl;
            return;
        }
        else{
            shared_ptr<Directory> newD = make_shared<Directory>(params[0],currentDir,'D',1024);
            currentDir->add_data(newD);
            currentDir->calculateSIZE();
            return; 
        }
    }else if(input == "exit"){
        saveToFile();
        exit(1);
        //implement saving when exiting
    }
    else if(input == "ls"){
        if(currentDir->getType() != 'D'){
            cerr<<"Cant use ls on non directory file!"<<endl;
            return;

        }else if(params[0] == "-R"){
            currentDir->lsr(currentDir);
        }
        else{
            currentDir->ls();
            return;
        }
    }else if(input == "cat"){
        for(auto x:currentDir->getContents()){
            if(params[0] == x->getName()){
                x->cat();
                return ;
            }
        }//stayed here
        
    }else if(input == "cp"){
        
        bool is_inside_os = false;
        shared_ptr<Directory> d;
        if(params[0].find('/') != string::npos){
            vector<string>tokens = tokenize_path(params[0]);
            for(auto a: currentDir->getContents()){
                if(a->getName() == *tokens.begin() ){
                    is_inside_os = true;
                    break;
                }
            }
                        
            if(is_inside_os){
                shared_ptr<Directory> original_dir = currentDir;
                string name_temp = *(tokens.end() - 1 );
                tokens.pop_back();
                operate("cd",tokens);
                for(auto x : currentDir->getContents()){
                    if(x->getType() == 'D'){
                        if(x->getName() == name_temp){
                            if(x->getSize() + root->getSize() > 10*1024*1024){
                                cerr<<"Not enough disk space!"<<endl;
                                return ;
                            }
                        d = copyDirectory(*(dynamic_pointer_cast<Directory>(x)));
                        d->setParentPTR(original_dir);
                        original_dir->add_data(d);
                        break;
                        }
                    }else if(x->getType() == 'F'){
                        if(x->getName() == name_temp){
                            if(x->getSize() + root->getSize() > 10*1024*1024){
                                cerr<<"Not enough disk space!"<<endl;
                                return ;
                            }
                        shared_ptr<RegularFile> rf = copyFile(*(dynamic_pointer_cast<RegularFile>(x)));
                        rf->setParentPTR(original_dir);
                        original_dir->add_data(rf);
                        break;
                        }
                    }else if(x->getType() == 'L'){
                        if(x->getName() == name_temp ){
                            if(x->getSize() + root->getSize() > 10*1024*1024){
                                cerr<<"Not enough disk space!"<<endl;
                                return ;
                            }
                            shared_ptr<LinkerFile> lf = copyLinker(*(dynamic_pointer_cast<LinkerFile>(x)));
                            lf->setParentPTR(original_dir);
                            original_dir->add_data(lf);
                            break;
                        }
                    }
                }
                currentDir = original_dir;
                change_Path();
                return;
            }
        }else{
            for(auto a: currentDir->getContents()){
                
                if(a->getName() == params[0]){
                    is_inside_os = true;
                    if(a->getType() == 'D'){
                        if(a->getSize() + root->getSize() > 10*1024*1024){
                            cerr<<"Not enough disk space to copy "<<a->getName()<< " directory."<<endl;
                            return;
                        }
                        d = copyDirectory(*(dynamic_pointer_cast<Directory>(a)));
                        d->setParentPTR(currentDir);
                        currentDir->add_data(d);
                    }else if(a->getType() == 'F'){
                        if(a->getSize() + root->getSize() > 10*1024*1024){
                            cerr<<"Not enough disk space to copy "<<a->getName()<< " file."<<endl;
                            return;
                        }
                        shared_ptr<RegularFile> rf = copyFile(*(dynamic_pointer_cast<RegularFile>(a)));
                        rf->setParentPTR(currentDir);
                        currentDir->add_data(rf);

                    }else if(a->getType() == 'L'){
                        if(a->getSize() + root->getSize() > 10*1024*1024){
                            cerr<<"Not enough disk space to copy "<< a->getName()<< " linker." <<endl;
                            return;
                        }
                        shared_ptr<LinkerFile> lf = copyLinker(*(dynamic_pointer_cast<LinkerFile>(a)));
                        lf->setParentPTR(currentDir);
                        currentDir->add_data(lf);

                    }
                    break;
                }else{is_inside_os = false;}
            }
        }

        if(!is_inside_os){
            for(auto x : currentDir->getContents()){
                if(x->getName() == params[1]){
                    params[1] = params[1] + "_" + to_string(rand() % 150);
                }
            }

            ifstream input(params[0],ios::binary);
            if(!input.is_open()){
                throw runtime_error("Error oppening file.");
            }

            input.seekg(0,std::ios::end);
            size_t file_bytes = static_cast<size_t>(input.tellg());
            input.seekg(0,ios::beg);

            if(root->getSize() + file_bytes >= disk_space){
                cerr<<"Not enough disk space to copy  file -> "<<params[0]<<" remove some file."<<endl;
                cerr<<"Root size : "<<root->getSize()<<" disk space "<<disk_space<<" file_bytes "<<file_bytes<<endl;
                return;
            }

            shared_ptr<RegularFile> newRF = make_shared<RegularFile>(params[1],currentDir,file_bytes);
            char byte;

            while(input.get(byte)){
                newRF->setData(byte);
            }

            input.close();
            currentDir->add_data(newRF);
            currentDir->setSize(currentDir->calculateSIZE());
        }
    }else if(input == "rmdir"){
        if(params[0].find('/') != string::npos){
            operate("cd",params);
            vector<string>token  = tokenize_path(params[0]);
            for(auto x : currentDir->getContents()){
                if(x->getName() == *(token.end() -1 )){
                    if(x->getType() == 'F'){
                        cerr<<"Cant remove a regular file with rmdir command! Use rm instead."<<endl;
                        return;
                    }
                }
            }
            currentDir->rm(*(token.end()-1));
        }else{
            shared_ptr<Directory> dk;
            for(auto x : currentDir->getContents()){
                if(x->getName() == params[0]){
                    if(x->getType() == 'F'){
                        cerr<<"Cant remove a regular file with rmdir command! Use rm instead."<<endl;
                        return;
                    }
                }
            }

            currentDir->rm(params[0]);
        }
        removeInvalidLinkers(root);
        currentDir->setSize(currentDir->calculateSIZE());
    }
    else if(input == "rm"){
        //currentDir->rm(params[0]);
        if(params[0].find('/') != string::npos){
            operate("cd",params);
            vector<string>token  = tokenize_path(params[0]);
            for(auto x : currentDir->getContents()){
                if(x->getName() == *(token.end() -1 )){
                    if(x->getType() == 'D'){
                        cerr<<"Cant remove directory file with rm command! Use rmdir instead."<<endl;
                        return;
                    }
                }
            }
            currentDir->rm(*(token.end()-1));
        }else{
            for(auto x : currentDir->getContents()){
                if(x->getName() == params[0]){
                    if(x->getType() == 'D'){
                        cerr<<"Cant remove directory files with rm command! Use rmdir instead."<<endl;
                        return;
                    }
                }
            }
            currentDir->rm(params[0]);
        }
        removeInvalidLinkers(root);
        currentDir->setSize(currentDir->calculateSIZE());
    }
    else if(input == "cd"){
        
        vector<string> tokenizedpp  = tokenize_path(params[0]);
        shared_ptr<Directory> temp = currentDir;
        shared_ptr<File> t;
        bool exists = false;
        
        if(params[0] == ".."){
            if(currentDir->getParent() == nullptr){
                cerr<<"Cant go backwards from root."<<endl;
            }else{
                temp = currentDir->getParent();
            }
            exists = true;
        }else if(params[0] == "."){
            exists = true;
        }
        
        if(exists == false){
            
        for(unsigned int i= 0; i<tokenizedpp.size();i++){
            exists = false;
            if(tokenizedpp[i] == ".."){
                if(temp->getParent() != nullptr){
                    temp = temp->getParent();
                    exists = true;
                    continue;
                }
            }else if(tokenizedpp[i] == "."){
                exists = true;
                continue;
            }
            for(auto x:temp->getContents()){
                if(x->getName() == tokenizedpp[i]){
                    exists = true;
                    t = x;
                }
            }
            if(exists){
                if(dynamic_pointer_cast<Directory>(t)){
                    temp = dynamic_pointer_cast<Directory>(t);
                }else if(auto k = dynamic_pointer_cast<LinkerFile>(t)){
                    if(dynamic_pointer_cast<Directory>(k->getLinked())){
                        temp = dynamic_pointer_cast<Directory>(k->getLinked());
                        std::cout<<"Change the directory to linked directory!"<<endl;
                    }
                }
                else{
                    cerr<<"File "<<t->getName()<< " is not a directory!"<<endl;
                    break;
                }
            }
        }
        }

        if(exists){
            currentDir = temp;
            change_Path();
        }else{
            cerr<<"Cant find the directory "<<endl;
        }
        return; 
        
    }else if(input == "link"){
        bool find = false;
        for(auto x: currentDir->getContents()){
            if(x->getName() == params[1]){
                params[1] = params[1] + "" + to_string(rand() % 25);
                break;
            }
        }
        if(params[0].find('/') != string::npos){
            vector<string> tokens = tokenize_path(params[0]);
            shared_ptr<Directory> temp = currentDir;
            vector<string > vec;
            string tempname = *(tokens.end()-1);
            for(long unsigned int i = 0 ; i<tokens.size()-1;i++){
                vec.push_back(tokens[i]);
                operate("cd",vec);
                vec.pop_back();
            }

            shared_ptr<File> te = nullptr;
            for(auto x: currentDir->getContents()){
                if(x->getName() == tempname){
                    //find
                    find = true;
                    te = x;
                    break;

                }
            }

            
            if(find){
                currentDir = temp;
                shared_ptr<LinkerFile> newL = make_shared<LinkerFile>(params[1],currentDir,te,24);
                currentDir->add_data(newL);
                currentDir->calculateSIZE();

                currentDir  = temp;
                change_Path();
                return;
            }else{
                cerr<<"Link file cannot find!"<<endl;
                currentDir = temp;
                change_Path();
                return;
            }

            
        }else{
            shared_ptr<File> mu = nullptr;
            for(auto x : currentDir->getContents()){
                if(x->getName() == params[0]){
                    find = true;
                    mu = x;
                    break;
                }
            }

            if(find){
                shared_ptr<LinkerFile> newL = make_shared<LinkerFile>(params[1],currentDir,mu,24);
                currentDir->add_data(newL);
                currentDir->calculateSIZE();
                return;
            }else{
                cerr<<"Cant find linked file."<<endl;
                return;
            }
        }
    }

    root->setSize(root->calculateSIZE());
    std::cout<<"Root size = "<<root->getSize()<<endl;

}
void OS::removeInvalidLinkers(const shared_ptr<File>& file){
    if(auto linkerfile = dynamic_pointer_cast<LinkerFile>(file)){
        if(linkerfile->getLinked() && linkerfile->getLinked()->getParent() == nullptr){
            currentDir->rm(linkerfile->getName());
            return;
        }
    }

    if(auto dir = dynamic_pointer_cast<Directory>(file)){
        for(const auto& con : dir->getContents()){
            removeInvalidLinkers(con);
        }
    }
}
void OS::bash(){bool errorflag = false;
    string input;
    vector<string> parameters;
	
    do
    {

        std::cout<<current_path<<">> ";
	    do{getline(cin,input);}while(input.empty());        
        vector<string> tokens = tokenize_string(input);
        if(!is_op_correct(tokens[0])){
            cerr<<"Incorrect operand!"<<endl;
            errorflag = true;
            continue;
        }
        else{
            input = tokens[0];
            tokens.erase(tokens.begin());
            errorflag = false;
        }
            if(input == "exit"){

            }else if(input == "rmdir"){
                if(tokens.size() != 1){
                    cerr<<"Incorrect parameters for rmdir command!"<<endl;
                    errorflag= true;
                    continue;
                }else{
                    parameters.push_back(tokens[0]);
                }
            }
            else if(input == "ls"){
            if(tokens.size() > 1 || (tokens.size() == 1 && tokens[0] != "-R")){
                cerr<<"Incorrect parameters for ls command!"<<endl;
                errorflag = true;
                continue;
            }else{
                parameters.push_back(tokens[0]);
            }
            }else if(input == "mkdir"){
            if(tokens.size() != 1 ){
                cerr<<"Incorrect parameters for mkdir command!"<<endl;
                errorflag = true;
                continue;
            }else{
                parameters.push_back(tokens[0]);
            }
            }else if(input == "rm"){
            if(tokens.size() != 1){
                cerr<<"Incorrect parameters for rm command!"<<endl;
                errorflag = true;
                continue;
            }else{
                parameters.push_back(tokens[0]);
            }
            }else if(input == "cp"){
            if(tokens.size() != 2){
                cerr<<"Incorrect parameters for cp command!"<<endl;
                errorflag = true;
                continue;
            }else{
                for(auto x=tokens.begin();x!=tokens.end();x++){
                    parameters.push_back(*x);
                }
            }
            }else if(input == "link"){
            if(tokens.size() != 2){
                cerr<<"Incorrect parameters for link command!"<<endl;
                errorflag = true;
                continue;
            }else{
                for(auto x=tokens.begin();x!=tokens.end();x++){
                    parameters.push_back(*x);
                }
            }
            }else if(input == "cd"){
            if(tokens.size() != 1){
                cerr<<"Incorrect parameters for cd command!"<<endl;
                errorflag = true;
                continue;
            }else{
                parameters.push_back(tokens[0]);
            }

            }else if(input == "cat"){
            if(tokens.size() != 1){
                cerr<<"Incorrect parameters for cd command!"<<endl;
                errorflag = true;
                continue;
            }else{
                parameters.push_back(tokens[0]);
            }
            }
        } while (errorflag);
    
    operate(input,parameters);
    }
