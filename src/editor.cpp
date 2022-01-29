#include "editor.h"
#include <iostream>
#include <string>
#include <cstdio>
#include <conio.h>
#include<algorithm>

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define BACKSPACE 8
#define ENTER 13
#define ESC 27

std::string intToHex(int num);


Editor::Editor(unsigned char* data, int size): data(data), size(size) {
    this->hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    this->cursor_position = cursor_position = {0, 0};
}

/**
 * Runs the program.
 */
int Editor::run() {
    DWORD res;
    system("cls");
    // print data
    for(int i = 0; i < this->size; i++) {
            if(i > 7 && i % 8 == 0) {
                std::cout << std::endl;
            }
            printf("%02x ", this->data[i]);
    }
    // print data as ascii

    COORD cursor = {24, 0};
    SetConsoleCursorPosition(hConsole, cursor);
    for (int i = 0; i < this->size; i++) {
        // new line
        if(i > 7 && i % 8 == 0) {
            std::cout << " |";
            cursor.Y++;
            SetConsoleCursorPosition(hConsole, cursor);
        }
        // add pipe character
        if(i % 8 == 0) {
            std::cout << "| ";
        }
        if (this->data[i] > 0x1f && this->data[i] < 0x80) std::cout << this->data[i];
        else std::cout << "."; 

    }
    std::cout << std::endl;

    // reset editor
    SetConsoleCursorPosition(hConsole, cursor_position);

    // run editor
    int c = 0;
    while(true)
    {
        SetConsoleCursorPosition(hConsole, cursor_position);
        FillConsoleOutputAttribute(hConsole, BACKGROUND_BLUE, 2, cursor_position, &res);
        c = 0;
        c = getch();
        
        // exit wihtout save - esc
        if (c == ESC) {
            system("cls");
            break;
        }
        // edit multiple - enter
        else if (c == ENTER) {
            int r = SAVE;
            while (true) {
                r = this->edit_byte();
                if (r != SAVE) break;
                FillConsoleOutputAttribute(hConsole, 0x0f, 2, cursor_position, &res);
                if (this->cursor_position.X == 7 * 3) {
                    this->cursor_position.Y += 1;
                    this->cursor_position.X = 0;
                }
                else this->cursor_position.X += 3;
            }
        }
        // edit one byte - backspace
        else if (c == BACKSPACE) {
            this->edit_byte();
        }
        // check if lctrl+s is pressed - save data back to file
        else if (GetAsyncKeyState(VK_LCONTROL) && c == 19) {
            system("cls");
            return SAVE;
        }
        // arrow input
        else if (c == 224) {
            FillConsoleOutputAttribute(hConsole, 0x0f, 2, this->cursor_position, &res);
            switch((c=getch())) {
            case KEY_UP:
                if (this->cursor_position.Y > 0)
                    this->cursor_position.Y -= 1;
                break;
            case KEY_DOWN:
                if (this->cursor_position.Y < this->size / 8)
                    this->cursor_position.Y += 1;
                break;
            case KEY_LEFT:
                if (this->cursor_position.X > 0)
                    this->cursor_position.X -= 3;
                break;
            case KEY_RIGHT:
                if (this->cursor_position.X < 7 * 3)
                    this->cursor_position.X += 3;
                break;
            default:
                std::cout << std::endl << "null" << std::endl;  // not arrow
                break;
            }
        }
    }

    system("cls");
    return 0;
}


int Editor::edit_byte() {
    DWORD res;
    // clear place and fill with red
    FillConsoleOutputCharacter(this->hConsole, ' ', 2, this->cursor_position, &res );
    FillConsoleOutputAttribute(this->hConsole, BACKGROUND_RED | 0xf, 2, this->cursor_position, &res);

    // save current value to recover if break
    std::string reserv = intToHex(this->data[(cursor_position.Y * 8) + (cursor_position.X / 3)]);
    // get new value
    int count = 0;
    std::string hexval = "0x";
    int c;
    // get new hex value and print it
    while (true) {
        // input only 2 characters
        if (count >= 2) break;
        c = getch();
        // check that character is a hex digit
        if (c == ESC) {
            this->cursor_position.X -= count;
            WriteConsoleOutputCharacter(hConsole, reserv.c_str(), 2, cursor_position, &res );
            return 0;
        }
        if ((c < '0' || c > '9') && (c < 'a' || c > 'f'))
            continue;
        FillConsoleOutputCharacter(hConsole, c, 1, cursor_position, &res );
        count++;
        cursor_position.X++;
        hexval += (char)c;
    }
    cursor_position.X-= 2; // get cursor position back
    // set new val in data
    this->data[(cursor_position.Y * 8) + (cursor_position.X / 3)] = std::stoi(hexval, nullptr, 16);
    return SAVE;
}

/**
 * Converst and integer to hex representation
 * @param num: number to convert
 * @return: a string containing the hex representation of num
 */
std::string intToHex(int num) {
    std::string hex_string = "";
    bool odd = false;
    while (num > 0) {
        int hex_char = num & 0xf; // get val of least significant 4 bytes
        // std::cout << hex_char << std::endl;
        if (hex_char <= 9) hex_string += (char)(hex_char + '0');
        else hex_string += (char)(hex_char += 'a') - 10;
        num >>= 4;
        odd = !odd;
    }
    if (odd) hex_string += '0';
    if (hex_string.length() == 0) hex_string = "00";
    std::reverse(hex_string.begin(),hex_string.end());

    return hex_string;
}