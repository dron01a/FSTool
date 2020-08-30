#include "folder.h"

#include <iostream>

using namespace FSTool;

int main(){
    
    std::cout << dir_information("./test").folders << std::endl; 

    return 0;
}
