#include "./include/dataClass.h"

void delayMilliseconds(unsigned long int __millisSeconds)
{
    clock_t start = clock();
    while (clock() < start + __millisSeconds); 
}

inline void printSplitLine(int __len, const char __style)
{
    for (int len = 0; len < __len; ++len)
    {
        std::putchar(__style);
    }
    std::putchar('\n');
}

inline bool PositiveConfidenceLimitsTable::isNumber(const std::string & __str) const
{
    /*for_each 读取该字符串，如果发现字符串中的某一个字符不是数字字符 '0' ~ '9'，返回 false*/
    for (const char & ch : __str)
    {
        if (!std::isdigit(ch)) { return false; }
    }

    /*否则返回 true*/
    return true;
}

inline void PositiveConfidenceLimitsTable::sortPclTable()
{
    auto compareRule = [](PositiveConfidenceLimits & __a, PositiveConfidenceLimits & __b) { return EXTRACT_COP_TO_NUMBER(__a.COP) <  EXTRACT_COP_TO_NUMBER(__b.COP); };
    std::sort(pclTable.begin(), pclTable.end(), compareRule);
}

int PositiveConfidenceLimitsTable::binarySearch(const std::string & __target) const
{
#if BINARY_SEARCH
    int left = 0;
    int right = pclTable.size() - 1;
    
    while(left <= right) 
    {
        int mid = left + (right - left) / 2;
        
        int targetNum = EXTRACT_COP_TO_NUMBER(__target); 
        int midNum = EXTRACT_COP_TO_NUMBER(pclTable[mid].COP);
        
        if(targetNum == midNum) { return mid; }
        else if(targetNum < midNum) { right = mid - 1; } 
        else { left = mid + 1; }
    }

    return -1;

#elif SEQUENTIAL_BRUTE_FORCE_SEARCH
    std::size_t targetNumber = EXTRACT_COP_TO_NUMBER(__target);
    int tarIndex = 0;

    for (auto & __cop : pclTable)
    {
        if (EXTRACT_COP_TO_NUMBER(__cop.COP) != targetNumber) { ++tarIndex; }
        else { return tarIndex; }
    }
    
    return -1;
#else
#endif
}

inline void PositiveConfidenceLimitsTable::splitString(std::string & __fileLineString, std::vector<std::string> & __subStrings) const
{
    /*
        创建一个字符串流，传入 __fileLineString 构建。
    */
    std::stringstream strStream(__fileLineString);
    std::string tempString;

    /*
        以字符串 __fileLineString 为界，不断的分割字符串，
        然后将子串存入 __subStrings 动态数组中去，直到分无可分。
    */
    while (std::getline(strStream, tempString, ' '))
    {
        __subStrings.push_back(tempString);
    }
}

inline void PositiveConfidenceLimitsTable::showOneLine(std::ostream & __os, PositiveConfidenceLimits & __tempPcl) const
{
    __os << __tempPcl.COP << "\t\t\t\t" 
              << __tempPcl.mpnIndex << "\t\t\t" << __tempPcl.lower << '\t' << __tempPcl.upper << '\n';
    delayMilliseconds(65);
}

bool PositiveConfidenceLimitsTable::readFile(const std::string __filePath)
{
    /*
        以只读模式 std::ios_base::in 打开 __filePath 文件。
    */
    std::ifstream readStream(__filePath, std::ios_base::in);

    /*如果出现文件不存在，或者被其他进程占用等其他情况，则返回 false*/
    if(!readStream.is_open()) { std::cerr << "Open File: " << __filePath << " Failed.\n"; return false; }

    pclTable.clear();

    std::cout << "Load File: " << __filePath << '\n';

    /*由于读取文件某一行的字符串*/
    std::string fileLineStr;

    /*由 splitString 分割后的字串们存储在此处*/
    std::vector<std::string> subStrings;

    /*一个临时的 PositiveConfidenceLimits 结构体，用于暂存文件中一行字符串所对应的数据（经过转换）*/
    PositiveConfidenceLimits tempTableLine;

    /*
        逐行读取文件内容，每读取一行，就把这一行的内容存入 fileLineStr，直到遇见 EOF 停止。
    */
    while (std::getline(readStream, fileLineStr))
    {
        /*按空格分割字符串并存入 subStrings 中*/
        splitString(fileLineStr, subStrings);

        /*将分割后的字串进行相应的转换后交由构建函数构建，而后存入 tempTableLine 临时结构体*/
        tempTableLine = PositiveConfidenceLimits(subStrings[0], std::stoi(subStrings[1]), std::stoi(subStrings[2]), std::stoi(subStrings[3]));
        
        /*最后在末尾插入这个结构体*/
        pclTable.push_back(tempTableLine);

        /*每读取完一行，就需要对 subStrings 进行一次清理，以准备下一次读取。*/
        subStrings.clear();
    }

    /*排序*/
    sortPclTable();

    /*读取完毕后输出报告行数*/
    std::cout << "Load Complete. file line: " << pclTable.size() << " \n";

    readStream.close(); // 关闭文件

    return true;
}

