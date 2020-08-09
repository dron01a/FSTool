#include "file.h"

#include <iostream>

using namespace FSTool;

int main(){
    file a("3.txt", ".");

    if(!a.exists()){
        std::cout << "нету\n";
    }

    std::cout << find("./2/3.txt",0,5,"жопа") << std::endl;

    std::cout << find("./2/3.txt",0,5,"жопа") << std::endl;

    std::cout << find("./2/3.txt",0,5,"жопа") << std::endl;

    std::cout << find("./2/3.txt",0,5,"жопа") << std::endl;

    return 0;
}
