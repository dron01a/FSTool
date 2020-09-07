#include "folder.h"

#include <iostream>

using namespace FSTool;

int main(){
    
    folder test("./2/3/4");

    test.create();

    for(int i = 0;i != test.get_info().length; i++ ){
        std::cout << test.get(i) << std::endl;
    }

    return 0;
}
