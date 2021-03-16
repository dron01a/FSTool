#include "folder.h"

#ifdef WIN32
#define stat _stat
#define mkdir _mkdir;
#endif

FSTool::folder::folder(std::string name) : FSTool::FST_object(name) {
    if (exists()){
        update();
    }
}

FSTool::folder::folder(std::string name, std::string path) : FSTool::FST_object(name,path) {
    if (exists()){
        update();
    }
}

void FSTool::folder::update(){
    std::vector<time_t> changes;
#ifdef WIN32
    struct _finddata_t data;
    intptr_t done = _findfirst(_fullName.c_str(), &data);
    _size += data.size;
    while (_findnext(done, &data) == 0) {
        _length++;
    	if (data.attrib == _A_SUBDIR && _elements != 0) {
            FSTool::folder * temp = new FSTool::folder(data.name); // temp object
    		_size += temp->size();
            _folders++;
            _folders += temp->folders();
            _files += temp->files();
            delete temp;
    	}else{
            _files++;
            _size += data.size;
        }
    }
    _findclose(done);
#elif defined (unix)
    DIR *dir = opendir(_fullName.c_str());
	struct dirent *ent;
    struct stat data;
    stat(_fullName.c_str(),&data);
    while((ent = readdir(dir)) != NULL){
	    stat((_fullName + "/" + ent->d_name).c_str(),&data);
        _length++;
	    if(S_ISDIR(data.st_mode)){
            if ( strcmp( ".", ent->d_name ) == 0 || strcmp( "..", ent->d_name ) == 0 ){
			    continue;
            }
            FSTool::folder * temp = new FSTool::folder(_fullName + "/" + ent->d_name); 
			_size += temp->size(); // get size of subdir
            _folders++;
            _folders += temp->folders(); // get count of folders in subdir
            _files += temp->files(); // get count of files in subdir
            changes.push_back(mktime(temp->last_modification()));
            delete temp;
		}
        else{
            tm * tempTime = localtime(&data.st_mtime);
            _files++;
	    	_size += data.st_size;
            tempTime->tm_mon += 1;     // fix month
            tempTime->tm_year += 1900; // fix year
            changes.push_back(mktime(tempTime));
        }
	}
	closedir(dir);
#endif
    _elements += _files + _folders; // count elements
    time_t last;
    if (_elements == 0 ){
        last = time(NULL);
    }
    else{ 
        last = *std::max_element(changes.begin(),changes.end());
    }
    _lmTime = localtime(&last);
    changes.clear();
}

