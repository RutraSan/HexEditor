#ifndef EDITOR_H
#define EDITOR_H

#include <windows.h>

#define SAVE 1 

class Editor {
    private:
        HANDLE hConsole;
        COORD cursor_position;
        unsigned char* data;
        int size;

    public:
        Editor(unsigned char* data, int size);

        // the fucntion that runs the editor
        int run();
        int edit_byte();

        // size getter
        int getSize();

};

#endif