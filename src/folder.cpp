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
    while((ent = readdir(dir)) != NULL){
		if ( strcmp( ".", ent->d_name ) == 0 || strcmp( "..", ent->d_name ) == 0 )
			continue;
		if(ent->d_type == DT_DIR){
			this->size += dir_information(full_name).size;
            this->folders++;
            this->folders += dir_information(full_name).folders;
            this->files += dir_information(full_name).files;
		}else {
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