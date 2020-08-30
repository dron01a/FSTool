#include "folder.h"

FSTool::_dirinfo::_dirinfo(std::string full_name){
    this->full_name = full_name;
    int *find = new int(this->full_name.find_last_of("/\\")); // split into folder name and path
    this->path = this->full_name.substr(0, *find);            // set path
    this->name = this->full_name.substr(*find + 1);           // set folder name
    delete find;    
#ifdef WIN32
	if (_access(full_name.c_str(), 0))
#elif defined(unix) 
    if (access(full_name.c_str(), 0))
#endif
    {
		return; // if folder exists 
    }
#ifdef WIN32
    struct _finddata_t data;
	intptr_t done = _findfirst(this->full_name.c_str(), &data);
    this->size += data.size;
	while (_findnext(done, &data) == 0) {
        this->length++;
		if (data.attrib == _A_SUBDIR && this->_elements != 0) {
			this->size += dir_information(full_name).size;
            this->folders++;
            this->folders += dir_information(full_name).folders;
            this->files += dir_information(full_name).files;
		}else{
            this->files++;
            this->size += data.size;
        }
	}
	_findclose(done);
#elif defined (unix)
    DIR *dir = opendir(this->full_name.c_str());
	struct dirent *ent;
    struct stat data;
    stat(full_name.c_str(),&data);
    while((ent = readdir(dir)) != NULL){
        this->length++;
	    if(ent->d_type == DT_DIR){
            if ( strcmp( ".", ent->d_name ) == 0 || strcmp( "..", ent->d_name ) == 0 ){
			    continue;
            }
			this->size += dir_information( ent->d_name).size;
            this->folders++;
            this->folders += dir_information(ent->d_name).folders;
            this->files += dir_information(ent->d_name).files;
		}
        if(ent->d_type == DT_REG){
			stat(ent->d_name,&data);
            this->files++;
		    this->size += data.st_size;
        }
	}
	closedir(dir);
#endif
    this->elements += this->files + this->folders;// count elements
}

FSTool::_dirinfo FSTool::dir_information(std::string file_name){
    return _dirinfo(file_name);// return struct
}  

FSTool::folder::folder(std::string name, std::string path){
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
    _info = new _dirinfo(*temp_fullname); // full information struct
    delete temp_fullname; 
}

FSTool::folder::folder(std::string name){
    _info = new _dirinfo(name); // full struct
}

FSTool::folder::~folder(){
    delete _info;
}

bool FSTool::folder::exists(){
#ifdef WIN32
    if (_access(_info->full_name.c_str(), 0))
#elif defined(unix)
    if (access(_info->full_name.c_str(), 0))
#endif
    {
        return false; // if folder exists 
    }
    return true;
}

FSTool::_dirinfo FSTool::folder::get_info(){
    return *_info;
}

std::string FSTool::folder::get(int index){
    int* inter = new int(0);
    std::string result;
#ifdef WIN32
	struct _finddata_t data;
	intptr_t done = _findfirst(this->_info->full_name.c_str(), &data);
	while (_findnext(done, &data) == 0){
		if (*inter == index){
            break;
        }
		else{
			(*inter)++;
        }
    }
    result = data.name;
#elif defined(unix)
	DIR *dir = opendir(this->_info->full_name.c_str());
	struct dirent *ent;
	while((ent = readdir(dir)) != NULL){
		if (*inter == index){
            break;
        }
		else{
			(*inter)++;
        }
	}
    result = ent->d_name;
#endif
    delete inter;
    return result;
}

std::string FSTool::folder::back(){
    return this->get(this->_info->length);
}