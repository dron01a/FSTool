#include "src/file.h"
#include "iostream"

using namespace FSTool;

int main(){
    file a("1");
    try{
        if(!a.exists()){
            a.create();
            a.write("000000000000000");
        }
        a.write("lenin",9);
        std::cout << a.buff(9,5) <<std::endl;
    }
    catch(fs_exception exc){
        std::cout << exc.get_error_code() << " " << exc.get_message() <<std::endl;
    }
    
}