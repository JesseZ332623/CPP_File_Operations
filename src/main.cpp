#include "./include/dataClass.h"

int main(int argc, char const *argv[])
{
    PositiveConfidenceLimitsTable newTable;

    newTable.readFile("./data/dataFile.txt");
    
    std::cout << newTable << '\n';

    newTable.insertFile("./data/dataFile.txt");
    newTable.readFile("./data/dataFile.txt");

    std::cout << newTable << '\n';
    return EXIT_SUCCESS;
}
