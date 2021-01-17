#include "src/FSTool.h"
#include <iostream>

using namespace std;
using namespace FSTool;

int main(){
    filesystemFree a("src");
    std::cout << a.size() << std::endl;
    for (int i = 0; i <  a.countNodes(); i++){
        std::cout << a.get(i).name() <<std::endl;
    } 
}