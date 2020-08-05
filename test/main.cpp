#include "file.h"

#include <iostream>

using namespace FSTool;

int main(){
    file a("12.txt", ".");

    if(!a.exists()){
        std::cout << "нету\n";
    }

    std::cout << a.destroy() << "\n";
    std::cout << a.create() << "\n";

    // std::cout << a.add("жопа") << "\n";

    // std::cout << a.add("жопа", 21) << "\n";

    std::cout << a.add("жопа") << "\n";

    //std::cout << a.get(0) << "\n";

    std::cout << a.add("жопа") << "\n";

    std::cout << a.insert("_+_+_+_жопа", 1, 2) << "\n";

    return 0;
}
