# FSTool
Linux/Windows filesystem tools


```C++

// test/main.cpp

#include "file.h"

#include <iostream>


int main(void){
    std::cout<< FSTool::file_information("./12.txt").name << std::endl;
    std::cout<< FSTool::file_information("./12.txt").path << std::endl;
    std::cout<< FSTool::file_information("./12.txt").full_name << std::endl;
    std::cout<< FSTool::file_information("./12.txt").lines << std::endl;
    std::cout<< FSTool::file_information("./12.txt").size << std::endl;
    std::cout<< FSTool::file_information("./12.txt").last_modification << std::endl;
}

```
________________
Output:

12.txt\n
.\n
./12\n
12\n
133\n
1595497344\n
________________


```C++

#include "file.h"

using namespace FSTool;

int main(void){
    file test("test.txt", "./"); // create object
    if(!test.exists()){
        test.create(); // if file not found
    }
    test.add("string 1"); // add strings to file
    test.add("string 2");
    test.add("string 0", 0); // add string to file in start position 
}

```
________________
test.txt 

string 0\n
string 2\n

________________


