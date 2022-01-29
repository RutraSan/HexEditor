#include <iostream>
#include <fstream>
#include "editor.h"

int main(int argc, char *argv[]) {
    std::string input = "";
    char* data;

    // file input
    if (argc < 2) {
        std::cout << "Enter file path:" << std::endl;
        std::cin >> input;
        getchar();
    }
    // file name as argument
    else {
        input = argv[1];
    }
    
    // open file and read it's contents
    std::fstream file (input, std::ios::binary | std::ios::in);
    if (file.is_open()) {
        // get file size
        file.seekg (0, std::ios_base::end);
        int size = file.tellg();
        file.seekg(0, std::ios_base::beg);
        
        // read file contents
        data = new char[size];
        file.read(data, size);
        file.close();

        Editor ed = Editor((unsigned char*) data, size);
        if (ed.run() == SAVE) {
            std::fstream file (input, std::ios::binary | std::ios::out);
            file.write(data, size);
            file.close();
        };
        delete data;
    }
    else std::cout << "Unable to open file" << std::endl;

    return 0;
}