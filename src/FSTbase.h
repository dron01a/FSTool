#ifndef __FSO__BASE__H__
#define __FSO__BASE__H__

#ifdef unix // for unix
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#elif defined(WIN32) // for windows
#include "direct.h"
#include "io.h"
#include "dos.h"
#endif
#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include "string.h"
#include "FSexception.h"

namespace FSTool {

    bool search(std::string name); // search file/folder

    typedef std::vector<std::string> strvect;

    //base class for folder and file
    class _base{
    private: 

        // class fields 
        std::string _name;     // name of file system element
        std::string _path;     // path to file system element
        std::string _fullName; // name of file with path
        int _size = 0;         // size of file system element in byte
        
    public:

        // class constructor
        _base(std::string name, std::string path); // construcnors
        _base(std::string name);
        ~_base(); // destructor 

        // virtual methods
        virtual std::string get(int index) = 0;     // get data fov index file system element
        virtual std::string back() = 0;             // return last element
        virtual bool range(int index) = 0;          // check index
        virtual strvect get_elements_of_path() = 0; // return elements of path
        virtual bool exists() = 0;                  // checks the file for existence
        virtual int create() = 0;                   // create file in directory
        virtual int destroy() = 0;                  // delete file
        virtual bool empty() = 0;                   // if file empty
        virtual int find(std::string object, int begin = 0, int end = 0) = 0;

        // non virtual methods
        int size();                           // return size of file/folder in bytes
        std::string full_name();              // return full name
        std::string name();                   // return name
        std::string path();                   // return path
        void rename(std::string newName);     // rename file/folder
        std::string front();                  // return first element
        std::string at(int index);            // get data with check
        void move(std::string path);          // move file/folder to path 

    };
}

#endif