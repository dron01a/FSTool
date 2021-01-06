#include "file.h"

FSTool::file::file(std::string name) : FSTool::_base(name) {
    int *found = new int(this->_fullName.find_first_of(".")); // find "." to set extension of file
    this->extension = this->_fullName.substr(*found + 1, name.size() - *found + 1);
    delete found; // free memory
    std::fstream *obj = new std::fstream(this->_fullName);
    if(obj->is_open()){
        std::string *buf = new std::string; //temporary string for getline
        while (getline(*obj, *buf)){
            this->_lines++;
        }
        obj->close(); // close file
        delete buf;
        delete obj;
#ifdef unix
        struct stat data;
        stat(this->_fullName.c_str(), &data);
#elif defined(WIN32)
        struct _stat data;
        _stat(this->_fullName.c_str(), &data);
#endif
        this->_size = data.st_size;
        this->_lmTime = gmtime(&data.st_mtime); // add time to struct
        this->_lmTime->tm_mon += 1;             // fix month
        this->_lmTime->tm_year += 1900;         // fix year
    }
}

FSTool::file::file(std::string name, std::string path) : FSTool::_base(name,path) {
    file(this->_fullName); 
}

int FSTool::file::resize(){
#ifdef unix
    struct stat rdata;
    stat(this->_fullName.c_str(), &rdata);
#elif defined(WIN32)
    struct _stat rdata;
    _stat(this->_fullName.c_str(), &rdata);
#endif
    return rdata.st_size;
}

int FSTool::file::create(){
    if(this->exists()){
        throw fs_exception("file already exists", -1);
    }
    std::ofstream *temp = new std::ofstream(this->_fullName.c_str(), std::fstream::binary); // create temp object
    if (!temp->is_open()){
        delete temp;   // free memory
        return 1;      // return code
    }
    else{
        temp->close();          // close stream
        file(this->_fullName);
        delete temp;            // free memory
        return 0;               // return code
    }
}

int FSTool::file::destroy(){
    if(!this->exists()){
        throw fs_exception("file not found", -2);
    }
    return remove(this->_fullName.c_str()); // return result of deleting file
}

bool FSTool::file::empty(){
    return (this->_size == 0) && (this->_lines == 0); 
}

