#ifndef __FSO__BASE__H__
#define __FSO__BASE__H__

#include <sys/stat.h>
#include <sys/types.h>
#include <string>
#include <vector>
#include "string.h"

typedef std::vector<std::string> strvect;

namespace FSTool {

    //base struct for folder and file info
    struct _baseINFO{
        std::string type;      // returns the file type depending on the extension
        std::string name;      // name of file system element
        std::string path;      // path to file system element
        std::string full_name; // name of file with path
        int size = 0;          // size of file system element in byte
        int lm_year = 0;       // year of last modification
        int lm_month = 0;      // month of last modification
        int lm_day = 0;        // day of last modification
        int lm_hour = 0;       // hour of last modification
        int lm_min = 0;        // min of last modification
        int lm_sec = 0;        // sec of last modification
    };

    //base class for folder and file
    template <class structTYPE> class _base{ 
    public:

        //virtual functions
        virtual structTYPE get_info() = 0;          // return struct with information
        virtual std::string get(int index) = 0;     // get data fov index file system element
        virtual std::string back() = 0;             // return last element
        virtual bool range(int index) = 0;          // check index
        virtual strvect get_elements_of_path() = 0; // return elements of path
        virtual void update() = 0;                  // update information of object 

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