#ifndef __FILE__H__
#define __FILE__H__

#include "FSTbase.h"
#include "FSexception.h"
#include <fstream>

namespace FSTool {

    // class for work with files
    class file : public FST_object {
    public:
        // class construcnors
        file(std::string name, std::string path); // parameters: path to file and name of file
        file(std::string name);                   // parameters: name of file

        // edit/add data(string) to file 
        int add(std::string data);               // add data to end file
        int add(std::string data, int index);    // edit data in line

        // insert string line to file 
        int insert(std::string data, int index);            // insert line from index
        int insert(std::string data, int index, int count); // insert count line`s from index

        // clone data in other file
        void copy(file &obj);        // clone file content in class object     

#ifdef unix 
        // move file to path
        void move(std::string path);
        void copy(std::string path);
#endif

        //  methods for get bytes in file
        std::string buff();                       // return all bytes in file
        std::string buff(int position, int size); // return buffer equal to size

        // methods for writing bytes to file
        void write(std::string buff, int position); // write buff to special position in file
        void write(std::string buff);               // write buff begin position in file

        int create();               // create file in directory
        int destroy();              // delete file
        bool empty();               // if file empty
        void update();              // update information
        std::string extension();    // return extension of file
        int lines();                // return count of lines
        std::string get(int index); // return string in file from index
        std::string back();         // return last string
        bool range(int index);      // check index
        void clear();               // deletes all data from the file
        int find(std::string object, int begin = 0, int end = 0); // find object in file

    private:
        std::string _extension; // file extension
        int _lines = 0;         // get count strings in file
    
    };

};

#endif