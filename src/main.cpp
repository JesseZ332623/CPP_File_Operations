#include "./include/dataClass.h"

/**
 * @brief 检查文件的路径是否正确，主要检查两点：
 * 
 * @brief     1. 文件后缀是否为 .txt
 * @brief     2. 在后缀正确的情况下，能否成功打开这个文件
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

/*显示 版本号，详情和帮助*/
inline void showVersion();
inline void showInfo();
inline void showHelp();

/*根据用户在终端输入的参数，输出不同的信息*/
void loadOptions(const char * __optionString);

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

void showMenu()
{
    std::cout << "Enter your choice: \n";

    std::for_each(menuTable.begin(), menuTable.end(), [](const char * __str) { std::cout << __str << "   "; });

    std::cout << '\n' << "Your Choose: ";
}

inline void showVersion() { std::printf("\n\nfileOperations version: %s\n", SOFTWARE_VERSION); printSplitLine(35, '-'); }
inline void showInfo() 
{   
    showVersion();
    std::puts("A C++ file operation question \nfrom the software engineering major of Xiamen University.");
    std::puts("Author: JesseZ332623");
    printSplitLine(35, '-');
}
inline void showHelp()
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

int main(int argc, char const *argv[])
{
    std::system("cls");

    loadOptions(argv[1]);

    PositiveConfidenceLimitsTable newTable;
    std::string filePath;
    char userChoose = 0;
    int64_t runTime = 0;

    showInfo();
    std::cout << myLog.Notify << "Enter The PATH of Data File: (Press q to qute):\n" << myLog.Original;
    std::getline(std::cin, filePath);

    checkPathFormat(filePath);
    std::system("cls");

    newTable.readFile(filePath);
    std::cout << newTable << '\n';

    showMenu();
    std::cin >> userChoose;

    while (userChoose != 'q')
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

            case DELETE:
                system("cls");
                runTime = TIMER(newTable.deleteFileLine(filePath));
                std::cout << "Execution time: " << runTime << " ms." << "\n\n";
                delayMilliseconds(1600);
                system("cls");
                newTable.readFile(filePath);
                std::cout << newTable << '\n';
                break;

            case SEARCH:
                system("cls");
                runTime = TIMER(newTable.search());
                std::cout << "Execution time: " << runTime << " ms." << "\n\n";
                delayMilliseconds(1000);
                std::cout << myLog.Notify << "File PATH: " << filePath << '\n' << myLog.Original;
                std::cout << newTable << '\n';
                break;

            default:
                std::cerr << myLog.Warning << "Invalid choice!" << '\n' << myLog.Original;
                delayMilliseconds(500);
                system("cls");
                std::cout << myLog.Notify << "File PATH: " << filePath << '\n' << myLog.Original;
                std::cout << newTable << '\n';
                break;
        }

        showMenu();
        std::cin >> userChoose;
    }

    std::cout << myLog.Correct << "Done.\n" << myLog.Original;
    
    return EXIT_SUCCESS;
}