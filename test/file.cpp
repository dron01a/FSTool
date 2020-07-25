#include "file.h"

FSTool::_finfo::_finfo(std::string full_name){
    this->full_name = full_name;
    std::fstream *obj = new std::fstream(this->full_name);
    if (!obj->is_open()){
        obj->close();
        return;
    }
    obj->open(this->full_name, std::fstream::out | std::fstream::in | std::fstream::binary);
    int *find = new int(this->full_name.find_last_of("/\\")); // split into file name and path
    this->path = this->full_name.substr(0, *find);            // set path
    this->name = this->full_name.substr(*find + 1);           // set file name
    delete find;                                              // free memory
#ifdef unix
    struct stat data;
    stat(this->full_name.c_str(), &data);
#elif defined(WIN32)
    struct _stat data;
    _stat(this->full_name.c_str(), &data);
#endif
    this->size = data.st_size;
    std::string *buf = new std::string; //temporary string for getline
    while (getline(*obj, *buf)){
        this->lines++;
    }
    obj->close(); // close file
    delete buf;
    delete obj;
    this->last_modification = data.st_mtime;
}

FSTool::_finfo FSTool::file_information(std::string file_name){
    return _finfo(file_name);
}  

// FSTool::file::file(std::string name, std::string path){
//     std::string *temp_path = new std::string(path); // temporary strings
//     std::string *temp_name = new std::string(name); // to path, name and full name
//     std::string *temp_fullname = new std::string;
// #ifdef unix 
// 	if(path[path.length() -1] != '/'){
// 		*temp_fullname = path + '/' + name;
// 		*temp_path += '/'; // redact str
// #elif defined(WIN32)
//     if(path[path.length() -1] != '\\'){
// 		*temp_fullname = path + '\\' + name;
// 		*temp_path += '\\';
// #endif
// 	}else{
// 		*temp_fullname = path + name;
// 	} 
//     delete temp_path; // free memory
//     delete temp_path;
//     _info = new _finfo(*temp_fullname); // full information struct
//     delete temp_fullname; 
// }

// FSTool::file::~file(){
//     delete _info;
// }