bool PositiveConfidenceLimitsTable::insertFile(const std::string __filePath)
{
    std::ofstream insertStream(__filePath, std::ios_base::app);

    /*如果出现文件不存在，或者被其他进程占用等其他情况，则返回 false*/
    if(!insertStream.is_open()) { std::cerr << "Open File: " << __filePath << " Failed.\n"; return false; }

    std::string tempFileLineStr, fileLineString; 

    std::cout << "Inset data to File: " << __filePath << " (Press q to back): \n";

    /*
        使用循环去读取 tableKey 中的内容，
        这样可以应对数据量大的情况，在表中有 100 甚至 1000 个键的时候不至于歇菜。
    */
    for (const std::string & key : tableKey)
    {
        std::cout << "Please enter: " << key << " (Press q to back): \n";
        std::getline(std::cin, tempFileLineStr);

        IF_QUIT(tempFileLineStr);

        /*
            处理输入非 Combination of Positives 键的情况，
            这个数据只需要过一遍 isNumber 函数的检查即可。
        */
        if (key != tableKey[0])
        {
            while (!isNumber(tempFileLineStr))
            {
                std::cerr << "Invalid Format!\n" << '\n';
                std::cout << "Enter " << key << " again (Press q to back): \n";
                std::getline(std::cin, tempFileLineStr);
                IF_QUIT(tempFileLineStr);
            }
            fileLineString += " ";
            fileLineString += tempFileLineStr;
        }
        /*
            处理 Combination of Positives 键的情况，
            这个键需要做格式检查，看看是不是 #-#-# 的格式。
            此外还要通过二分法搜索整个结构体，看看有没有重复输入。
        */
        else
        {
            while (CHECK_COP_FORMAT(tempFileLineStr))
            {
                std::cerr << "Invalid Format!\n" << '\n';
                std::cout << "Enter " << key << ": \n";
                std::getline(std::cin, tempFileLineStr);
                IF_QUIT(tempFileLineStr);
            }

            while (binarySearch(tempFileLineStr) != -1)
            {
                std::cerr << "WARNING: Insert duplicate Combination of Positions! [" << tempFileLineStr << "]" << '\n';
                std::cout << "Enter " << key << ": \n";
                std::getline(std::cin, tempFileLineStr);
                IF_QUIT(tempFileLineStr);

                /*检查用户输入的格式是否为 #-#-# 如果不是就得重新输入，避免写入垃圾数据*/
                while (CHECK_COP_FORMAT(tempFileLineStr))
                {
                    std::cerr << "Invalid Format!\n" << '\n';
                    std::cout << "Enter " << key << ": \n";
                    std::getline(std::cin, tempFileLineStr);
                    IF_QUIT(tempFileLineStr);
                }
            }

            fileLineString += tempFileLineStr;
        }
    }

    insertStream << '\n' << fileLineString;

    std::cout << "OK!\tdata:[" << fileLineString << "]\tInsert Complete.\n";
    printSplitLine(70, '-');

    insertStream.close();

    return true;
}

bool PositiveConfidenceLimitsTable::search(void)
{
    std::string target;

    std::cout << "Enter Combination of Positive (Format: #-#-#): \n";
    std::getline(std::cin, target);
    IF_QUIT(target);

    while (CHECK_COP_FORMAT(target))
    {
        std::clog << "Invalid Format!\n" << std::endl;
        std::cout << "Enter Combination of Positives (Format: #-#-#): \n";
        std::getline(std::cin, target);
        IF_QUIT(target);
    }

    int targetIndex = binarySearch(target);

    if (targetIndex == -1) 
    { 
        std::clog << "Not Found Combination of Positive: [" << target << ']' << '\n';
        return false; 
    }
    else
    {
        std::cout << "OK!\t Find the target data: \n";
        printSplitLine(70, '-');
        std::cout << "Combination of Positives" << '\t' << "MPN index(per 100 ml)" << '\t' << "Lower" << '\t' << "Upper" << '\n';
        printSplitLine(70, '-');
        showOneLine(std::cout, pclTable.at(targetIndex));
        printSplitLine(70, '-');
    }
    return true;
}

std::ostream & operator<<(std::ostream & __os, PositiveConfidenceLimitsTable & __pclTable) 
{
    printSplitLine(70, '-');
    __os << "Combination of Positives" << '\t' << "MPN index(per 100 ml)" << '\t' << "Lower" << '\t' << "Upper" << '\n';
    printSplitLine(70, '-');

    for (PositiveConfidenceLimits & __temp : __pclTable.pclTable)
    {
        __pclTable.showOneLine(__os, __temp);
    }

    printSplitLine(70, '-');

    return __os;
}