#ifndef __FOLDER__H__
#define __FOLDER__H__

#ifdef unix // for unix 
#include "dirent.h"
#endif
#include "FSTbase.h"

namespace FSTool{

    // class for work with folders
    class folder : public _base{
    private:
        int _folders = 0;  // count of folders in folder
        int _files = 0;    // count of files in folder
        int _elements = 0; // count of elements in folder
        int _length = 0;   // count of elements in main dir
    public:
        folder(std::string name, std::string path); // construcnor
        folder(std::string name);
        int folders();                  // return count of folders
        int files();                    // return count of files
        int elements();                 // return count of elements in folder
        int lenght();                   // return count of elemtns in parent dir 
        bool exists();                  // checks the folder for existence
        int create();                   // create folder in directory
        int destroy();                  // delete folder
        bool empty();                   // if folder empty
        void update();                  // update information
        std::string get(int index);     // return name of element in folder from index
        std::string back();             // return last element
        bool range(int index);          // check index
        strvect get_elements_of_path(); // return elements of path
        strvect get_content_list();     // return file`s name`s 
        int find(std::string object, int begin = 0, int end = 0); // find object in folder
    };

};

#endif