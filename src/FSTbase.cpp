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

FSTool::_base::_base(std::string name, std::string path){
    _updateFullName(path,name);
}

FSTool::_base::_base(std::string name){
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

std::string FSTool::_base::front(){ 
    return get(0); //return first element
}

std::string FSTool::_base::at(int index) {
    if (range(index)){
        return get(index); // return element from index 
    }
    throw FSTool::fs_exception("not valid index", -3); // get exceprion 
}

void FSTool::_base::move(std::string path){
    if(!FSTool::exists(path)){
        throw FSTool::fs_exception(path + " not found", -1); 
    }
    int *res = new int(std::rename(_fullName.c_str(), path.c_str())); // get result of rename
    if(*res != 0){
        if(*res == ENOENT){
            throw FSTool::fs_exception("not found", -1);
        }
        throw FSTool::fs_exception("renaming error", -11);
    }
    _updateFullName(path,_name);
    delete res;
}

FSTool::strvect FSTool::_base::pathSteps(){
    strvect elements; 
    std::string *temp = new std::string;
    char* token, * next_token = NULL;
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
    for (int i = 1; token != NULL; token = strtok(NULL, "/"), i++){
        *temp = elements[i-1] + "/" + token;
#endif
		elements.push_back(*temp);
    }
	delete token; 
    delete next_token;
    delete temp;
    return elements;
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