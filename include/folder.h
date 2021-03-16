#ifndef __FOLDER__H__
#define __FOLDER__H__

#include "FSTbase.h"

namespace FSTool{

    // class for work with folders
    class folder : public FST_object{
    public:
        // class construcnors
        folder(std::string name, std::string path); // parameters: path to folder and name of folder
        folder(std::string name);                   // parameters: name of folder

        // return information of folder
        int folders();  // return count of folders
        int files();    // return count of files
        int elements(); // return count of elements in folder
        int lenght();   // return count of elemtns in parent dir 

        int create();               // create folder in directory
        int destroy();              // delete folder
        bool empty();               // if folder empty
        void update();              // update information
        std::string get(int index); // return name of element in folder from index
        std::string back();         // return last element
        bool range(int index);      // check index
        strvect content();          // return file`s name`s 
        int find(std::string object, int begin = 0, int end = 0); // find object in folder

#ifdef unix 
        // move file to path
        void move(std::string path);
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