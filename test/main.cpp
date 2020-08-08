#include "file.h"

#include <iostream>

using namespace FSTool;

int main(){
    file a("3.txt", "./1");

    if(!a.exists()){
        std::cout << "нету\n";
    }

    a.create();

    a.copy("./1/1");

    a.move("./2/");


    return 0;
}
