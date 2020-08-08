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
    delete temp_name;
    _info = new _finfo(*temp_fullname); // full information struct
    delete temp_fullname; 
}

FSTool::file::~file(){
    //delete _info;
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

int FSTool::file::resize(){
#ifdef unix
    struct stat rdata;
    stat(this->_info->full_name.c_str(), &rdata);
#elif defined(WIN32)
    struct _stat rdata;
    _stat(this->_info->full_name.c_str(), &rdata);
#endif
    return rdata.st_size;
}

int FSTool::file::create(){
    std::ofstream *temp = new std::ofstream(this->_info->full_name, std::fstream::binary); // create temp object
    if (!temp->is_open()){
        temp->close(); // close stream
        delete temp;   // free memory
        return 1;      // return code 
    }
    else{
        temp->close(); // close stream
        delete temp;   // free memory
        this->_info = new _finfo(this->_info->full_name); // full information struct
        return 0;      // return code 
    }
}

int FSTool::file::destroy(){
    if(!this->exists()){
        return -1;// if file exists 
    }
    return remove(this->_info->full_name.c_str()); // return result of deleting file
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
    if (index < 0 || index > this->_info->lines){
        return false; // if index not in range
    }
    else{
        return true; // if index in range
    }
}

int FSTool::file::add(std::string data){
    if (!this->exists()){
        return -1;  // file exists 
    }
    std::fstream *obj; // temp object 
    obj = new std::fstream(this->_info->full_name, std::fstream::app | std::fstream::binary);
    *obj << data << std::endl; // write
    obj->close();              // save and close stream 
    delete obj;                // free memory 
    this->_info->lines++;
    return 0;
}

int FSTool::file::add(std::string data, int index){
    try{
        if (!this->exists()){
            throw -1; // if file exists
        }
        if (index >= this->_info->lines){
            throw 1; 
        }
        std::string *_buff = new std::string[this->_info->lines]; // temp buffeer
        for(int i = 0; i < this->_info->lines; i++){
            _buff[i] = this->get(i); // load file data to buff 
        }
        _buff[index] = data; // rewrite line 
        this->clear(); // delete data in file
        for(int i = 0; i < this->_info->lines; i++){
            this->add(_buff[i]); // load buf to file  
        }
        delete _buff;
        this->_info->lines++;
        this->_info->size = resize();  // get new size from bites of file 
        return 0; 
    }
    catch(int error_code){
        return error_code;
    }
}

int FSTool::file::insert(std::string data, int index){
    try{
        if (!this->exists()){
            throw -1; // if file exists
        }
        if (index > this->_info->lines){
			throw 1; // index is out of bounds of file
		}
        std::string *_fdata = new std::string[this->_info->lines]; //buffer
        for (int i = 0; i < this->_info->lines; i++) { // load data in file to array
            _fdata[i] = this->get(i);
        }
        int * lines = new int(this->_info->lines); // temp count lines  
        this->clear();
        for (int i = 0; i < *lines; i++){
            if (i == index ){
                this->add(data);// add data 
            }
            this->add(_fdata[i]);
        }
        delete[] _fdata;
        delete lines;
        this->_info->lines++;
        this->_info->size = resize(); // get new size from bites of file 
        return 0;
    }
    catch (int result){
		return result;
	}
}

int FSTool::file::insert(std::string data, int index, int count){
    try{
        if (!this->exists()){
            throw -1; // if file exists
        }
        if (index > this->_info->lines){
			throw 1; // index is out of bounds of file
		}
        std::string *_fdata = new std::string[this->_info->lines]; //buffer
        for (int i = 0; i < this->_info->lines; i++) { // load data in file to array
            _fdata[i] = this->get(i);
        }
        int * lines = new int(this->_info->lines);  // temp count lines
        this->clear();
        for (int i = 0; i < *lines; i++){
            if (i == index ){
                for (int c = 0; c < count; c++){
                    this->add(data); // add data
                } 
            }
            this->add(_fdata[i]);
        }
        delete lines;
        delete[] _fdata;
        this->_info->lines++;
        this->_info->size = resize(); // get new size from bites of file 
        return 0;
    }
    catch (int result){
		return result;
	}
}

void FSTool::file::clear(){
    std::ofstream *temp = new std::ofstream(this->_info->full_name); // temp object
    this->_info->lines = 0;
    this->_info->size = 0;
    delete temp;
}

int FSTool::file::rename_file(std::string new_name){
    if (!this->exists()){
        return 1; // if file not exists 
    }
    std::string * temp_fullname = new std::string; // temp string 
#ifdef unix 
	if(_info->path[_info->path.length() -1] != '/'){
		*temp_fullname = _info->path + '/' + new_name;
#elif defined(WIN32)
    if(path[path.length() -1] != '\\'){
        *temp_fullname = _info->path + '\\' + new_name;		
#endif
    }
    else{
        *temp_fullname = this->_info->path + new_name;
    }
    rename(this->_info->full_name.c_str(), temp_fullname->c_str());
    this->_info->name = new_name;
    this->_info->full_name = *temp_fullname;
    delete temp_fullname;
    return 0;
}

void FSTool::file::copy(file &source){
    std::ifstream *src; // temp input
    std::ofstream *out; // temp output
    src = new std::ifstream(source.get_info().full_name, std::ios::binary); // open input file
    out = new std::ofstream(this->_info->full_name, std::ios::binary); // open source file 
    *out << src->rdbuf(); // write data
    src->close(); // close streams 
	out->close();
    delete src; // free memory
    delete out;
}

void FSTool::file::copy(std::string name){
    std::ifstream *src; // temp input
    std::ofstream *out; // temp output
    src = new std::ifstream(name, std::ios::binary); // open input file
    out = new std::ofstream(this->_info->full_name, std::ios::binary); // open source file 
    *out << src->rdbuf(); // write data
    src->close(); // close streams 
	out->close();
    delete src; // free memory
    delete out;
}

void FSTool::file::move(std::string path){
    FSTool::file * temp = new FSTool::file(this->_info->name, path);  // temp file object
    if(!temp->exists()){
        temp->create();
    }
    temp->copy(this->_info->full_name); // clone file   
    this->destroy(); // delete file 
    this->_info->path = path;
    this->_info->full_name = temp->get_info().full_name;
    delete temp;
}