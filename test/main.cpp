#include "file.h"

#include <iostream>

using namespace FSTool;

int main(){
    file a("3.txt", ".");

    if(!a.exists()){
        std::cout << "нету\n";
    }

    a.create();

    std::cout << a.get_info().lm_year << std::endl;

    std::cout << a.get_info().lm_month << std::endl;

    std::cout << a.get_info().lm_day << std::endl;

    std::cout << a.get_info().lm_hour << std::endl;

    return 0;
}
