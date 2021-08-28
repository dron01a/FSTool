#ifndef __FILE__H__
#define __FILE__H__

#include "FSTbase.h"
#include "FSexception.h"
#include <fstream>

#define END_OF_FILE -1

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

        // // edit/add data(string) to file 
        // /*
        //     add data to end file
        //     @param data: information to add to file
        // */
        // int add(std::string data);
        // /*
        //     edit data in line
        //     @param data: information to add to file
        //     @param index: number of line 
        // */
        // int add(std::string data, int index); 

       // /*
       //     insert line from index
       //     @param data: information to add to file
       //     @param index: number of line
       //     @param count: number of inserts
       // */ 
       // virtual int insert(std::string data, int index, int count = 1) = 0; 

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

        std::string buff(); // return all bytes in file

        // methods for writing bytes to file
        /*  
            write buff begin position in file
            @param buff: string with information 
            @param position: position in file
        */ 
        virtual void write(std::string buff, int position = END_OF_FILE) = 0; 

        /*
            write buff begin position in file
            @param buff: char buffer 
            @param size: buffer size 
            @param position: position in file
        */     
        virtual void write(char* buff, int size, int position = 0) = 0;

         // virtual methods
        /*
            get data fov index file system element
            @param index: position
        */
        virtual std::string get(int index) = 0;   
        /*
            return buffer equal to size
            @param position: position in file
            @param size: size from bytes to copyng to buffer
        */ 
        virtual std::string get(int position, int size); 

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
        
        virtual void update() = 0;   // update information
        
        int create();            // create file in directory
        int destroy();           // delete file
        bool empty();            // if file empty
        std::string extension(); // return extension of file
        int lines();             // return count of lines
        std::string back();      // return last string
        void clear();            // deletes all data from the file
        mode_t type();           // return type of file
    private:
        std::string _extension; // file extension
        mode_t _type;           // constant of file type 
    };



    class binary_file : public file {
    public:

        // methods for writing bytes to file
        /*  
            write buff begin position in file
            @param buff: string with information 
            @param position: position in file
        */ 
        void write(std::string buff, int position = END_OF_FILE);
        /*
            write buff begin position in file
            @param buff: char buffer 
            @param size: buffer size 
            @param position: position in file
        */     
        void write(char* buff, int size, int position = 0);

        /*
            insert line from index
            @param data: information to add to file
            @param index: number of line
            @param count: number of inserts
        */ 
       // int insert(std::string data, int index, int count = 1); 
        
        /*
            get data fov index file system element
            @param index: position
        */
        std::string get(int index);  
        /*
            return buffer equal to size
            @param position: position in file
            @param size: size from bytes to copyng to buffer
        */ 
        std::string get(int position, int size); 
        /*
            set to char buffer
            @param buff: char buffer 
            @param size: buffer size 
            @param position: position in file
        */     
        void get(char* buff, int size, int position = 0);

        /*
            find object in file
            @param object: object from find 
            @param begin: start position 
            @param end: stop position
        */
        //int find(std::string object, int begin = 0, int end = 0);

        void update();   // update information
    private:
        // class construcnors
        /*
            class constructor
            @param name: name of file 
            @param path: path of file 
        */ 
        binary_file(std::string name, std::string path); 

        /*
            class constructor
            @param name: name of file 
        */
        binary_file(std::string name);      
        friend class file;
    };



    class text_file : public file {
    public:

        // methods for writing bytes to file
        /*  
            write buff begin position in file
            @param buff: string with information 
            @param position: position in file
        */ 
        void write(std::string buff, int position = END_OF_FILE);
        /*
            write buff begin position in file
            @param buff: char buffer 
            @param size: buffer size 
            @param position: position in file
        */     
        void write(char* buff, int size, int position = 0);

        /*
            insert line from index
            @param data: information to add to file
            @param index: number of line
            @param count: number of inserts
        */ 
        //int insert(std::string data, int index, int count = 1); 

        /*
            return string in file from index
            @param index: number of line
        */
        std::string get(int index); 
         /*
            return buffer equal to size
            @param position: position in file
            @param size: size from bytes to copyng to buffer
        */ 
        std::string get(int position, int size); 
        /*
            set to char buffer
            @param buff: char buffer 
            @param size: buffer size 
            @param position: position in file
        */     
        void get(char* buff, int size, int position = 0);

        /*
            find object in file
            @param object: object from find 
            @param begin: start position 
            @param end: stop position
        */
        //int find(std::string object, int begin = 0, int end = 0);

        void update();   // update information
    private:
        // class construcnors
        /*
            class constructor
            @param name: name of file 
            @param path: path of file 
        */ 
        text_file(std::string name, std::string path); 

        /*
            class constructor
            @param name: name of file 
        */
        text_file(std::string name);      

        friend class file;
    };

};

#endif