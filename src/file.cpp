#include "file.h"

#ifdef WIN32
#define stat _stat
#endif

FSTool::file::file(std::string name) : FSTool::FST_object(name) {
    int found = _fullName.find(".");    // position of '.'
    _extension = "";
    if(found != std::string::npos){
        _extension = _fullName.substr(found + 1, _fullName.size() - found - 1);
    }
}   

FSTool::file::file(std::string name, std::string path) : FSTool::FST_object(name,path) {
    int found = _fullName.find(".");    // position of '.'
    _extension = "";
    if(found != std::string::npos){
        _extension = _fullName.substr(found + 1, _fullName.size() - found - 1);
    }
}

std::string FSTool::file::extension(){
    return _extension; 
}

int FSTool::file::create(){
    if(exists()){
        throw fs_exception("file already exists", -1);
    }
    int result = 0;
    std::ofstream *temp; // temp stream object
    temp = new std::ofstream(_fullName.c_str(), std::fstream::binary); // create temp object
    if (!temp->is_open()){
        result = 1; // set result code
    }
    else{
        temp->close(); // close stream
        update();      // update information of file 
    }
    delete temp; // free memory
    return result;
}

int FSTool::file::destroy(){
    if(!exists()){
        throw fs_exception("file not found", -2);
    }
    return remove(_fullName.c_str()); // return result of deleting file
}

bool FSTool::file::empty(){
    return (_size == 0); 
}

std::string FSTool::file::back(){
    return get(_size--);// return lasr line  
}

bool FSTool::file::range(int index){
    if (index < 0 || index > _lines){
        return false; // if index not in range
    }
    else{
        return true; // if index in range
    }
}

int FSTool::file::insert(std::string data, int index, int count){
    if(!exists()){
        throw fs_exception("file not found", -2); // if file exists
    }
    if(index >= _lines || index < 0){
        throw fs_exception("not valid index", -3);
    }
    std::string *_fdata = new std::string[_lines]; //buffer
    for (int i = 0; i < _lines; i++) { // load data in file to array
        _fdata[i] = get(i);
    }
    int * linesTemp = new int(_lines);  // temp count lines
    clear();
    for (int i = 0; i < *linesTemp; i++){
        if (i == index ){
            for (int c = 0; c < count; c++){
                add(data); // add data
            } 
        }
        add(_fdata[i]);
    }
    delete linesTemp;
    delete[] _fdata;
    update(); // update information of file 
    return 0;
}

void FSTool::file::clear(){
    std::ofstream *temp = new std::ofstream(this->_fullName); // temp object
    this->_size = 0;
    delete temp;
}

void FSTool::file::copy(file &source){
    std::ifstream *src; // temp input
    std::ofstream *out; // temp output
    src = new std::ifstream(source.full_name(), std::ios::binary); // open input file
    out = new std::ofstream(_fullName, std::ios::binary); // open source file 
    *out << src->rdbuf(); // write data
    src->close(); // close streams 
	out->close();
    delete src; // free memory
    delete out;
}

