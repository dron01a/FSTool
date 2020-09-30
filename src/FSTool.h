#ifndef __FSTOOL__H__
#define __FSTOOL__H__

#include "FSTbase.h"

namespace FSTool{
    bool is_file(std::string file_name);
    bool is_folder(std::string folder_name);
}; 

#endif