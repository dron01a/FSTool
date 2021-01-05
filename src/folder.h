#ifndef __FOLDER__H__
#define __FOLDER__H__

#ifdef unix // for unix 
#include <unistd.h>
#include "dirent.h"
#elif defined(WIN32) // for windows
#include "direct.h"
#include "io.h"
#include "dos.h"
#endif
#include "FSTbase.h"
#include "FSexception.h"
#include <vector>

namespace FSTool{

    // class for work with folders
    class folder : public _base{
    private:
        int _folders = 0;  // count of folders in folder
        int _files = 0;    // count of files in folder
        int _elements = 0; // count of elements in folder
        int _length = 0;   // count of elements in maindir
    public:
        folder(std::string name, std::string path); // construcnor
        folder(std::string name);
        ~folder();
        bool exists();                  // checks the folder for existence
        int create();                   // create folder in directory
        int destroy();                  // delete folder
        bool empty();                   // if folder empty
        std::string get(int index);     // return name of element in folder from index
        std::string back();             // return last element
        bool range(int index);          // check index
        strvect get_elements_of_path(); // return elements of path
        strvect get_content_list();     // return file`s name`s 
        int find(std::string object, int begin = 0, int end = 0); // find object in folder
    };

};

#endif