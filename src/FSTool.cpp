#include "FSTool.h"

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
    _base *temp;
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