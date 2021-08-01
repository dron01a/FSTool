#ifndef __FSTOOL__H__
#define __FSTOOL__H__

#include "FSTbase.h"
#include "file.h"
#include "folder.h"

namespace FSTool{
    
    /*
        return type-checked object
        @param name: name of object
    */ 
    FST_object* open(std::string name);

    /*
        return true if object is file
        @param name: name of object
    */
    bool is_file(std::string name);
    
    /*
        return true if object is folder
        @param name: name of object
    */
    bool is_folder(std::string name);

    /*
        function find object in fs element 
        @param name: name of file/folder
        @param object: object for find 
        @param begin: start position 
        @param end: stop position
    */ 
    int find(std::string name, std::string object, int begin = 0, int end = 0);

    // class for work with filesystem free 
    class FSToken{
    public:
        /*
            class constructor
            @param name: name of file/folder
        */
        FSToken(std::string name);

        /*
            class destructor
        */  
        ~FSToken();             

        /*
            get count of nodes with name
            @param name: name of file 
            @param allEnt: flag of entryi`s
        */                        
        int count(std::string name, bool allEnt = false);

        /*
            return child filesystem free
            @param index: index of node in root
        */
        FSToken get(int index); 

        /*
            return class object of node
            @param index: index of node in root
        */
        FST_object* getObject(int index); 

        /*
            return path to object
            @param name: name of object
        */ 
        std::string getPath(std::string name); 
        
        /*
            check element with this name
            @param name: name of object
        */ 
        bool have(std::string name);             

        int size();              // return size
        int countNodes();        // get size of _subNodes
        std::string type();      // return type of node 
        std::string name();      // return name of node 
        std::string full_name(); // return full name 
    private:
        int _size = 0;                  // size of element
        std::string _type;              // type of node 
        std::string _name;              // name of node 
        std::string _fullName;          // name with path
        std::vector<FSToken> _subNodes; // child nofes
    };
}; 

#endif