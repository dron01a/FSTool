#ifndef __FOLDER__H__
#define __FOLDER__H__

#include "FSTbase.h"

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
    };

    // class for work with files
    class folder{

    };

};

#endif