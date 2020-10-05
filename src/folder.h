#ifndef __FOLDER__H__
#define __FOLDER__H__

#ifdef unix // for unix 
#include <unistd.h>
#include "dirent.h"
#elif defined(WIN32) // for windows
#include "direct.h"
#include "io.h"dd
#include "dos.h"
#endif
#include "FSTbase.h"
#include <vector>

namespace FSTool{

    //struct with information of file
    struct _dirinfo : _baseINFO{
        _dirinfo(std::string full_name); // constructor
        int folders = 0;                 // count of folders in folder
        int files = 0;                   // count of files in folder
        int elements = 0;                // count of elements in folder
        int length = 0;                  // count of elements in maindir
    };

    // function return information of folder
    _dirinfo dir_information(std::string full_name);

    // class for work with folders
    class folder : public _base{
    private:
        _dirinfo *_info = nullptr; // struct with info
    public:
        folder(std::string name, std::string path); // construcnor
        folder(std::string name);
        ~folder();
        bool exists();                  // checks the folder for existence
        int create();                   // create folder in directory
        int destroy();         // delete folder
        bool empty();                   // if folder empty
        _dirinfo get_info();            // return information of folder
        std::string get(int index);     // return name of element in folder from index
        std::string back();             // return last element
        bool range(int index);          // check index
        strvect get_elements_of_path(); // return elements of path
    };

};

#endif