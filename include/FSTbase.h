#ifndef __FSO__BASE__H__
#define __FSO__BASE__H__

#ifdef unix // for unix
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "dirent.h"
#include <fcntl.h>
#elif defined(WIN32) // for windows
#include "direct.h"
#include "io.h"
#include "dos.h"
#endif
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>
#include "string.h"
#include "FSexception.h"

namespace FSTool {

    bool exists(std::string name); // search file/folder

    typedef std::vector<std::string> strvect;

    //base class with information
    class _baseINFO{
    protected:
        // class fields 
        std::string _name;      // name of file system element
        std::string _path;      // path to file system element
        std::string _fullName;  // name of file with path
        int _size = 0;          // size of file system element in byte
        tm * _lmTime = nullptr; // time of last modification  
    };

    //base class for folder and file
    class FST_object : protected _baseINFO{
    public:

        // class constructors
        FST_object(std::string name, std::string path); // parameters: path to object and name
        FST_object(std::string name);                   // parameters: name

        // virtual methods
        virtual std::string get(int index) = 0;     // get data fov index file system element
        virtual std::string back() = 0;             // return last element
        virtual bool range(int index) = 0;          // check index
        virtual int find(std::string object, int begin = 0, int end = 0) = 0;
        
        // methods for controlling the state of a folder/file
        virtual int create() = 0;  // create file/folde in directory
        virtual int destroy() = 0; // delete file/folder
        virtual bool empty() = 0;  // if file/folder empty
        virtual void update() = 0; // update info of file/folder

#ifdef unix 
        // moving/copy methods
        virtual void copy(std::string path) = 0; // copy file/folder
        virtual void move(std::string path) = 0; // move file/folder to path 
#endif
        // non virtual methods
        int size();                       // return size of file/folder in bytes
        std::string full_name();          // return full name
        std::string name();               // return name
        std::string path();               // return path
        void rename(std::string newName); // rename file/folder
        std::string front();              // return first element
        std::string at(int index);        // get data with check
        bool exists();                    // checks the file/folder for existence
        tm * last_modification();         // return tm struct with date of last modification
        strvect pathSteps();              // return steps of path
    private:
        // update _fullName after using methods rename, move
        void _updateFullName(std::string path, std::string name);

    };

    // comparison operators 
    bool operator==(FST_object & objectA, FST_object & objectB);
    bool operator>=(FST_object & objectA, FST_object & objectB);
    bool operator<=(FST_object & objectA, FST_object & objectB);
    bool operator!=(FST_object & objectA, FST_object & objectB);
    bool operator<(FST_object & objectA, FST_object & objectB); 
    bool operator>(FST_object & objectA, FST_object & objectB);
}

#endif