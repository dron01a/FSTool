#include "file.h"

FSTool::_finfo::_finfo(std::string full_name){
    this->full_name = full_name;
    std::fstream *obj = new std::fstream(this->full_name);
    if (!obj->is_open()){
        obj->close();
        return;
    }
    std::fstream *obj = new std::fstream(this->full_name, std::ios::out | std::ios::in | std::ios::binary);
    int *find = new int(this->full_name.find_last_of("/\\")); // split into file name and path
    this->path = this->full_name.substr(0, *find);            // set path
    this->name = this->full_name.substr(*find + 1);           // set file name
    delete find;                                              // free memory
    int *exp = new int(this->full_name.find_first_of("."));   // find "." to set extension of file
    this->type = this->full_name.substr(*exp + 1);
    delete exp;                                               // free memory 
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
    return _finfo(file_name);// return struct
}  

FSTool::file::file(std::string name, std::string path){
    std::string *temp_path = new std::string(path); // temporary strings
    std::string *temp_name = new std::string(name); // to path, name and full name
    std::string *temp_fullname = new std::string;
#ifdef unix 
	if(path[path.length() -1] != '/'){
		*temp_fullname = path + '/' + name;
		*temp_path += '/'; // redact str
#elif defined(WIN32)
    if(path[path.length() -1] != '\\'){
		*temp_fullname = path + '\\' + name;
		*temp_path += '\\';
#endif
	}else{
		*temp_fullname = path + name;
	} 
    delete temp_path; // free memory
    delete temp_path;
    _info = new _finfo(*temp_fullname); // full information struct
    delete temp_fullname; 
}

FSTool::file::~file(){
    delete _info;
}

bool FSTool::file::exists(){
    std::fstream *temp = new std::fstream(this->_info->full_name); // create temp object 
    if( !temp->is_open() ) {
        temp->close(); // close stream
        delete temp;   // free memory
        return false;
    }else{
        temp->close(); // close stream
        delete temp;   // free memory
        return true; 
    }
}

int FSTool::file::create(){
    std::ofstream *temp = new std::ofstream(this->_info->full_name, std::fstream::binary); // create temp object 
    try{
        if (!temp->is_open()) {
            throw 1;
        }else{
            throw 0;
        }
    }
    catch( int code ){
        temp->close(); // close stream
        delete temp;   // free memory
        return code;   // return result
    }
}

int FSTool::file::destroy(){
    try{
        if(remove(this->_info->full_name.c_str())){
            throw 1;// if can`t remove file 
        }
        else if(this->exists()){
            throw -1;// if file exists 
        }
        else{
            throw 0;// if good 
        }
    }
    catch(int code){
        return code;// return result 
    }
}

bool FSTool::file::empty(){
    if((this->_info->size == 0) && (this->_info->lines == 0)){
        return true; // if file empty 
    }
    else{
        return false;
    } 
}

FSTool::_finfo FSTool::file::get_info(){
    return *this->_info; // return struct with info of file
}

std::string FSTool::file::get(int index){
    std::fstream * object = new std::fstream(this->_info->full_name, std::fstream::out | std::fstream::in | std::fstream::binary); 
	std::string buf; //result
	int* i = new int(0); //temporary counter
	while (getline(*object, buf)) {//find index
		if (*i == index)
			break;
		(*i)++;
	}
	object->close();//close file
	delete i;
    delete object;
	return buf;
}

std::string FSTool::file::back(){
    return this->get(this->_info->lines--);// return lasr line  
}

bool FSTool::file::range(int index){
    try{
        if(index < 0){
            throw -1; // if out of range 
        }  
        else if(index > this->_info->lines){
            throw 1; // if out of range 
        }
        else{
            throw 0; // if good 
        }  
    }
    catch(int result){
        return result;
    }
}

