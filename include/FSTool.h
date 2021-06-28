#ifndef __FSTOOL__H__
#define __FSTOOL__H__

#include "FSTbase.h"
#include "file.h"
#include "folder.h"

namespace FSTool{
    
    // return type-checked object
    FST_object* open(std::string name);

    //check functions 
    bool is_file(std::string file_name);
    bool is_folder(std::string folder_name);

    // function find object in fs element 
    int find(std::string name, std::string object, int begin = 0, int end = 0);

    // class for work with filesystem free 
    class FSToken{
    public:
    void a();
        FSToken(std::string name);               // class constructor 
        ~FSToken();                              // class destructor 
        int countNodes();                        // get size of _subNodes
        int size();                              // return size of bytes
        FSToken get(int index);                  // return child filesystem free
        FST_object* getObject(int index);        // return class object of node 
        std::string type();                      // return type of node 
        std::string name();                      // return name of node 
        std::string full_name();                 // return full name 
        std::string getPath(std::string object); // return path to object
        bool have(std::string name);             // check element with this name
    private:
        int _size = 0;                  // size of element
        std::string _type;              // type of node 
        std::string _name;              // name of node 
        std::string _fullName;          // name with path
        std::vector<FSToken> _subNodes; // child nofes
    };
    
}; 

#endif