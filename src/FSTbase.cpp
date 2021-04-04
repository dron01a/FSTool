#include "FSTbase.h"

#ifdef WIN32
#define stat _stat
#define access _access;
#endif

bool FSTool::exists(std::string path){ 
    if (access(path.c_str(), 0) != 0){
		return false; // if not exists 
    }
    return true; // if path found 
}

FSTool::FST_object::FST_object(std::string name, std::string path){
    _updateFullName(path,name);
}

FSTool::FST_object::FST_object(std::string name){
    _fullName = name;
    int *find = new int(_fullName.find_last_of("/\\")); // split into file name and path
    if(*find !=std::string::npos){
        _path = _fullName.substr(0, *find);  // set path
        _name = _fullName.substr(*find + 1); // set file name
    }
    else{
        _name = name;
    }
    delete find; // free memory 
}

void FSTool::FST_object::_updateFullName(std::string path, std::string name){
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

int FSTool::FST_object::size(){
    return _size;
}

std::string FSTool::FST_object::name(){
    return _name;
}

std::string FSTool::FST_object::full_name(){
    return _fullName;
}

std::string FSTool::FST_object::path(){
    return _path;
}

tm* FSTool::FST_object::last_modification(){
    return _lmTime; 
}

void FSTool::FST_object::rename(std::string newName){
    int *res = new int(std::rename(_fullName.c_str(), newName.c_str())); // get result of rename
    if(*res != 0){
        if(*res == ENOENT){
            throw FSTool::fs_exception("not found", -1); 
        }
        throw FSTool::fs_exception("renaming error", -11);
    }
    _updateFullName(_path,newName);
    delete res;
}

std::string FSTool::FST_object::front(){ 
    return get(0); //return first element
}

std::string FSTool::FST_object::at(int index) {
    if (range(index)){
        return get(index); // return element from index 
    }
    throw FSTool::fs_exception("not valid index", -3); // get exceprion 
}

FSTool::strvect FSTool::FST_object::pathSteps(){
    strvect elements; 
    std::string *temp = new std::string;
    char* token, * next_token = NULL;
    if(_fullName.find("/") != std::string::npos){
#ifdef WIN32
        char p[1024];
	    strcpy_s(p, _info->full_name.c_str());
	    token = strtok_s(p, "\\", &next_token);
        elements.push_back(token);
	    for (int i = 0; token != NULL; token = strtok_s(NULL, "\\", &next_token), i++){
            *temp = elements[i-1] + "\\" + token;
#elif defined(unix)
        char nameBuff[_fullName.length()];
        strcpy(nameBuff, _fullName.c_str());
        token = strtok(nameBuff, "/");
        elements.push_back(token);
        token = strtok(NULL, "/");
        for (int i = 1; token != NULL; token = strtok(NULL, "/"), i++){
            *temp = elements[i-1] + "/" + token;
#endif
	    	elements.push_back(*temp);
        }
	    delete token; 
        delete next_token;
        delete temp;
    }
    else{
        elements.push_back(_fullName);
    }
    return elements;
}

bool FSTool::FST_object::exists(){
    return FSTool::exists(_fullName); // return result of FSTool::exists(std::string)
}

bool FSTool::operator==(FSTool::FST_object & objectA, FSTool::FST_object & objectB){
    return objectA.size() == objectB.size(); 
}

bool FSTool::operator>=(FSTool::FST_object & objectA, FSTool::FST_object & objectB){
    return objectA.size() >= objectB.size(); 
}

bool FSTool::operator<=(FSTool::FST_object & objectA, FSTool::FST_object & objectB){
    return objectA.size() <= objectB.size(); 
}

bool FSTool::operator!=(FSTool::FST_object & objectA, FSTool::FST_object & objectB){
    return objectA.size() != objectB.size(); 
}

bool FSTool::operator<(FSTool::FST_object & objectA, FSTool::FST_object & objectB){
    return objectA.size() < objectB.size(); 
}

bool FSTool::operator>(FSTool::FST_object & objectA, FSTool::FST_object & objectB){
    return objectA.size() > objectB.size(); 
}