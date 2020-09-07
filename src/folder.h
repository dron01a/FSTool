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
    struct _dirinfo{
        _dirinfo(std::string full_name); // constructor
        std::string name;                // name of file system element
        std::string path;                // path to file system element
        std::string full_name;           // name of file with path
        int size = 0;                    // size of file system element in byte
        int folders = 0;                 // count of folders in folder
        int files = 0;                   // count of files in folder
        int elements = 0;                // count of elements in folder
        int lm_year = 0;                 // year of last modification
        int lm_month = 0;                // month of last modification
        int lm_day = 0;                  // day of last modification
        int lm_hour = 0;                 // hour of last modification
        int lm_min = 0;                  // min of last modification
        int lm_sec = 0;                  // sec of last modification
        int length = 0;                  // count of elements in maindir
    };

    // function return information of folder
    _dirinfo dir_information(std::string full_name);

    // class for work with files
    class folder : public _base<_dirinfo>{
    private:
        _dirinfo *_info = nullptr; // struct with info
    public:
        folder(std::string name, std::string path); // construcnor
        folder(std::string name);
        ~folder();
        bool exists();                  // checks the folder for existence
        int create();                   // create folder in directory
        int destroy(char mode);         // delete folder
        bool empty();                   // if folder empty
        _dirinfo get_info();            // return information of folder
        std::string get(int index);     // return name of element in folder from index
        std::string back();             // return last element
        bool range(int index);          // check index
        strvect get_elements_of_path(); // return elements of path
    };

};

#endif