#ifndef __FILE__H__
#define __FILE__H__

#include "FSTbase.h"
#include "FSexception.h"
#include <fstream>

namespace FSTool {

    // class for work with files
    class file : public _base {
    private:
        int lines = 0; // get count strings in file
        int resize();  // return file size
    public:
        file(std::string name, std::string path); // construcnors
        file(std::string name);
        ~file();
        bool exists();                           // checks the file for existence
        int create();                            // create file in directory
        int destroy();                           // delete file
        bool empty();                            // if file empty
        std::string get(int index);              // return string in file from index
        std::string back();                      // return last string
        bool range(int index);                   // check index
        int add(std::string data);               // add data to end file
        int add(std::string data, int index);    // redact data in line
        int insert(std::string data, int index); // insert
        int insert(std::string data, int index, int count);
        void clear();                            // deletes all data from the file
        int rename_file(std::string new_name);   // change name of file
        void copy(file &obj);                    // clone data in other file
        void copy(std::string name);             // clone data in other file
        strvect get_elements_of_path();          // return elements of path
        int find(std::string object, int begin = 0, int end = 0); //find object in file
    };

};

#endif