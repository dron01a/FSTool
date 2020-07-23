#include "file.h"

FSTool::_finfo::_finfo(std::string full_name){
    this->full_name = full_name;
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
    std::ifstream *obj = new std::ifstream(this->full_name, std::fstream::binary | std::fstream::in | std::fstream::app);
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