int FSTool::file::find(std::string object, int begin, int end){
    if(!exists()){
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
            _end = _lines;
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

std::string FSTool::file::buff(){
    return get(0,_size);
}

#ifdef unix

void FSTool::file::move(std::string path){
    if(!FSTool::exists(path)){
        throw fs_exception("not found", -2);
    }
    file * temp = new binary_file(_name, path);
    temp->copy(*this);
    delete temp;
    destroy();
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

void FSTool::file::copy(std::string path ){
    std::ifstream *src; // temp input
    std::ofstream *out; // temp output
    src = new std::ifstream( path, std::ios::binary); // open input file
    out = new std::ofstream(_fullName, std::ios::binary); // open source file 
    *out << src->rdbuf(); // write data
    src->close(); // close streams 
	out->close();
    delete src; // free memory
    delete out;
}
#endif

mode_t FSTool::file::type(){
    return _type;
}

//////binary file///////

FSTool::binary_file::binary_file(std::string name) : FSTool::file(name) {
    if(exists()){
        update();
    }
}   

FSTool::binary_file::binary_file(std::string name, std::string path) : FSTool::file(name,path) {
    if(exists()){
        update();
    }
}

void FSTool::binary_file::update(){
    std::fstream *obj = new std::fstream(_fullName); // temp stream object
    if(obj->is_open()){
        struct stat data;
        stat(this->_fullName.c_str(), &data);
        _size = data.st_size;
        _lmTime = gmtime(&data.st_mtime); // add time to struct
        _lmTime->tm_mon += 1;             // fix month
        _lmTime->tm_year += 1900;         // fix year
    }
    delete obj;
}

void FSTool::binary_file::write(std::string buff, int position){
    if(!exists()){
       throw fs_exception("file not found", -2); // if file on found 
    }
    if((position > _size || position < 0) && position != END_OF_FILE){
        throw fs_exception("not valid position in file", 3); 
    }
    std::ofstream * bin; // temp stream object
    bin = new std::ofstream(_fullName, std::ios_base::in | std::ios::binary);
    if(bin->good()){
        if(position == END_OF_FILE){
            bin->seekp (std::ios::end);
        }
        else{
            bin->seekp (position, std::ios::beg);
        }
        for (int byte = 0; byte < buff.size(); byte++){
            bin->put(buff[byte]); // write bytes
        }
        bin->close(); // close stream
    }
    update(); // update information of file 
    delete bin;
}

void FSTool::binary_file::write(char* buff, int size, int position = 0){
    if(!exists()){
        throw fs_exception("file not found", -2); // if file on found 
    }
    if(position > _size || position < 0){
        throw fs_exception("not valid position in file", 3); 
    }
    if( size > _size){
        throw fs_exception("not valid size of buffer", 2); 
    }
    std::ofstream * bin; // temp stream object
    bin = new std::ofstream(_fullName, std::ios_base::in | std::ios::binary);
    if(bin->good()){
        bin->seekp(std::ios::beg);
        bin->seekp(position);
        for (int byte = 0; byte < size; byte++){
            bin->put(buff[byte]); // write bytes
        }
        bin->close(); // close stream
    }
    update(); // update information of file 
    delete bin; 
}

std::string FSTool::binary_file::get(int position){
    return get(position, 1);
}

void FSTool::binary_file::get(char* buff, int size, int position = 0){
    if(!exists()){
        throw fs_exception("file not found", -2); // if file on found 
    }
    if(position > _size || position < 0){
        throw fs_exception("not valid position in file", 3); 
    }
    if( size > _size){
        throw fs_exception("not valid size of buffer", 2); 
    }
    std::ifstream *bin; // temp stream object 
    bin = new std::ifstream(this->_fullName, std::ifstream::binary);
    if (bin->good()){
        bin->seekg(std::ios_base::beg); // move to position
        bin->seekg(position);
        bin->read(buff, size); // read bytes
        bin->close(); // close stream
    }
    delete bin;
}

std::string FSTool::binary_file::get(int position, int size){
    if(!exists()){
        throw fs_exception("file not found", -2); // if file on found 
    }
    if(position > _size || position < 0){
        throw fs_exception("not valid position in file", 3); 
    }
    if( size > _size){
        throw fs_exception("not valid size of buffer", 2); 
    }
    std::string result;
    std::ifstream *bin; // temp stream object 
    char * resBuff = new char[size + 1];
    resBuff[size] = '\0';
    bin = new std::ifstream(this->_fullName, std::ifstream::binary);
    if (bin->good()){
        bin->seekg(std::ios_base::beg); // move to position
        bin->seekg(position);
        bin->read(resBuff, size); // read bytes
        bin->close(); // close stream
    }
    result = resBuff;
    delete bin;
    delete[] resBuff;
    return result;
}


//////text file//////


FSTool::text_file::text_file(std::string name) : FSTool::file(name) {
    if(exists()){
        update();
    }
}   

FSTool::text_file::text_file(std::string name, std::string path) : FSTool::file(name,path) {
    if(exists()){
        update();
    }
}

void FSTool::text_file::update(){
    std::fstream *obj = new std::fstream(_fullName); // temp stream object
    if(obj->is_open()){
        struct stat data;
        stat(this->_fullName.c_str(), &data);
        _lmTime = gmtime(&data.st_mtime); // add time to struct
        _lmTime->tm_mon += 1;             // fix month
        _lmTime->tm_year += 1900;         // fix year
        _size = 0;
        std::string *buf = new std::string; //temporary string for getline
        while (getline(*obj, *buf)){
            this->_size++;
        }
        obj->close(); // close file
        delete buf;
    }
    delete obj;
}

std::string FSTool::text_file::get(int index){
    if(!exists()){
        throw fs_exception("file not found", -2); // if file exists
    }
    if(index >= _size || index < 0){
        throw fs_exception("not valid index", -3);
    }
    std::fstream * object; // temp stream object
    object = new std::fstream(_fullName, std::fstream::out | std::fstream::in | std::fstream::binary);
    std::string result;               // result
    int *iter = new int(0);           // temporary counter
    while (getline(*object, result)){ // find index
        if (*iter == index)
			break;
		(*iter)++;
    }
    object->close();//close file
	delete iter;
    delete object;
	return result;
}

void FSTool::binary_file::get(char* buff, int size, int position = 0){
    if(!exists()){
        throw fs_exception("file not found", -2); // if file on found 
    }
    if(position > _size || position < 0){
        throw fs_exception("not valid position in file", 3); 
    }
    if( size > _size){
        throw fs_exception("not valid size of buffer", 2); 
    }
    std::ifstream *bin; // temp stream object 
    std::string _result;
    std::string _buf; 
    bin = new std::ifstream(this->_fullName, std::ifstream::out | std::ifstream::binary);
    if (bin->good()){
        int *iter = new int(0);           // temporary counter
        while (getline(*bin, _buf)){ // find index
            if (*iter >= position)
		    	_result += _buf + "\n";
		    (*iter)++;
            if(*iter == position + size){
                break;
            }
        }
        bin->close(); // close stream
        delete iter;
    }
    delete bin;
}

std::string FSTool::text_file::get(int position){
    return get(position, 1);
}

void FSTool::text_file::write(std::string buff, int position){
    if(!exists()){
        throw fs_exception("file not found", -2); // if file exists
    }
    if(position >= _size || position < 0){
        throw fs_exception("not valid index", -3);
    }
    std::string *_buff = new std::string[_size]; // temp buffeer
    for(int i = 0; i < _size; i++){
        _buff[i] = get(i); // load file data to buff 
    }
    _buff[position] = buff; // rewrite line 
    int *linesTemp = new int(_size); 
    clear(); // delete data in file
    std::ofstream curFile(_fullName,std::fstream::app | std::fstream::binary);
    for(int i = 0; i < *linesTemp; i++){
        curFile << _buff[i] << std::endl; // load buf to file  
    }
    curFile.close();
    delete[] _buff;
    delete linesTemp;
    update(); // update information of file 
}

void FSTool::text_file::write(char* buff, int size, int position ){
    if(!exists()){
        throw fs_exception("file not found", -2); // if file exists
    }
    if(position >= _size || position < 0){
        throw fs_exception("not valid index", -3);
    }
    std::string *_buff = new std::string[_size]; // temp buffeer
    for(int i = 0; i < _size; i++){
        _buff[i] = get(i); // load file data to buff 
    }
    _buff[position].insert(0, size, *buff); // rewrite line 
    int *linesTemp = new int(_size); 
    clear(); // delete data in file
    std::ofstream curFile(_fullName,std::fstream::app | std::fstream::binary);
    for(int i = 0; i < *linesTemp; i++){
        curFile << _buff[i] << std::endl; // load buf to file  
    }
    curFile.close();
    delete[] _buff;
    delete linesTemp;
    update(); // update information of file 
}


