#include "file control.h"

//relizations functions for work with file system 

bool file_control::search(std::string object) {
#ifdef WIN32
	if (_access(object.c_str(), 0))
#elif defined(unix) 
    if (access(object.c_str(), 0))
#endif
		return false;
	else
		return true;
}

bool file_control::search(std::string directory, std::string object) {
#ifdef WIN32
	if (_access((directory + object).c_str(), 0))
#elif defined(unix) 
    if (access((directory + object).c_str(), 0))
#endif
		return false;
	else
		return true;
}

void file_control::get_elements_of_path(std::string source, std::string parts_of_string[]) {
	char* token, * next_token = NULL;
#ifdef WIN32
    char p[1024];
	strcpy_s(p, source.c_str());
	token = strtok_s(p, "\\", &next_token);
	for (int i = 0; token != NULL; token = strtok_s(NULL, "\\", &next_token), i++)
#elif defined(unix)
    char p[source.length()];
    strcpy(p, source.c_str());
    token = strtok(p, "/");
    for (int i = 0; token != NULL; token = strtok(NULL, "/"), i++)
#endif
		parts_of_string[i] = token;
	delete token, next_token;
}

void file_control::make_path(std::string path) {
	std::string* pfolder = new std::string[16], * paths = new std::string[16];
#ifdef WIN32
	get_elements_of_path(path, pfolder); 
	paths[0] = pfolder[0] + '\\';
#elif defined(unix)
    get_elements_of_path(path, pfolder);
	paths[0] = pfolder[0] + "/";
#endif
	for (int i = 1; i != 16; i++)// make steps of path
#ifdef WIN32
		paths[i] = paths[i - 1] + pfolder[i] + '\\';
#elif defined(unix)
        paths[i] = paths[i - 1] + pfolder[i] + "/";
#endif
	for (int i = 0; i < 16; i++)//create folders
#ifdef WIN32
		_mkdir(paths[i].c_str());
#elif defined(unix)
        mkdir(paths[i].c_str(),0777); 
#endif
	delete[] paths, pfolder;
}

