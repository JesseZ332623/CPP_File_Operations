#include "./include/dataClass.h"

/**
 * @brief 检查文件的路径是否正确，主要检查两点：
 * 
 *      1. 文件后缀是否为 .txt
 *      2. 在后缀正确的情况下，能否成功打开这个文件
 * 
 * @param __filePath 文件路径
 * 
 * @return  non-return
*/
void checkPathFormat(std::string & __filePath);

/**
 * @brief 在终端输出菜单
 * 
 * @return  non-return
*/
void showMenu();

void checkPathFormat(std::string & __filePath)
{
    std::fstream openFile(__filePath);

    while ((__filePath.length() >= 3) && (__filePath.substr(__filePath.length() - 3, 3) != "txt"))
    {
        if (__filePath == "q") { std::cout << "Done.\n"; std::exit(EXIT_SUCCESS); }

        std::system("cls");

        std::cerr << "Invalid PATH! (File Format Error, Only Support .txt File).\n";
        std::getline(std::cin, __filePath);
    }

    while (!openFile.is_open())
    {
        if (__filePath == "q") { std::cout << "Done.\n"; std::exit(EXIT_SUCCESS); }

        system("cls");
        std::cerr << "Invalid PATH! (Can Not Open File).\n";
        std::cout << "Enter The PATH of Data File: (Press q to qute):\n";
        std::getline(std::cin, __filePath);

        while ((__filePath.length() >= 3) && (__filePath.substr(__filePath.length() - 3, 3) != "txt"))
        {
            if (__filePath == "q") { std::cout << "Done.\n"; std::exit(EXIT_SUCCESS); }

            std::system("cls");

            std::cerr << "Invalid PATH! (File Format Error, Only Support .txt File).\n";
            std::getline(std::cin, __filePath);
        }
    }
}

void showMenu()
{
    std::cout << "Enter your choice: \n";
    std::cout << "1. Insert data   2. Load and display data  3. Search data  0. Exit system\n";
    std::cout << "Your Choose: ";
}

int main(int argc, char const *argv[])
{
    std::system("cls");

    PositiveConfidenceLimitsTable newTable;
    std::string filePath;
    char userChoose = 0;
    int64_t runTime = 0;

    std::cout << "Enter The PATH of Data File: (Press q to qute):\n";
    std::getline(std::cin, filePath);

    checkPathFormat(filePath);

    newTable.readFile(filePath);
    std::cout << newTable << '\n';

    showMenu();
    std::cin >> userChoose;

    while (userChoose != '0')
    {
        while (std::getchar() != '\n') { continue; }
        switch (userChoose)
        {
            case INSERT:
                system("cls");
                newTable.insertFile(filePath);
                delayMilliseconds(1600);
                system("cls");
                newTable.readFile(filePath);
                std::cout << newTable << '\n';
                break;
            case LOAD:
                system("cls");
                newTable.readFile(filePath);
                std::cout << newTable << '\n';
                break;

            case SEARCH:
                system("cls");

                runTime = TIMER(newTable.search());
                std::cout << "Execution time: " << runTime << " ms." << "\n\n";
                break;

            default:
                std::cout << "invalid choice." << std::endl;
                break;
        }

        showMenu();
        std::cin >> userChoose;
    }

    std::cout << "Done.\n";
    
    return EXIT_SUCCESS;
}
