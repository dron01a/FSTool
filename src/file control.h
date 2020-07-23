#ifndef __FSO__BASE__H__
#define __FSO__BASE__H__

#include <string>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>
#ifdef unix 
#include <string.h>
#include <unistd.h>
#include "dirent.h"
#elif defined(WIN32) // for windows
#include "direct.h"
#include "io.h"
#include "dos.h"
#endif

namespace file_control{ 
    //classes 
    class fs_element; // base class for private 
    class fs_base; //base class 
    class file;//for files
    class folder;// for folders
    //functions for work with paths
    bool is_file(const char* name);
    bool is_folder(const char* name); 
    bool search(std::string path);//find in currient directory
    bool search(std::string path, std::string object);//finding obgect(file or folger) in directory
    void get_elements_of_path(std::string source, std::string parts_of_string[]); // delim path to steps
    void make_path(std::string path);//create more steps path 
    int duplicate(file_control::file& _source, file_control::file& _object);
    int duplicate(file_control::folder& _source, file_control::folder& _object);
    // operators
    bool operator==(file_control::fs_base& a, file_control::fs_base& b);
    bool operator!=(file_control::fs_base& a, file_control::fs_base& b);
    bool operator>=(file_control::fs_base& a, file_control::fs_base& b);
    bool operator<=(file_control::fs_base& a, file_control::fs_base& b);
    bool operator>(file_control::fs_base& a, file_control::fs_base& b);
    bool operator<(file_control::fs_base& a, file_control::fs_base& b);
}

class file_control::fs_element{ // fs -----> file system
protected:
    std::string name; // name of file system element
    std::string path; // path to file system element
    std::string full_name; // name of file with path
    int size; // size of file system element in byte
    virtual void calc_size() = 0; // clalculate size of file system element 
};

class file_control::fs_base : protected file_control::fs_element{ // fs -----> file system
public:
    fs_base(std::string name, std::string path); //class constructor
    ~fs_base(){}
    std::string front();
    std::string get_name();// return name of file system element
    std::string get_path();// return path of file system element
    std::string get_full_name();// return name with path of file system element
    int get_size();//return size
    virtual std::string get(int index) = 0;//get data fov index file system element
    virtual std::string back() = 0;// return last element
    bool range(int index); //check index 
    std::string at(int index);// get data with check
};

class file_control::file : public file_control::fs_base{
private:
    int lines = 0; 
    std::fstream object;// stream obgect
    void calc_size() override;// count lines in file and size of file  
public:
    file(std::string name, std::string path);// construcnor
    ~file(){}
    int add(std::string data);// add data to end file 
    int add(std::string data, int index);// redact data in line
    int insert(std::string data, int index);//insert 
	int insert(std::string data, int index, int count);
	void clear();//deletes all data from the file
	void rename_file(std::string new_name); //change name of file 
	void copy(file & obj); //clone data in other file
	void move(std::string path);//moves a file to the specified folder
    int c_lines();// return count strings in file 
    std::string get(int index);// return string in file from index
    std::string back();
};

class file_control::folder : public file_control::fs_base{
private:
    int _files = 0;
    int _folders = 0;
    int _elements = 0;
    void calc_size() override; // claculate size of folder 
public:
    folder(std::string name, std::string path);//constructor
    std::string get(int index) override;//get name of element
    std::string back();//return name of last element
    bool verification();//checks the folder for existence
	void create();// create folder in directory 
    int c_files();// get number of files
    int c_folders();// return number of folders
    int c_elements();//return number of elements
};

#endif