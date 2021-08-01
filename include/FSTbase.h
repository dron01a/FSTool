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

    /*
        search file/folder
        @param name: name of file/folder
    */ 
    bool exists(std::string name); 

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

        /*
            class constructor
            @param name: name of object 
            @param path: path of object 
        */ 
        FST_object(std::string name, std::string path); 
        
        /*
            class constructor
            @param name: name of object 
        */ 
        FST_object(std::string name);

        // virtual methods
        /*
            get data fov index file system element
            @param index: position
        */
        virtual std::string get(int index) = 0;    

        /*
            check index
            @param index: position
        */
        virtual bool range(int index) = 0;

        /*
            get from index
            @param object: object from find 
            @param begin: start position 
            @param end: stop position
        */
        virtual int find(std::string object, int begin = 0, int end = 0) = 0;

        virtual std::string back() = 0; // return last element
        
        // methods for controlling the state of a folder/file
        virtual int create() = 0;  // create file/folde in directory
        virtual int destroy() = 0; // delete file/folder
        virtual bool empty() = 0;  // if file/folder empty
        virtual void update() = 0; // update info of file/folder
        virtual void clear() = 0;  // dastroy data in object

#ifdef unix 
        // moving/copy methods
        /*
            copy file/folder to path 
            @param path: copy path
        */ 
        virtual void copy(std::string path) = 0; 
        
        /*
            move file/folder to path 
            @param path: move path
        */ 
        virtual void move(std::string path) = 0;  
#endif
        // non virtual methods
        /*
            rename file/folder
            @param newName: new name of file/folder
        */ 
        void rename(std::string newName); 

        /*
            get data with check
            @param index: position
        */ 
        std::string at(int index);        
        
        int size();                       // return size of file/folder in bytes
        std::string full_name();          // return full name
        std::string name();               // return name
        std::string path();               // return path
        std::string front();              // return first element
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