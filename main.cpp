#include "src/FSTool.h"

int main(){
    FSTool::filesystemFree fr("src");
    printf("%s\n", fr.getPath("1").c_str());
}