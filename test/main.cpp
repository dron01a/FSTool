#include "file control.h"

#include <iostream>


int main(){
    file_control::folder fl{ "1" , "." };
    file_control::folder fl1{ "2" , "." };
    fl1.create();
    std::cout<< fl.c_files() << std::endl;
}
