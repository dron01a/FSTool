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
   
}; 

#endif