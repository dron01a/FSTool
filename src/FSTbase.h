#ifndef __FSO__BASE__H__
#define __FSO__BASE__H__

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include <filesystem>
#include "string.h"
#include "FSexception.h"

namespace FSTool {

    typedef std::vector<std::string> strvect;

    //base class for folder and file
    class _base{
    private: 

        // class fields
        std::string _name;     // name of file system element
        std::string _path;     // path to file system element
        std::string _fullName; // name of file with path
        int _size = 0;         // size of file system element in byte
        
        // struct with information of last modification
        struct _lastModification {
            int _lmYear = 0;       // year of last modification
            int _lmMonth = 0;      // month of last modification
            int _lmDay = 0;        // day of last modification
            int _lmHour = 0;       // hour of last modification
            int _lmMin = 0;        // min of last modification
            int _lmSec = 0;        // sec of last modification
        } _lM;
        
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
        virtual void move(std::string path) = 0;    // move object to path 
        virtual int find(std::string object, int begin = 0, int end = 0) = 0;

        // non virtual methods
        int size();                           // return size of file/folder in bytes
        std::string full_name();              // return full name
        std::string name();                   // return name
        std::string path();                   // return path
        _lastModification lastModification(); // return year of last modification
        void rename(std::string newName);     // rename file/folder
        std::string front();                  // return first element
        std::string at(int index);            // get data with check

    };
}

#endif