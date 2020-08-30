#include "folder.h"

#include <iostream>

using namespace FSTool;

int main(){
    
    folder test("./test");


    for(int i = 0;i != test.get_info().length; i++ ){
        std::cout << test.get(i) << std::endl;
    }

    return 0;
}