std::string FSTool::file::get(int index){
    if(!this->exists()){
        throw fs_exception("file not found", -2); // if file exists
    }
    if(index >= _lines || index < 0){
        throw fs_exception("not valid index", -3);
    }
    std::fstream * object = new std::fstream(this->_fullName, std::fstream::out | std::fstream::in | std::fstream::binary);
    std::string buf;               // result
    int *i = new int(0);           // temporary counter
    while (getline(*object, buf)){ // find index
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
    return this->get(_lines--);// return lasr line  
}

bool FSTool::file::range(int index){
    if (index < 0 || index > _lines){
        return false; // if index not in range
    }
    else{
        return true; // if index in range
    }
}

int FSTool::file::add(std::string data){
    if(!this->exists()){
        throw fs_exception("file not found", -2); // if file exists
    }
    std::fstream *obj; // temp object 
    obj = new std::fstream(this->_fullName, std::fstream::app | std::fstream::binary);
    *obj << data << std::endl; // write
    obj->close();              // save and close stream 
    delete obj;                // free memory 
    this->_lines++;
    return 0;
}

int FSTool::file::add(std::string data, int index){
    if(!this->exists()){
        throw fs_exception("file not found", -2); // if file exists
    }
    if(index >= this->_lines || index < 0){
        throw fs_exception("not valid index", -3);
    }
    std::string *_buff = new std::string[this->_lines]; // temp buffeer
    for(int i = 0; i < this->_lines; i++){
        _buff[i] = this->get(i); // load file data to buff 
    }
    _buff[index] = data; // rewrite line 
    int *linesTemp = new int(this->_lines); 
    this->clear(); // delete data in file
    for(int i = 0; i < *linesTemp; i++){
        this->add(_buff[i]); // load buf to file  
    }
    delete[] _buff;
    delete linesTemp;
    this->_lines++;
    this->_size = resize();  // get new size from bites of file 
    
    return 0; 
}

int FSTool::file::insert(std::string data, int index){
    if(!this->exists()){
        throw fs_exception("file not found", -2); // if file exists
    }
    if(index >= this->_lines || index < 0){
        throw fs_exception("not valid index", -3);
    }
    std::string *_fdata = new std::string[this->_lines]; //buffer
    for (int i = 0; i < this->_lines; i++) { // load data in file to array
        _fdata[i] = this->get(i);
    }
    int *linesTemp = new int(this->_lines);
    this->clear();
    for (int i = 0; i < *linesTemp; i++){
        if (i == index ){
            this->add(data);// add data 
        }
        this->add(_fdata[i]);
    }
    delete[] _fdata;
    delete linesTemp;
    this->_lines++;
    this->_size = resize(); // get new size from bites of file 
    return 0;
}

int FSTool::file::insert(std::string data, int index, int count){
    if(!this->exists()){
        throw fs_exception("file not found", -2); // if file exists
    }
    if(index >= this->_lines || index < 0){
        throw fs_exception("not valid index", -3);
    }
    std::string *_fdata = new std::string[this->_lines]; //buffer
    for (int i = 0; i < this->_lines; i++) { // load data in file to array
        _fdata[i] = this->get(i);
    }
    int * linesTemp = new int(this->_lines);  // temp count lines
    this->clear();
    for (int i = 0; i < *linesTemp; i++){
        if (i == index ){
            for (int c = 0; c < count; c++){
                this->add(data); // add data
            } 
        }
        this->add(_fdata[i]);
    }
    delete linesTemp;
    delete[] _fdata;
    this->_lines++;
    this->_size = resize(); // get new size from bites of file 
    return 0;
}

void FSTool::file::clear(){
    std::ofstream *temp = new std::ofstream(this->_fullName); // temp object
    this->_lines = 0;
    this->_size = 0;
    delete temp;
}

void FSTool::file::copy(file &source){
    std::ifstream *src; // temp input
    std::ofstream *out; // temp output
    src = new std::ifstream(source.full_name(), std::ios::binary); // open input file
    out = new std::ofstream(this->_fullName, std::ios::binary); // open source file 
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
    out = new std::ofstream(this->_fullName, std::ios::binary); // open source file 
    *out << src->rdbuf(); // write data
    src->close(); // close streams 
	out->close();
    delete src; // free memory
    delete out;
}

int FSTool::file::find(std::string object, int begin, int end){
    if(!this->exists()){
        throw fs_exception("file not found", -2); // if file exists
    }
    static int _find;
    static int _begin; // begin position
    static int _end;   // end position
    static std::string _object;
    if (_object != object ||begin != _begin & 0 || end != _end & 0) {
        _find = begin; // update data
        _object = object;
        _begin = begin;
        if(end == 0){
            _end = this->_lines;
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

FSTool::strvect FSTool::file::get_elements_of_path(){
    strvect elements; 
    std::string *temp = new std::string;
    char* token, * next_token = NULL;
#ifdef WIN32
    char p[1024];
	strcpy_s(p, _info->full_name.c_str());
	token = strtok_s(p, "\\", &next_token);
    elements.push_back(token);
	for (int i = 0; token != NULL; token = strtok_s(NULL, "\\", &next_token), i++){
        *temp = elements[i-1] + "\\" + token;
#elif defined(unix)
    char p[this->_fullName.length()];
    strcpy(p, this->_fullName.c_str());
    token = strtok(p, "/");
    elements.push_back(token);
    for (int i = 1; token != NULL; token = strtok(NULL, "/"), i++){
        *temp = elements[i-1] + "/" + token;
#endif
		elements.push_back(*temp);
    }
	delete token; 
    delete next_token;
    delete temp;
    return elements;
}