#include "./include/fileFormatCheck.h"

void checkPathFormat(std::string & __filePath)
{
    while ((__filePath.length() >= 3) && (__filePath.substr(__filePath.length() - 3, 3) != "txt"))
    {
        if (__filePath == "q") { std::cout << myLog.Correct << "Done.\n" << myLog.Original; std::exit(EXIT_SUCCESS); }

        std::system("cls");

        std::cerr << myLog.Warning << "Invalid PATH! (File Format Error, Only Support .txt File).\n" << myLog.Original;
        std::cout << "Enter The PATH of Data File: (Press q to qute):\n";
        std::getline(std::cin, __filePath);
    }

    std::fstream openFile(__filePath);
    while (!openFile.is_open())
    {
        if (__filePath == "q") { std::cout << myLog.Correct << "Done.\n" << myLog.Original; std::exit(EXIT_SUCCESS); }

        system("cls");
        std::cerr << myLog.Warning << "Invalid PATH! (File Format Error, Only Support .txt File).\n" << myLog.Original;
        std::cout << "Enter The PATH of Data File: (Press q to qute):\n";
        std::getline(std::cin, __filePath);

        while ((__filePath.length() >= 3) && (__filePath.substr(__filePath.length() - 3, 3) != "txt"))
        {
            if (__filePath == "q") { std::cout << myLog.Correct << "Done.\n" << myLog.Original; std::exit(EXIT_SUCCESS); }

            std::system("cls");

            std::cerr << myLog.Warning << "Invalid PATH! (File Format Error, Only Support .txt File).\n" << myLog.Original;
            std::getline(std::cin, __filePath);
        }
        openFile.open(__filePath);
    }
}