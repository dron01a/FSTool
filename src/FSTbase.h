#ifndef __FSO__BASE__H__
#define __FSO__BASE__H__

#include <sys/stat.h>
#include <sys/types.h>
#include <string>
#include <vector>
#include "string.h"

typedef std::vector<std::string> strvect;

namespace FSTool {
    template < class structTYPE > 
    class _base{ //base class for folder and file 
    public:
    
        //virtual functions
        virtual structTYPE get_info() = 0;          // return struct with information
        virtual std::string get(int index) = 0;     // get data fov index file system element
        virtual std::string back() = 0;             // return last element
        virtual bool range(int index) = 0;          // check index
        virtual strvect get_elements_of_path() = 0; // return elements of path

        //return first element 
        std::string front(){ 
            return this->get(0); 
        }

        //get data with check
        std::string at(int index) {
            if ( this->range(index) ){
                return this->get(index);
            }
            else{
                //throw std::out_of_range;
            } 
        }

    };
}

#endif