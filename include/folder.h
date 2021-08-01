#ifndef __FOLDER__H__
#define __FOLDER__H__

#include "FSTbase.h"

namespace FSTool{

    // class for work with folders
    class folder : public FST_object{
    public:

        // class construcnors
        /*
            class constructor
            @param name: name of folder 
            @param path: path of folder 
        */ 
        folder(std::string name, std::string path); 
        
        /*
            class constructor
            @param name: name of folder 
        */
        folder(std::string name);

        // return information of folder
        int folders();  // return count of folders
        int files();    // return count of files
        int elements(); // return count of elements in folder
        int lenght();   // return count of elemtns in parent dir 

        /*
            return name of element in folder from index
            @param index: position
        */
        std::string get(int index); 

        /*
            check index
            @param index: position
        */
        bool range(int index);  

        /*
            find object in folder
            @param object: object from find 
            @param begin: start position 
            @param end: stop position
        */
        int find(std::string object, int begin = 0, int end = 0);
        
        int create();       // create folder in directory
        void clear();       // destroy data in folder 
        int destroy();      // delete folder
        bool empty();       // if folder empty
        void update();      // update information
        std::string back(); // return last element
        strvect content();  // return file`s name`s 

#ifdef unix 
        /*
            move folder to path 
            @param path: move path
        */ 
        void move(std::string path);
        
        /*
            copy folder to path 
            @param path: copy path
        */ 
        void copy(std::string path);
#endif

    private:
        int _folders = 0;  // count of folders in folder
        int _files = 0;    // count of files in folder
        int _elements = 0; // count of elements in folder
        int _length = 0;   // count of elements in main dir
    };

};

#endif