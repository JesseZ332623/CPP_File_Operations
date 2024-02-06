#include "./include/optionsAndMenu.h"
#include "./include/defs.h"

void showMenu()
{
    std::cout << "Enter your choice: \n";

    std::for_each(menuTable.begin(), menuTable.end(), [](const char * __str) { std::cout << __str << "\n"; });

    std::cout << '\n' << "Your Choose: ";
}

void showVersion() { std::printf("\n\nfileOperations version: %s\n", SOFTWARE_VERSION); printSplitLine(35, '-'); }
void showInfo() 
{   
    showVersion();
    std::puts("A C++ file operation question \nfrom the software engineering major of Xiamen University.");
    std::puts("Author: JesseZ332623");
    printSplitLine(35, '-');
}

void showHelp()
{
    showVersion();
    printSplitLine(35, '-');
    std::puts("-v\t Show software version.");
    std::puts("-h\t Show help.");
    std::puts("-info\t Display Descriptions.");
    printSplitLine(35, '-');
}

void loadOptions(const char * __optionString)
{
    if (__optionString == nullptr) { return; }

    else if (std::strcmp(__optionString, "-v") == 0) 
    { 
        showVersion();
        exit(EXIT_SUCCESS);
    }
    else if (std::strcmp(__optionString, "-info") == 0) 
    {
        showInfo();
        exit(EXIT_SUCCESS);
    }
    else if (std::strcmp(__optionString, "-h") == 0)
    {
        showHelp();
        exit(EXIT_SUCCESS);
    }
    else
    {
        std::cout << myLog.Error << "Invalid Options!" << myLog.Original;
        showHelp();
        exit(EXIT_SUCCESS);
    }
}