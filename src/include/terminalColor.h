#ifndef _TERMINAL_COLOR_H_
#define _TERMINAL_COLOR_H_

#include <iostream>

enum ColorMenu
{
    RED = 1,                // EROOR
    GREEN,                  // CORRECT
    YELLO,                  // WARNING
    BLUE = 6,               // NOTIFY
    WHITE,                  // ORIGINAL
};

class TerminalTextColor
{
    private:
        int colorCode;

    public:
        TerminalTextColor() : colorCode(0) {}
        TerminalTextColor(const int __colorSign) : colorCode(__colorSign) {}

        friend std::ostream & operator<<(std::ostream & __os, const TerminalTextColor & __colorSign);

        ~TerminalTextColor() {}
};

#endif // _TERMINAL_COLOR_H_