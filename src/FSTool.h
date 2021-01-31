#ifndef __FSTOOL__H__
#define __FSTOOL__H__

#include "FSTbase.h"
#include "file.h"
#include "folder.h"

namespace FSTool{
    
    //check functions 
    bool is_file(std::string file_name);
    bool is_folder(std::string folder_name);

    // function find object in fs element 
    int find(std::string name, std::string object, int begin = 0, int end = 0);

    // class for work with filesystem free 
    class filesystemFree{
    public:
        filesystemFree(std::string name);        // class constructor 
        ~filesystemFree();                       // class destructor 
        int countNodes();                        // get size of _subNodes
        int size();                              // return size of bytes
        filesystemFree get(int index);           // return child filesystem free
        std::string type();                      // return type of node 
        std::string name();                      // return name of node 
        std::string full_name();                 // return full name 
        std::string getPath(std::string object); // return path to object
        bool have(std::string name);             // check element with this name
    private:
        int _size = 0;                         // size of element
        std::string _type;                     // type of node 
        std::string _name;                     // name of node 
        std::string _fullName;                 // name with path
        std::vector<filesystemFree> _subNodes; // child nofes
    };
    
}; 

#endif