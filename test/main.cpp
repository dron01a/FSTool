#include "file.h"

#include <iostream>


int main(){
    std::cout<< FSTool::file_information("./12.txt").name << std::endl;
    std::cout<< FSTool::file_information("./12.txt").path << std::endl;
    std::cout<< FSTool::file_information("./12.txt").full_name << std::endl;
    std::cout<< FSTool::file_information("./12.txt").lines << std::endl;
    std::cout<< FSTool::file_information("./12.txt").size << std::endl;
    std::cout<< FSTool::file_information("./12.txt").last_modification << std::endl;
}
