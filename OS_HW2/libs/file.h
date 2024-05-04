#ifndef FILE_H
#define FILE_H
#include <iostream>
#include <vector>
#include <ctime>
#include <memory>

using namespace std;
class Directory;

class File{//all implemented
    private:
    string name;
    size_t size;
    string created_dt;
    char type;
    shared_ptr<Directory> parent;
    int uniqiue_id;
    
    

    public:
        void setParentPTR(const shared_ptr<Directory> p){parent = p;}
        static void inc_id(){
            id_count = id_count +1 ;
        }
       
        File(string _n,char t,shared_ptr<Directory>parentF=nullptr,size_t size_send=1024):name(_n),size(size_send),type(t),parent(parentF){
            inc_id();
            uniqiue_id = File::id_count;
            time_t currtime = time(nullptr);
            tm* timst = localtime(&currtime);

            char buffer[80];
            strftime(buffer,sizeof(buffer),"%m-%d %H:%M",timst);
            created_dt = buffer;
        }
        //normal members
        //SETTERS
        void setName(const string& n){name = n;}
        void setType(const char c){type = c;}
        void setSize(size_t se){size = se;}
        void setUniq(const int u){uniqiue_id = u;}
        void setdt(const string& dt){created_dt = dt;}
        //GETTERS
        const int& getUnique()const{return uniqiue_id;}
        const char& getType()const{return type;}
        const size_t& getSize()const{return size;}
        const int getParentUniq()const;

        string getName()const{return name;}
        string getDT()const{return created_dt;}
        shared_ptr<Directory> getParent()const{return parent;}

        //virtuals
        virtual void cat()const = 0;
        virtual void ls()const = 0;
        virtual ~File(){}
        virtual void rm() = 0;
        static int id_count;


};


class Directory : public File{
    private:
        vector<shared_ptr<File>>contents;
    public:
        vector<shared_ptr< File> >& getContents(){
            return contents;
        }
        vector<shared_ptr< File>>const& getContents()const{
            return contents;
        } 
        
        Directory(string n_,shared_ptr<Directory> parentF=nullptr,char t='D',size_t size=1024UL);

        void cat()const{
            this->ls();
        }
        void ls()const{
            for(auto a= begin();a!=end();a++){
                cout<<(*a).getType()<<" "<<(*a).getName()<<" "<< (*a).getDT()<<" size = "<<(*a).getSize()<<endl;
            }   

        }
        void lsr(shared_ptr<Directory> dir,string prefix="")const{
            for(auto x: dir->getContents()){
                cout<<prefix<<x->getType()<<" "<<x->getName()<<" "<<x->getDT() << " size = "<<x->getSize()<<endl;
                if(dynamic_pointer_cast<Directory>(x)){
                    lsr(dynamic_pointer_cast<Directory>(x),prefix + " ");
                }
            }
        }

        //ITERATOR
        class Iterator{
            private:
                int index;
                const Directory& dir;

            public:
                Iterator(const Directory& _dir,int _index=0):index(_index),dir(_dir){}
                Iterator& operator++(){
                    index++;
                    return *this;
                }
                Iterator operator++(int){
                    Iterator it1(dir,index);
                    index++;

                    return it1;
                }
                Iterator& operator--(){
                    if(index == 0){
                        cout<<"Dont try to decrement an iterator when its pointing to the first element.";
                        return *this;
                    }else{
                        index--;
                        return *this;
                    }
                }
                Iterator operator--(int){
                    if(index == 0){
                        cout<<"Dont try to decrement an iterator when its pointing to the first element.";
                        return *this;
                    }else{
                        Iterator t1(dir,index);
                        index--;
                        return t1;
                    }
                }

                bool operator!=(const Iterator& other){
                    return !(index == other.index);
                }

                File& operator*(){
                    return *dir.contents[index];
                }
        };

        ~Directory();

        Iterator begin()const; 
        Iterator end()const;
        Directory(const Directory& other);
        size_t calculateSIZE();
        void rm(string);
        void rm(){};
        
        void add_data(shared_ptr<File > newf);
        


    //FULLY IMPLEMENTED I THINK
};

class RegularFile:public File{
    private:
    vector<char> data;

    public:
    //ITERATOR
    class Iterator{
        private:
        int index;
        const RegularFile& file;
        public:
        Iterator(const RegularFile& fi,int _i=0):index(_i),file(fi){

        }
        Iterator& operator++(){
            index++;
            return *this;
        }
        Iterator operator++(int){
            Iterator it = Iterator(file,index);
            index++;
            return it;
        }
        Iterator& operator--(){
            if(index == 0){
                cout<<"Dont try to decrement an iterator when its pointing to the first element.";
                    return *this;
            }else{
                index--;
                return *this;
            }
        }
        Iterator operator--(int){
            if(index == 0){
                cout<<"Dont try to decrement an iterator when its pointing to the first element.";
                return *this;
            }else{
                Iterator t1(file,index);
                index--;
                return t1;
            }
        }

        bool operator!=(const Iterator& other){
        bool c = (index == other.index);
        return !c;
        }

        const char& operator*(){
            return file.data[index];
        }
    };  
    //members;
    public:
    void rm(){;}
    const vector<char>& getData()const {return data;}
    void cp();
    RegularFile(string,shared_ptr<Directory > parentF=nullptr,size_t st=1);
    ~RegularFile();
    RegularFile(const RegularFile& other);
    void setData(char);
    Iterator begin()const;
    Iterator end()const;
    void cat()const{
        for(auto a = begin(); a != end(); a++){
            cout<<*a;
        }
        cout<<"\n";
    }

    void ls()const{
        cout<<getName() <<" is not a directory."<<endl;
    }

};


class LinkerFile:public File{
    private:
        int linked_file_unique;
        shared_ptr<File> linked_file;
    public:
        //constructor and destroctor
        ~LinkerFile();
        LinkerFile(string _n,shared_ptr<Directory >parentF,shared_ptr<File >lf,size_t size = 0);
        LinkerFile(const LinkerFile& other);
        //members
        void set_lk_unique(int iq){linked_file_unique = iq;}
        
        const int getLinkedUnique()const{return linked_file_unique;}
        void del();
        void cat()const;
        void ls()const;
        void rm(){
            linked_file.reset();
        }

        void setLinked(shared_ptr<File>& linked){linked_file = linked;}
        const shared_ptr<File>& getLinked()const{return linked_file;}

        


};

#endif


