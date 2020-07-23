#ifndef __FSO__BASE__H__
#define __FSO__BASE__H__

#include <string>

namespace FSTool {
    template < class structTYPE > 
    class _base{ //base class for folder and file 
    public:
        //virtual functions
        virtual structTYPE get_info() = 0;      //return struct with information
        virtual std::string get(int index) = 0; //get data fov index file system element
        virtual std::string back() = 0;         // return last element
        virtual bool range(int index);          //check index

        //return first element 
        std::string front(){ 
            return this->get(0); 
        }

        //get data with check
        std::string at(int index) {
            if (range(index)){ 
                return get(index);
            }
            else{
                return "";
            }
        }

    };
}

#endif