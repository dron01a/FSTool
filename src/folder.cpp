#include "folder.h"

#ifdef WIN32
#define stat _stat
#define mkdir _mkdir;
#endif

FSTool::folder::folder(std::string name) : FSTool::_base(name) {
    if (this->exists()){
        this->update();
    }
}

FSTool::folder::folder(std::string name, std::string path) : FSTool::_base(name,path) {
    if (this->exists()){
        this->update();
    }
}

void FSTool::folder::update(){
    std::vector<time_t> changes;
#ifdef WIN32
    struct _finddata_t data;
    intptr_t done = _findfirst(this->_fullName.c_str(), &data);
    this->_size += data.size;
    while (_findnext(done, &data) == 0) {
        this->_length++;
    	if (data.attrib == _A_SUBDIR && this->_elements != 0) {
            FSTool::folder * temp = new FSTool::folder(data.name); // temp object
    		this->_size += temp->size();
            this->_folders++;
            this->_folders += temp->folders();
            this->_files += temp->files();
            delete temp;
    	}else{
            this->_files++;
            this->_size += data.size;
        }
    }
    _findclose(done);
#elif defined (unix)
    DIR *dir = opendir(this->_fullName.c_str());
	struct dirent *ent;
    struct stat data;
    stat(this->_fullName.c_str(),&data);
    while((ent = readdir(dir)) != NULL){
        this->_length++;
	    if(ent->d_type == DT_DIR){
            if ( strcmp( ".", ent->d_name ) == 0 || strcmp( "..", ent->d_name ) == 0 ){
			    continue;
            }
            FSTool::folder * temp = new FSTool::folder(this->_fullName + "/" + ent->d_name); 
			this->_size += temp->size();
            this->_folders++;
            this->_folders += temp->folders();
            this->_files += temp->files();
            changes.push_back(mktime(temp->last_modification()));
            delete temp;
		}
        if(ent->d_type == DT_REG){
            tm * tempTime = localtime(&data.st_mtime);
			stat((this->_fullName + "/" + ent->d_name).c_str(),&data);
            this->_files++;
	    	this->_size += data.st_size;
            tempTime->tm_mon += 1;                // fix month
            tempTime->tm_year += 1900;            // fix year
            changes.push_back(mktime(tempTime));
        }
	}
	closedir(dir);
#endif
    this->_elements += this->_files + this->_folders;// count elements
    time_t last =  *std::max_element(changes.begin(),changes.end());
    changes.clear();
    this->_lmTime = localtime(&last);
}

bool FSTool::folder::empty(){
    if(this->_length == 2){
        return true;
    }
    return false;
}

int FSTool::folder::folders(){
    return _folders;
}

int FSTool::folder::files(){
    return _files;
}

int FSTool::folder::elements(){
    return _elements;
}

int FSTool::folder::lenght(){
    return _length;
}

std::string FSTool::folder::get(int index){
    if(!this->exists()){
        throw fs_exception("folder not found", -2); // if file exists
    }
    if(index >= this->_length || index < 0){
        throw fs_exception("not valid index of element", -3);
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
	DIR *dir = opendir(this->_fullName.c_str());
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
    return this->get(this->_length);
}


FSTool::strvect FSTool::folder::get_elements_of_path(){
    strvect elem; 
    std::string *temp = new std::string;
    char* token = NULL;
#ifdef WIN32
    char * next_token = NULL;
    char buff[1024];
	strcpy_s(buff, _fullName.c_str());
	token = strtok_s(buff, "\\", &next_token);
    elem.push_back(token);
    token = strtok(NULL,"/",&next_token);
    for(int i = 1;token != NULL; i++){
        *temp = elem[i-1] + "/" + token;
        elem.push_back(*temp);
        token = strtok(NULL,"/",*next_token);
    }
    delete next_token;
#elif defined(unix)
    char buff[_fullName.length()];
    strcpy(buff, _fullName.c_str());
    token = strtok(buff,"/");
    elem.push_back(token);
    token = strtok(NULL,"/");
    for(int i = 1;token != NULL; i++){
        *temp = elem[i-1] + "/" + token;
        elem.push_back(*temp);
        token = strtok(NULL,"/");
    }
#endif
	delete token; 
    delete temp;
    return elem;
}


int FSTool::folder::create(){
    if(this->exists()){
        throw fs_exception("folder already exists", -1);
    }
    std::vector<std::string> path = this->get_elements_of_path();
	for (int i = 0; i < path.size(); i++) {//create folders
        mkdir(path[i].c_str(),0777);
    }
    return 0;
} 

bool FSTool::folder::range(int index){
    if(index > this->_length || index < 0 ){
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
	while (_findnext(done, &data) == 0) {;
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
    DIR *dir = opendir(this->_fullName.c_str());
	struct dirent *ent;
    while((ent = readdir(dir)) != NULL){
	    if(ent->d_type == DT_DIR){
            if ( strcmp( ".", ent->d_name ) == 0 || strcmp( "..", ent->d_name ) == 0 ){
			    continue;
            }
			folder * temp = new folder(std::string(this->_fullName + "/" + ent->d_name).c_str());
            temp->destroy(); // delete subdir 
            delete temp; 
            rmdir(std::string(_fullName + "/" + ent->d_name).c_str());
		}
        else {
			remove(std::string(this->_fullName+ "/" + ent->d_name).c_str());
        }
	}
    closedir(dir);
    rmdir(this->_fullName.c_str());	
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
    for(int i = 0; i < this->_length; i++){
        result.push_back(this->get(i));
    }
    return result;
}

int FSTool::folder::find(std::string object, int begin, int end){
    if(!this->exists()){
        throw fs_exception("folder not found", -2); // if file exists
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
            _end = this->_length;
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