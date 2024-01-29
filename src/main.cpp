#include "./include/dataClass.h"
#include "./include/optionsAndMenu.h"
#include "./include/fileFormatCheck.h"

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
            case BACK:
                std::system("cls");
                showInfo();
                std::cout << myLog.Notify << "Enter The PATH of Data File: (Press q to qute):\n" << myLog.Original;
                std::getline(std::cin, filePath);
                checkPathFormat(filePath);
                std::system("cls");
                newTable.readFile(filePath);
                std::cout << newTable << '\n';
                break;

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