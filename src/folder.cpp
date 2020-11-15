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
    if (access(full_name.c_str(), 0) !=0)
#endif
    {
		return; // if folder not exists 
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
			this->size += dir_information((full_name + "/" + ent->d_name).c_str()).size;
            this->folders++;
            this->folders += dir_information((full_name + "/" + ent->d_name).c_str()).folders;
            this->files += dir_information((full_name + "/" + ent->d_name).c_str()).files;
		}
        if(ent->d_type == DT_REG){
			stat((full_name + "/" + ent->d_name).c_str(),&data);
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
        return false; // if folder not exists 
    }
    return true;
}

bool FSTool::folder::empty(){
    if(this->_info->length == 2){
        return true;
    }
    return false;
}

FSTool::_dirinfo FSTool::folder::get_info(){
    return *_info;
}

std::string FSTool::folder::get(int index){
    if(!this->exists()){
        throw fs_exception("folder not found", -2); // if file exists
    }
    if(index >= _info->length){
        throw fs_exception("not valid index of element", -32);
    }
    int* inter = new int;
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
    }    result = data.name;
#elif defined(unix)
	DIR *dir = opendir(this->_info->full_name.c_str());
	struct dirent *ent;
	while((ent = readdir(dir)) != NULL){
		if (*inter == index){
            result = ent->d_name;
            break;
        }
		else{
			(*inter)++;
        }
	}
#endif
    delete inter;
    return result;
}

std::string FSTool::folder::back(){
    return this->get(this->_info->length);
}


FSTool::strvect FSTool::folder::get_elements_of_path(){
    strvect elements; 
    std::string *temp = new std::string;
    char* token = NULL;
#ifdef WIN32
    char * next_token = NULL;
    char buff[1024];
	strcpy_s(buff, _info->full_name.c_str());
	token = strtok_s(buff, "\\", &next_token);
    elements.push_back(token);
    token = strtok(NULL,"/",&next_token);
    for(int i = 1;token != NULL; i++){
        *temp = elements[i-1] + "/" + token;
        elements.push_back(*temp);
        token = strtok(NULL,"/",*next_token);
    }
    delete next_token;
#elif defined(unix)
    char buff[_info->full_name.length()];
    strcpy(buff, _info->full_name.c_str());
    token = strtok(buff,"/");
    elements.push_back(token);
    token = strtok(NULL,"/");
    for(int i = 1;token != NULL; i++){
        *temp = elements[i-1] + "/" + token;
        elements.push_back(*temp);
        token = strtok(NULL,"/");
    }
#endif
	delete token; 
    delete temp;
    return elements;
}


int FSTool::folder::create(){
    if(this->exists()){
        throw fs_exception("folder already exists", -1);
    }
    std::vector<std::string> path = this->get_elements_of_path();
	for (int i = 0; i < path.size(); i++) {//create folders
#ifdef WIN32
		_mkdir(pats[i].c_str());
#elif defined(unix)
        mkdir(path[i].c_str(),0777);
#endif
    }
    return 0;
} 

bool FSTool::folder::range(int index){
    if(index > this->_info->length || index < 0 ){
        return false;
    }
    else{
        return true;
    }
}

int FSTool::folder::destroy(){
    if(!this->exists()){
        throw fs_exception("folder not found", -2); // if file exists
    }
#ifdef WIN32
    struct _finddata_t data;
	intptr_t done = _findfirst(this->full_name.c_str(), &data);
    this->size += data.size;
	while (_findnext(done, &data) == 0) {
        this->length++;
		if (data.attrib == _A_SUBDIR && this->_elements != 0) {
			if ( strcmp( ".", ent->d_name ) == 0 || strcmp( "..", ent->d_name ) == 0 ){
			    continue;
            }
			folder * temp = new folder(std::string(this->_info->full_name+ "\\" + ent->d_name).c_str());
            temp->destroy(); // delete subdir 
            delete temp; 
            _rmdir(std::string(this->_info->full_name+ "\\" + ent->d_name).c_str());
		}else{
            remove(std::string(this->_info->full_name+ "\\" + ent->d_name).c_str());
        }
	}
	_findclose(done);
#elif defined (unix)
    DIR *dir = opendir(this->_info->full_name.c_str());
	struct dirent *ent;
    while((ent = readdir(dir)) != NULL){
	    if(ent->d_type == DT_DIR){
            if ( strcmp( ".", ent->d_name ) == 0 || strcmp( "..", ent->d_name ) == 0 ){
			    continue;
            }
			folder * temp = new folder(std::string(this->_info->full_name+ "/" + ent->d_name).c_str());
            temp->destroy(); // delete subdir 
            delete temp; 
            rmdir(std::string(this->_info->full_name+ "/" + ent->d_name).c_str());
		}
        if(ent->d_type == DT_REG){
			remove(std::string(this->_info->full_name+ "/" + ent->d_name).c_str());
        }
	}
    closedir(dir);
    rmdir(this->_info->full_name.c_str());	
#endif
    return 0;
}

FSTool::strvect FSTool::folder::get_content_list(){
    if(this->empty()){
        throw fs_exception("folder is empty", -321);
    }
    if(!this->exists()){
        throw fs_exception("folder not found", -2); // if file exists
    }
    strvect result;
    for(int i = 0; i < this->_info->length; i++){
        result.push_back(this->get(i));
    }
    return result;
}

void FSTool::folder::move(std::string path){
    folder * temp = new folder(path);
    if(!temp->exists()){
        temp->create();
    }
    delete temp;
    std::string * src;
    src = new std::string(this->_info->full_name);
    std::string * new_pl;
    if(path[path.length()-1] != '/'){
        new_pl = new std::string(path + "/" + this->_info->name);
        this->_info->path = path + "/";
    }else{
        new_pl = new std::string(path + this->_info->name);
        this->_info->path = path;
    }
	rename(src->c_str(), new_pl->c_str());
    this->_info->full_name = *new_pl;
    delete new_pl;
    delete src;
}

int FSTool::folder::find(std::string object, int begin, int end){
    if(!this->exists()){
        throw fs_exception("file not found", -2); // if file exists
    }
    static int _find;
    static int _begin; // begin position
    static int _end;   // end position
    static std::string _object;
    if (_object != object || begin != _begin & 0 || end != _end & 0) {
        _find = begin; // update data
        _object = object;
        _begin = begin;
        if(end == 0){
            _end = this->_info->length;
        }
        else{
            _end = end;
        }
    }
    if (_begin > _end || _begin & _end < 0){
        throw fs_exception("not valid search point",41);
    }
    if (_begin == _end){
        if(this->get(begin).find(object)!=std::string::npos){
            _find = begin;
            return begin;
        }
    }
    else{
        for (int i = _find;i < _end;i++){
            if((this->get(i).find(object)!=std::string::npos)){
                _find = i + 1; // save point
                return i;
            }
        }
    }
    return -1;
}