file_control::fs_base::fs_base(std::string name, std::string path){
	this->name = name;
	this->path = path;
#ifdef unix 
	if(path[path.length() -1] != '/'){
		this->full_name = path + '/' + name;
		this->path += '/';
#elif defined(WIN32)
    if(path[path.length() -1] != '\\'){
		this->full_name = path + '\\' + name;
		this->path += '\\';
#endif
	}else{
		this->full_name = path + name;
	}
	size = 0;
}

std::string file_control::fs_base::get_name(){
	return this->name;
}

std::string file_control::fs_base::get_path(){
	return this->path;
}

std::string file_control::fs_base::front(){
	return this->get(0);
}

std::string file_control::fs_base::get_full_name(){
	return this->full_name;
}

int file_control::fs_base::get_size(){
	return this->size;
}

bool file_control::fs_base::range(int index){
	try {
		if (index > get_size() || index < 0) { // checks range
			throw false;
		}
		return true;
	}
	catch (bool out_of_range) {
		return out_of_range;
	}

}

std::string file_control::fs_base::at(int index){
	if(range(index)){
		return get(index);
	}else{
		return get(this->size);
	}
}

// chesk operations 

bool file_control::operator==(file_control::fs_base& a,file_control::fs_base& b) {
	return a.get_size() == b.get_size();
}

bool file_control::operator!=(file_control::fs_base& a,file_control::fs_base& b) {
	return a.get_size() != b.get_size();
}

bool file_control::operator>=(file_control::fs_base& a,file_control::fs_base& b) {
	return a.get_size() >= b.get_size();
}

bool file_control::operator<=(file_control::fs_base& a,file_control::fs_base& b) {
	return a.get_size() <= b.get_size();
}

bool file_control::operator>(file_control::fs_base& a,file_control::fs_base& b) {
	return a.get_size() > b.get_size();
}

bool file_control::operator<(file_control::fs_base& a,file_control::fs_base& b) {
	return a.get_size() < b.get_size();
}


//class implementation file 

file_control::file::file(std::string name, std::string path):fs_base(name,path){
    object.open(full_name.c_str(), std::fstream::out | std::fstream::in | std::fstream::app);
	object.close();
	this->calc_size();
}

void file_control::file::calc_size(){
#ifdef unix
    struct stat data;
	stat(this->full_name.c_str(),&data);
#elif defined(WIN32)
    struct _stat data;
	_stat(this->full_name.c_str(),&data);
#endif
	this->size = data.st_size;
	std::string* buf = new std::string;//temporary string for getline
	object.open(this->full_name, std::fstream::binary | std::fstream::in | std::fstream::app );
	while (getline(object, *buf))
		this->lines++;
	object.close(); // close file
	delete buf;
}

int file_control::file::c_lines(){
	return this->lines;
}

int file_control::file::add(std::string data){
	try{
		object.open(this->full_name, std::fstream::app | std::fstream::binary);// open stream
		if(!object.is_open()){
			throw -1; // if file not open 
		}
		else{
			object << data << std::endl; // write data to file 
			object.close();// close stream 
			this->lines++;
#ifdef unix
    		struct stat rdata;
        	stat(this->full_name.c_str(),&rdata);
#elif defined(WIN32)
            struct _stat rdata;
			_stat(this->full_name.c_str(),&rdata);
#endif
			this->size = rdata.st_size;// change value of size 
			throw 0;
		}
	}
	catch(int result){
		return result;// return code 
	}
}

int file_control::file::add(std::string data, int index){
	try{
		object.open(this->full_name, std::fstream::out | std::fstream::in | std::fstream::binary);// open stream
		if(!object.is_open()){
			throw -1;// if file not open 
		}
		else{ 
			object.close();// close stream 
			if(index > lines){	
				throw 1;// index is out of bounds of file
			}
			else{
				std::string* _fdata = new std::string[lines];//buffer 
				for(int i = 0; i < lines; i++){// load data in file to array
        			_fdata[i] = this->get(i);
				}
				_fdata[index] = data;//rewrite data		
				object.open(this->full_name , std::fstream::binary);//open file to record
				for (int i = 0; i < lines; i++){
					object << _fdata[i] << "\n";
				}
				object.close();
   				this->lines++;
   				delete _fdata;// clean 
#ifdef unix
    			struct stat rdata;
				stat(this->full_name.c_str(),&rdata);
#elif defined(WIN32)
    			struct _stat rdata;
				_stat(this->full_name.c_str(),&rdata);
#endif
				this->size = rdata.st_size;
				throw 0;
			}
		}
	}
	catch(int result){
		return result;
	}
}

std::string file_control::file::get(int index) {
	object.open(this->full_name, std::fstream::out | std::fstream::in | std::fstream::binary);
	std::string buf; //result
	int* i = new int; //temporary counter
	*i = 0;
	while (getline(object, buf)) {//find index
		if (*i == index)
			break;
		(*i)++;
	}
	object.close();//close file
	delete i;
	return buf;
}

std::string file_control::file::back(){
	return this->get(this->lines - 1);
}

int file_control::file::insert(std::string data, int index){
	try
	{
		object.open(this->full_name, std::fstream::out | std::fstream::in | std::fstream::binary); // open stream
		if (!object.is_open())
		{
			throw - 1; // if file not open
		}
		else
		{
			object.close(); // close stream
			if (index > lines)
			{
				throw 1; // index is out of bounds of file
			}
			else
			{
				std::string *_fdata = new std::string[lines]; //buffer
				for (int i = 0; i < lines; i++)
				{ // load data in file to array
					_fdata[i] = this->get(i);
				}
				_fdata[index] += "\n" + data;
				object.open(this->full_name, std::fstream::binary); //open file to record
				for (int i = 0; i < this->lines; i++)
				{
					object << _fdata[i] << "\n";
				}
				object.close();
				delete[] _fdata;
				this->size++;
#ifdef unix
				struct stat rdata;
				stat(this->full_name.c_str(), &rdata);
#elif defined(WIN32)
				struct _stat rdata;
				_stat(this->full_name.c_str(), &rdata);
#endif
				this->size = rdata.st_size;
			}
		}
	}
	catch (int result)
	{
		return result;
	}
	// 	std::string* _fdata = new std::string[lines];//buffer
	// 	for(int i = 0; i < lines; i++){// load data in file to array
	//         _fdata[i] = this->get(i);
	//     }
	// 	_fdata[index] += "\n" + data ;
	//     object.open(this->full_name , std::fstream::binary);//open file to record
	// 	for (int i = 0; i < this->lines; i++)
	// 		object << _fdata[i] << "\n";
	// 	object.close();
	// 	delete[] _fdata;
	// 	this->size++;
	// #ifdef unix
	//     struct stat rdata;
	// 	stat(this->full_name.c_str(),&rdata);
	// #elif defined(WIN32)
	//     struct _stat rdata;
	// 	_stat(this->full_name.c_str(),&rdata);
	// #endif
	// 	this->size = rdata.st_size;
}

int file_control::file::insert(std::string data, int index, int count) {
	std::string* _fdata = new std::string[lines];//buffer 
	for(int i = 0; i < lines; i++){// load data in file to array
        _fdata[i] = this->get(i);
    }
	for (int i = 0; i < count; i++){
		_fdata[index] += "\n" + data;
		this->size++;
	}
	object.open(this->full_name , std::fstream::binary);//open file to record
	for (int i = 0; i < this->lines; i++)
		object << _fdata[i] << "\n";
	object.close();
	delete[] _fdata;
#ifdef unix
    struct stat rdata;
	stat(this->full_name.c_str(),&rdata);
#elif defined(WIN32)
    struct _stat rdata;
	_stat(this->full_name.c_str(),&rdata);
#endif
	this->size = rdata.st_size;
}

void file_control::file::move(std::string directory) {
	int* i = new int;
	file_control::file* temp_out = new file(this->name, this->path); //current file
	file_control::file* temp_in = new file(this->name, directory);//clone
	temp_in->copy(*temp_out);//copy old to new
	delete temp_in, temp_out;
	*i = remove(this->full_name.c_str());//deleting old file
	this->path = directory;
	this->full_name = directory + this->name;
	delete i;
}

void file_control::file::clear() {
	object.open(this->full_name);
	object.close();
	this->size = 0;
	this->lines = 0;
}

void file_control::file::copy(file& source) {
	// for (int i = 0; i != source.get_size(); i++)
	// 	this->add(source.get(i));
    std::ifstream in(source.get_full_name(),std::ios_base::in |  std::ios_base::binary);
	std::ofstream out(this->full_name, std::ios_base::out | std::ios_base::binary);
	char buf[source.get_size()];
	do {
		in.read(&buf[0], source.get_size());   
		out.write(&buf[0], in.gcount()); 
	} while (in.gcount() > 0);     
	in.close();
	out.close();
}

void file_control::file::rename_file(std::string new_name) {
	int* i = new int;
	*i = rename(this->full_name.c_str(), (this->path + new_name).c_str());//rename
	this->name = new_name;
	this->full_name = this->path + new_name; //change name
	delete i;
}

//class implementation folder

file_control::folder::folder(std::string name, std::string path):fs_base(name,path){
    if (search(this->full_name)){ // if folder exist
		calc_size();
    }
}

void file_control::folder::calc_size(){
#ifdef WIN32
    struct _finddata_t data;
	intptr_t done = _findfirst(this->full_name.c_str(), &data);
    this->size += data.size;
	while (_findnext(done, &data) == 0) {
		if (data.attrib == _A_SUBDIR && this->_elements != 0) {
			folder f(std::string(data.name), this->path + this->name + "\\");
			this->size += f.get_size();
            this->_folders++;
            this->_folders += f.c_folders();
            this->_files += f.c_files();
		}else{
            this->_files++;
            this->size += data.size;
        }
	}
	_findclose(done);
#elif defined (unix)
 DIR *dir = opendir(this->full_name.c_str());
	struct dirent *ent;
    struct stat data;
    while((ent = readdir(dir)) != NULL){
		if ( strcmp( ".", ent->d_name ) == 0 || strcmp( "..", ent->d_name ) == 0 )
			continue;
		if(ent->d_type == DT_DIR){
			folder f(std::string(ent->d_name),this->full_name);// temp 
			this->size += f.get_size();
            this->_folders++;
            this->_folders += f.c_folders();
            this->_files += f.c_files();
		}else {
			stat(ent->d_name,&data);
            this->_files++;
		    this->size += data.st_size;
        }
	}
	closedir(dir);
#endif
    this->_elements += this->_files + this->_folders;// count elements
} 
 
std::string file_control::folder::get(int index) {
	int* inter = new int;
	*inter = 0;
    std::string result; // temp 
#ifdef WIN32
	struct _finddata_t data;
	intptr_t done = _findfirst(this->full_name.c_str(), &data);
	while (_findnext(done, &data) == 0)
		if (*inter == index)
			break;
		else
			(*inter)++;
	delete inter;
	result = data.name;
#elif defined(unix)
	struct stat data;
	DIR *dir = opendir(this->full_name.c_str());
	struct dirent *ent;
	while((ent = readdir(dir)) != NULL){
		if (*inter == index) // find index
			break;
		else
			(*inter)++;
	}
	delete inter;
    result = ent->d_name;
#endif
	return result;
}

std::string file_control::folder::back(){
    std::string result; // temp 
#ifdef WIN32
	struct _finddata_t data;
	intptr_t done = _findfirst(this->full_name.c_str(), &data);
	while (_findnext(done, &data) == 0);
	result = data.name;
#elif defined(unix)
	struct stat data;
	DIR *dir = opendir(this->full_name.c_str());
	struct dirent *ent;
	while((ent = readdir(dir)) != NULL){
    	result = ent->d_name;
	}
#endif
	return result;
}

bool file_control::folder::verification() {
	if (!search(this->path.c_str(), this->name.c_str()))
		return false;
	else
		return true;
}

void file_control::folder::create() {
	make_path(this->full_name);
	calc_size();
}

int file_control::folder::c_elements(){
    return this->_elements;
}

int file_control::folder::c_files(){
    return this->_files;
}

int file_control::folder::c_folders(){
    return this->_folders;
}

int file_control::duplicate(file_control::file& _source, file_control::file& _object){
	static int diff;
	static std::string _src_name; // name of source file
	static std::string _obj_name; // name of object file
	//check data
	if(_src_name != _source.get_full_name() || _obj_name != _object.get_full_name()){
		diff = 0;
		_src_name = _source.get_full_name(); // save 
	    _obj_name = _object.get_full_name();
	}
	int _end = (_source.c_lines() > _object.c_lines())? _source.c_lines() : _object.c_lines();
	for(int i = diff; i < _end; i++){
		if(_source.get(i) != _object.get(i)){
			diff = i + 1; 
			return i;// return index differences
		}
	}
	diff = 0;//dropping
	_src_name = ""; 
	_obj_name = "";
	return -1;// if the contents are the same
}

#include <iostream>

int file_control::duplicate(file_control::folder& _source, file_control::folder& _object){
	static int diff;
	static std::string _src_name; // name of source folder
	static std::string _obj_name; // name of object folder
	//check data
	if(_src_name != _source.get_full_name() || _obj_name != _object.get_full_name()){
		diff = 0;
		_src_name = _source.get_full_name(); // save 
		_obj_name = _object.get_full_name();
	}
	for(int i = diff + 2;; i++){
		std::cout << "a " << _object.get(i) <<"\n";
		std::cout << "b " << _source.get(i) <<"\n";
		if(_source.get(i) != _object.get(i)){
			diff = i + 1;
			return i;// return index differences
		}else {
			if(is_file(_source.get(i).c_str()) && !is_file(_object.get(i).c_str())){
				diff = i + 1;
				return i;//return index of differences
			}
			if(is_folder(_source.get(i).c_str()) && !is_folder(_object.get(i).c_str())){
				diff = i + 1;
				return i;//return index of differences
			}
		}
		if(_source.get(i) != _source.back()){
			break;
		}
	}
	diff = 0;
	_src_name = "";
	_obj_name = "";
	return -1;// if the contents are the same
}

bool file_control::is_file(const char* name){
#ifdef WIN32
    struct _stat st;
    if(_stat(name, &st) == 0){
        if(st.st_mode & _S_IFREG){
			return true;
		} 
        if(st.st_mode & _S_IFDIR){
			return false;
		} 
    }
#elif defined(unix)
    struct stat st;
    if(stat(name, &st) == 0){
        if(S_ISREG(st.st_mode)){ 
			return true; 
		}
        if(S_ISDIR(st.st_mode)){
			return false;
		} 
    }
#endif
}

bool file_control::is_folder(const char* name){
#ifdef WIN32
    struct _stat st;
    if(_stat(name, &st) == 0){
        if(st.st_mode & _S_IFREG){
			return false;
		} 
        if(st.st_mode & _S_IFDIR){
			return true;
		} 
    }
#elif defined(unix)
    struct stat st;
    if(stat(name, &st) == 0){
        if(S_ISREG(st.st_mode)){ 
			return false; 
		}
        if(S_ISDIR(st.st_mode)){
			return true;
		} 
    }
#endif
}