bool FSTool::folder::empty(){
    if(_length == 2 && _size == 0){
        return true; // folder empty 
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
    int inter = 0;
    std::string result;
#ifdef WIN32
	struct _finddata_t data;
	intptr_t done = _findfirst(this->_info->full_name.c_str(), &data);
	while (_findnext(done, &data) == 0){
		if (inter == index){
            break;
        }
		else{
			inter++;
        }
    }    
    result = data.name;
#elif defined(unix)
	DIR *dir = opendir(this->_fullName.c_str());
	struct dirent *ent;
	while((ent = readdir(dir)) != NULL){
		if (inter == index){
            result = ent->d_name; // add result
            break;
        }
		inter++;
	}
#endif
    return result;
}

std::string FSTool::folder::back(){
    return this->get(this->_length); // return last element 
}

int FSTool::folder::create(){
    if(exists()){
        throw fs_exception("folder already exists", -1);
    }
    std::vector<std::string> path = pathSteps();
	for (int i = 0; i < path.size(); i++) {  
        mkdir(path[i].c_str(),0777); //create folders
    }
    return 0;
} 

bool FSTool::folder::range(int index){
    if(index > _length || index < 0 ){
        return false;
    }
    else{
        return true;
    }
}

int FSTool::folder::destroy(){
    if(!exists()){
        throw fs_exception("folder not found", -2); // if file exists
    }
#ifdef WIN32
    struct _finddata_t data;
	intptr_t done = _findfirst(full_name.c_str(), &data);
	while (_findnext(done, &data) == 0) {;
		if (data.attrib == _A_SUBDIR && _elements != 0) {
			if ( strcmp( ".", ent->d_name ) == 0 || strcmp( "..", ent->d_name ) == 0 ){
			    continue;
            }
			folder * temp = new folder(std::string(_info->full_name+ "\\" + ent->d_name).c_str());
            temp->destroy(); // delete subdir 
            delete temp; 
            _rmdir(std::string(_info->full_name+ "\\" + ent->d_name).c_str());
		}else{
            remove(std::string(_info->full_name+ "\\" + ent->d_name).c_str());
        }
	}
	_findclose(done);
#elif defined (unix)
    DIR *dir = opendir(_fullName.c_str());
	struct dirent *ent;
    struct stat data;
    while((ent = readdir(dir)) != NULL){
        if ( strcmp( ".", ent->d_name ) == 0 || strcmp( "..", ent->d_name ) == 0 ){
		    continue;
        }
        std::string curObj = _fullName + "/" + ent->d_name;
        curObj = realpath(curObj.c_str(),NULL);
        stat(curObj.c_str(), &data);
	    if(S_ISDIR(data.st_mode)){
			folder * temp = new folder(curObj);
            temp->destroy(); // delete content in subdir 
            delete temp; 
		}
        else {
            unlink(curObj.c_str());
        }
	}
    closedir(dir);
    rmdir(_fullName.c_str()); // remove dir
#endif
    return 0;
}

FSTool::strvect FSTool::folder::content(){
    if(empty()){
        throw fs_exception("folder is empty", -321);
    }
    if(!exists()){
        throw fs_exception("folder not found", -2); // if file exists
    }
    strvect result;
    for(int i = 0; i < _length; i++){
        result.push_back(get(i));
    }
    return result;
}

int FSTool::folder::find(std::string object, int begin, int end){
    if(!exists()){
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
        if(get(begin).find(object)!=std::string::npos){
            _find = begin;
            return begin;
        }
    }
    else{
        for (int i = _find;i < _end;i++){
            if((get(i).find(object)!=std::string::npos)){
                _find = i + 1; // save point
                return i;
            }
        }
    }
    return -1;
}

#ifdef unix

void FSTool::folder::move(std::string path){
    if(!FSTool::exists(path)){
        throw fs_exception("not found", -2);
    }
    folder *temp = new folder(_name, path); // temp obj to clone
    if(!temp->exists()){
        temp->create();
    }
    temp->copy(_fullName);
    delete temp;
    this->destroy();
#ifdef WIN32
	if(path[path.length() -1] != '\\'){
		_fullName = path + '\\' + _name;
		_path += '\\';
	}
#elif defined(unix) 
    if(path[path.length() -1] != '/'){
		_fullName = path + '/' + _name;
		_path += '/';
	}
#endif
    else{
		_fullName = path + _name;
	}   
}

void FSTool::folder::copy(std::string path){
    DIR *dir;
    struct dirent *ent;
    struct stat data;
    if ((dir = opendir (path.c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            if (ent->d_name != std::string(".") && ent->d_name != std::string("..")){
                stat(std::string(path + "/" + ent->d_name).c_str(), &data);
                if(S_ISDIR(data.st_mode)){
                    folder subdir(_fullName + "/" + ent->d_name);
                    subdir.create();
                    subdir.copy(path + "/" + ent->d_name);
                }
                else{
                    char buff[data.st_size];
                    FILE *in, *out;
                    size_t n;
                    in  = fopen(std::string(path + "/" + ent->d_name).c_str(), "rb");
                    out = fopen(std::string(_fullName + "/" + ent->d_name).c_str(), "wb");
                    while ( (n=fread(buff,1,BUFSIZ,in)) != 0 ) {
                        fwrite( buff, 1, n, out );
                    }
                    fclose(in);
                    fclose(out);
                }
            } 
        }
    }
}

#endif