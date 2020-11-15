#ifndef __FS_EXP__H__
#define __FS_EXP__H__

#include <string>

namespace FSTool{   

    //class describing file system exceptions
    class fs_exception{
    private:
        std::string *message;
        int *error_code; 
    public:
        fs_exception(std::string message, int error_code){
            this->message = new std::string(message);
            this->error_code = new int(error_code);
        }
        ~fs_exception(){
            delete message;
            delete error_code;
        }
        int get_error_code(){ 
            return *error_code;
        }
        std::string get_message(){
            return *message;
        }
    };

};

#endif