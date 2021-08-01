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
        /*
            class constructor
            @param name: name of file 
            @param path: path of file 
        */ 
        file(std::string name, std::string path); 

        /*
            class constructor
            @param name: name of file 
        */
        file(std::string name);                   

        // edit/add data(string) to file 
        /*
            add data to end file
            @param data: information to add to file
        */
        int add(std::string data);
        /*
            edit data in line
            @param data: information to add to file
            @param index: number of line 
        */
        int add(std::string data, int index); 

        // insert string line to file 
        /*
            insert line from index
            @param data: information to add to file
            @param index: number of line
        */ 
        int insert(std::string data, int index);            
        
        /*
            insert line from index
            @param data: information to add to file
            @param index: number of line
            @param count: number of inserts
        */ 
        int insert(std::string data, int index, int count); 

        /*
            clone file content in class object
            @param obj: class object to clone
        */ 
        void copy(file &obj);

#ifdef unix 
        /*
            move file to path 
            @param path: move path
        */ 
        void move(std::string path);
        
        /*
            copy file to path 
            @param path: copy path
        */ 
        void copy(std::string path);
#endif

        //  methods for get bytes in file
        /*
            return buffer equal to size
            @param position: position in file
            @param size: size from bytes to copyng to buffer
        */ 
        std::string buff(int position, int size);      
        /*
            set to char buffer
            @param buff: char buffer 
            @param size: buffer size 
            @param position: position in file
        */     
        void buff(char* buff, int size, int position = 0);
        std::string buff(); // return all bytes in file

        // methods for writing bytes to file
        /*  
            write buff begin position in file
            @param buff: string with information 
            @param position: position in file
        */ 
        void write(std::string buff, int position); 

        /*
            write buff begin position in file
            @param buff: string with information 
        */ 
        void write(std::string buff);

        /*
            write buff begin position in file
            @param buff: char buffer 
            @param size: buffer size 
            @param position: position in file
        */     
        void write(char* buff, int size, int position = 0);

        /*
            return string in file from index
            @param index: number of line
        */
        std::string get(int index); 
        
        /*
            check index
            @param index: position
        */
        bool range(int index);  
        
        /*
            find object in file
            @param object: object from find 
            @param begin: start position 
            @param end: stop position
        */
        int find(std::string object, int begin = 0, int end = 0);
        
        int create();               // create file in directory
        int destroy();              // delete file
        bool empty();               // if file empty
        void update();              // update information
        std::string extension();    // return extension of file
        int lines();                // return count of lines
        std::string back();         // return last string
        void clear();               // deletes all data from the file

    private:
        std::string _extension; // file extension
        int _lines = 0;         // get count strings in file   
    };

};

#endif