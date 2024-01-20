#include "./include/dataClass.h"

void PositiveConfidenceLimitsTable::delayMilliseconds(unsigned long int __millisSeconds)
{
    clock_t start = clock();
    while (clock() < start + __millisSeconds); 
}

inline bool PositiveConfidenceLimitsTable::isNumber(const std::string & __str)
{
    for (const char & ch : __str)
    {
        if (!std::isdigit(ch)) { return false; }
    }

    return true;
}

inline void PositiveConfidenceLimitsTable::printSplitLine(int __len, char __style)
{
    for (int len = 0; len < __len; ++len)
    {
        std::putchar(__style);
    }
    std::putchar('\n');
}

inline void PositiveConfidenceLimitsTable::sortPclTable(std::vector<PositiveConfidenceLimits> & __pclTable)
{
    std::sort(__pclTable.begin(), __pclTable.end(), compareRule);
}

int PositiveConfidenceLimitsTable::binarySearch(std::vector<PositiveConfidenceLimits> & __pclTable, std::string & __target)
{
    std::size_t begin = 0;
    std::size_t end = __pclTable.size();

    while (begin <= end)
    {
        int midIndex = begin + (end - begin) / 2;
        if (EXTRACT_COP_TO_NUMBER(__pclTable[midIndex].COP) == EXTRACT_COP_TO_NUMBER(__target)) { return midIndex; }
        else if (EXTRACT_COP_TO_NUMBER(__pclTable[midIndex].COP) < EXTRACT_COP_TO_NUMBER(__target)) { begin = midIndex + 1; }
        else if (EXTRACT_COP_TO_NUMBER(__pclTable[midIndex].COP) > EXTRACT_COP_TO_NUMBER(__target)) { end = midIndex - 1; }
    }

    return -1;
}