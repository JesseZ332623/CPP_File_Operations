#include "./include/terminalColor.h"

std::ostream & operator<<(std::ostream & __os, const TerminalTextColor & __colorSign)
{
    std::string asciiColorSign;

    asciiColorSign = asciiColorSign + "\033[38;5;" + std::to_string(__colorSign.colorCode) + "m";

    __os << asciiColorSign; 

    return __os;
}