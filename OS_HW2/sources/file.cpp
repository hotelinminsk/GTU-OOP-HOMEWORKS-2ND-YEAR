#include "../libs/file.h"

int File::id_count = 0;
const int File::getParentUniq()const{return parent->getUnique();}
//DIRECTORY FULLY IMPLEMENTED
Directory::Directory(string _n,shared_ptr<Directory> parentF,char t,size_t siz):File(_n,t,parentF,siz){

}

size_t Directory::calculateSIZE(){
    this->setSize(0);
    
    size_t samplesize = 0;

    for(auto x:this->contents){
        if(dynamic_pointer_cast<Directory>(x)){
            samplesize += (dynamic_pointer_cast<Directory>(x))->calculateSIZE();
        }else{
            samplesize += x->getSize();
        }
    }
    samplesize += 1024;

    // Update the size of the directory
    this->setSize(samplesize);

    return samplesize;
}

Directory::Iterator Directory::begin()const{
    return Iterator(*this,0);
}
Directory::Iterator Directory::end()const{
    return Iterator(*this,this->getContents().size());
}

void Directory::add_data(shared_ptr<File> newF){
    this->contents.push_back(newF);
    this->calculateSIZE();
}
void Directory::rm(string name){
    auto temp = contents.end();
    for (auto a = contents.begin(); a != contents.end(); ++a) {
        if ((*a)->getName() == name) {
            temp = a;
            break;
        }
    }

    if (temp != contents.end()) {
        (*temp)->setParentPTR(nullptr);
        (*temp)->rm();
        contents.erase(temp);
        this->calculateSIZE();
    } else {
        cerr << "Cant find the file to remove!" << endl;
    }
}
Directory::Directory(const Directory& other):File(other.getName(),'D',other.getParent(),other.getSize()){
    for(auto a:other.contents){
        this->add_data(a);
    }
}
Directory::~Directory(){
    contents.erase(contents.begin(),contents.end());
}

//REGULAR FILE

RegularFile::RegularFile(string _n,shared_ptr<Directory> parentF,size_t size1):
File(_n,'F',parentF,size1)
{

}
RegularFile::RegularFile(const RegularFile& ot):File(ot.getName(),'F',ot.getParent(),ot.getSize()){
    for(auto a:ot.data){
        this->data.push_back(a);
    }
}
RegularFile::~RegularFile(){
    
}

void RegularFile::setData(char data){
    this->data.push_back(data);
}

RegularFile::Iterator RegularFile::begin()const{
    return  Iterator(*this,0);
}
RegularFile::Iterator RegularFile::end()const{
    return Iterator(*this,data.size());
}


//LINKER FILE

LinkerFile::LinkerFile(string _n,shared_ptr<Directory >parentF,shared_ptr<File> lf,size_t st):File(_n,'L',parentF,24){
    this->linked_file = lf;
    if(linked_file != nullptr){
        linked_file_unique = linked_file->getUnique();
    }else{
        linked_file_unique = -5;
    }

}
void LinkerFile::del(){
    this->setParentPTR(nullptr);

}
void LinkerFile::cat()const{
    if(linked_file->getParent() == nullptr){
        //wait

    }else{
        cout<<"This file softly linked to file -> "<<linked_file->getName()<<endl;
        linked_file->cat();
    }
}
void LinkerFile::ls()const{

}

LinkerFile::~LinkerFile(){

    linked_file.reset();
}

LinkerFile::LinkerFile(const LinkerFile& other):File(other.getName(),'L',nullptr,other.getSize()){
    linked_file_unique = other.linked_file_unique;
    linked_file = other.linked_file;
}
