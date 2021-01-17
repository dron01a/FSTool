#include "FSTbase.h"

bool FSTool::exists(std::string path){
#ifdef WIN32
	if (_access(path.c_str(), 0)){}
#elif defined(unix) 
    if (access(path.c_str(), 0) !=0){
#endif
		return false; // if not exists 
    }
    return true; // if path found 
}

FSTool::_base::_base(std::string name, std::string path){
    this->_updateFullName(path,name);
}

FSTool::_base::_base(std::string name){
    this->_fullName = name;
    int *find = new int(this->_fullName.find_last_of("/\\")); // split into file name and path
    if(*find !=std::string::npos){
        this->_path = this->_fullName.substr(0, *find);  // set path
        this->_name = this->_fullName.substr(*find + 1); // set file name
    }
    else{
        this->_name = name;
    }
    delete find; // free memory 
}

void FSTool::_base::_updateFullName(std::string path, std::string name){
    std::string *_path = new std::string(path); // temporary strings
    std::string *_name = new std::string(name); // to path, name and full name
    std::string *_fullName = new std::string;
#ifdef WIN32
	if(path[path.length() -1] != '\\'){
		*_fullName = path + '\\' + name;
		*_path += '\\';
	}
#elif defined(unix) 
    if(path[path.length() -1] != '/'){
		*_fullName = path + '/' + name;
		*_path += '/';
	}
#endif
    else{
		*_fullName = path + name;
	}
    this->_fullName = *_fullName;
    this->_name = *_name;
    this->_path = *_path;
    delete _path; // free memory
    delete _name;
    delete _fullName;
}

int FSTool::_base::size(){
    return _size;
}

std::string FSTool::_base::name(){
    return _name;
}

std::string FSTool::_base::full_name(){
    return _fullName;
}

std::string FSTool::_base::path(){
    return _path;
}

tm* FSTool::_base::last_modification(){
    return _lmTime; 
}

void FSTool::_base::rename(std::string newName){
    int *res = new int(std::rename(_fullName.c_str(), newName.c_str()));
    if(*res != 0){
        if(*res == ENOENT){
            throw FSTool::fs_exception("not found", -1);
        }
        throw FSTool::fs_exception("renaming error", -11);
    }
    this->_updateFullName(this->_path,newName);
    delete res;
}

std::string FSTool::_base::front(){ 
    return this->get(0); 
}

std::string FSTool::_base::at(int index) {
    if ( this->range(index) ){
        return this->get(index);
    }
    throw FSTool::fs_exception("not valid index", -3); 
}

void FSTool::_base::move(std::string path){
    if(!FSTool::exists(path)){
        throw FSTool::fs_exception(path + " not found", -1);
    }
    int *res = new int(std::rename(_fullName.c_str(), path.c_str()));
    if(*res != 0){
        if(*res == ENOENT){
            throw FSTool::fs_exception("not found", -1);
        }
        throw FSTool::fs_exception("renaming error", -11);
    }
    this->_updateFullName(path,this->_name);
    delete res;
}

bool FSTool::_base::exists(){
    return FSTool::exists(_fullName); // return result of FSTool::exists(std::string)
}

bool FSTool::operator==(FSTool::_base & objectA, FSTool::_base & objectB){
    return objectA.size() == objectB.size(); 
}

bool FSTool::operator>=(FSTool::_base & objectA, FSTool::_base & objectB){
    return objectA.size() >= objectB.size(); 
}

bool FSTool::operator<=(FSTool::_base & objectA, FSTool::_base & objectB){
    return objectA.size() <= objectB.size(); 
}

bool FSTool::operator!=(FSTool::_base & objectA, FSTool::_base & objectB){
    return objectA.size() != objectB.size(); 
}

bool FSTool::operator<(FSTool::_base & objectA, FSTool::_base & objectB){
    return objectA.size() < objectB.size(); 
}

bool FSTool::operator>(FSTool::_base & objectA, FSTool::_base & objectB){
    return objectA.size() > objectB.size(); 
}