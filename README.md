# FSTool
Linux/Windows filesystem tools<br/>
Сontains classes and functions for working with the file system.<br/>

#### clone

'git clone https://github.com/ARmodRey/FSTool.git'

## Examples

class file(src/file.cpp) --> class for work with files<br/>
files open binary mode, but work with lines and buffer`s<br/>

```C++
#include "file.h"

using namespace FSTool;

int main(){
    file myFile("file.txt");
    if(!myFile.exists()){
        myFile.create();
    }
    myFile.add("string 0");
    myFile.add("string 1");
    myFile.add("string", 0);
    return 0;
}
```

class folder(src/folder.cpp) --> class for work with folder<br/>

```C++
#include "folder.h"

using namespace FSTool;

int main(){
    folder myFolder("folder");
    if(!myFolder.exists()){
        myFolder.create();
    }
    return 0;
}
```