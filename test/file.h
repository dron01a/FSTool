#ifndef __FILE__H__
#define __FILE__H__

#include "FSTbase.h"

#include "sys/stat.h"
#include <fstream>

namespace FSTool {

    //struct with information of file
    struct _finfo{
        _finfo(std::string full_name);  // constructor
        std::string type;               // returns the file type depending on the extension
        std::string name;               // name of file system element
        std::string path;               // path to file system element
        std::string full_name;          // name of file with path
        int size = 0;                   // size of file system element in byte
        int lines = 0;                  // get count strings in file
        time_t last_modification;       // time of last modification
        // time_t create_time;             // time of create file
    };

    // class for work with files
    class file : public _base<_finfo> {
    private:
        _finfo *_info = nullptr;  // struct with info
        int resize();   // return file size
    public:
        file(std::string name, std::string path); // construcnor
        file(std::string name);
        ~file();
        bool exists();                           // checks the file for existence
        int create();                            // create file in directory
        int destroy();                           // delete file
        bool empty();                            // if file empty
        _finfo get_info();                       // return information of file
        std::string get(int index);              // return string in file from index
        std::string back();                      // return last string
        bool range(int index);                   // check index
        int add(std::string data);               // add data to end file
        int add(std::string data, int index);    // redact data in line
        int insert(std::string data, int index); // insert
        int insert(std::string data, int index, int count);
        void clear();                           // deletes all data from the file
        int rename_file(std::string new_name);  // change name of file
        void copy(file &obj);                   // clone data in other file
        void copy(std::string name);            // clone data in other file
        void move(std::string path);            // moves a file to the specified folder
    };

    // function return information of file
    _finfo file_information(std::string file_name);

    // functions find object in file
    int find(std::string file_name, int begin, int end, std::string object);
    int find(std::string file_name, std::string begin, std::string end, std::string object);
    int find(std::string file_name, std::string object);

};

#endif