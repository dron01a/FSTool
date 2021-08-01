#include "FSTool.h"

#ifdef WIN32
#define stat _stat
#define access _access;
#endif

FSTool::FST_object* FSTool::open(std::string name){
    if(exists(name)){
        if(is_file(name)){
            return new file(name);
        }
        else if(is_folder){
            return new folder(name);
        }
        else{
            throw fs_exception("error type",-1);
        }
    }
    throw fs_exception("not found", -1); // if file folder not exists
}

bool FSTool::is_file(std::string file_name){
#if defined(WIN32)
    struct _stat fl_stat;
    if(_stat(file_name.c_str(), &fl_stat) == 0){
        if(fl_stat.st_mode & __S_IFREG){
            return true;
        }
    }
#else
    struct stat fl_stat;
    if(stat(file_name.c_str(), &fl_stat) == 0){
        if(S_ISREG(fl_stat.st_mode)){
            return true;
        }
    }
#endif
    return false;
}

bool FSTool::is_folder(std::string file_name){
#if defined(WIN32)
    struct _stat fl_stat;
    if(_stat(file_name.c_str(), &fl_stat) == 0){
        if(fl_stat.st_mode & __S_IFDIR){
            return true;
        }
    }
#else
    struct stat fl_stat;
    if(stat(file_name.c_str(), &fl_stat) == 0){
        if(S_ISDIR(fl_stat.st_mode)){
            return true;
        }
    }
#endif
    return false;
}

int FSTool::find(std::string name, std::string object, int begin, int end){
    static std::string _name; // name of object 
    if(_name != name ){
         _name = name;
    }
    FST_object *temp;
    if(is_file(name)){
        temp = new file(name);
    }
    if(is_folder(name)){
        temp = new folder(name);
    }
    int result = temp->find(object,begin,end);
    delete temp;
    return result;
}

FSTool::FSToken::FSToken(std::string name){
    if (is_folder(name)){   
        folder * temp = new FSTool::folder(name); // temp object as folder
        this-> _type = "folder"; // set type;
        this->_size = temp->size(); // set size
        this->_name = temp->name();
        for(int i = 0; i != temp->lenght(); i++){
            if(strcmp(temp->get(i).c_str(),".") == 0 || strcmp(temp->get(i).c_str(),"..") == 0){
                continue; // skip .. and . 
            }
            _subNodes.push_back(FSTool::FSToken(name + "/" + temp->get(i)));
        }
        delete temp; 
    }
    else{
        file * temp = new FSTool::file(name); // temp object as file
        this->_name = temp->name();
        this-> _type = "file"; // set type;
        this->_size = temp->size(); // set size
        delete temp; 
    }
    this->_fullName = name; 
}

FSTool::FSToken::~FSToken(){
    _subNodes.clear();
}

int FSTool::FSToken::countNodes(){
    return _subNodes.size();
}

int FSTool::FSToken::size(){
    return _size;
}

FSTool::FSToken FSTool::FSToken::get(int index){
    return _subNodes[index];
}

std::string FSTool::FSToken::type(){
    return _type;
}

std::string FSTool::FSToken::name(){
    return _name;
}

std::string FSTool::FSToken::full_name(){
    return _fullName;
}

bool FSTool::FSToken::have(std::string name){
    for (int i = 0; i < _subNodes.size(); i++){
        if(_subNodes[i].name() == name){
            return true;
        }
        if(is_folder(_subNodes[i].full_name())){
            if (_subNodes[i].have(name) == false){
                continue;
            }
            return true;
        }
    }
    return false;
}

std::string FSTool::FSToken::getPath(std::string object){
    std::string result;
    if(!have(object)){
        throw fs_exception("not found", 1);
    } 
    for (int i = 0; i <  _subNodes.size(); i++){
        if(_subNodes[i].name() == object){
            result = _subNodes[i].full_name();
            break; 
        }
        if(_subNodes[i].type() == "folder"){
            if(_subNodes[i].have(object)){
               result = _subNodes[i].getPath(object);
               break;
            }
            continue;
        }
    }
    return result;
}

FSTool::FST_object* FSTool::FSToken::getObject(int index){
    if(_subNodes[index].type() == "file"){
        return new FSTool::file(_subNodes[index].full_name());
    }
    if(_subNodes[index].type() == "folder"){
        return new FSTool::folder(_subNodes[index].full_name());
    }
}

int FSTool::FSToken::count(std::string name, bool allEnt){
    int result = 0;
    if(this->have(name)){
        for(int count = 0; count < _subNodes.size(); count++){
            if(allEnt){
                if(_subNodes[count].name().find(name) == std::string::npos){
                    result++;
                }
            }
            else{
                if(_subNodes[count].name() == name){
                    result++;
                }
            }
            if(_subNodes[count].type() == "folder"){
                result += _subNodes[count].count(name);
            }
        }
    }
    return result;
}
