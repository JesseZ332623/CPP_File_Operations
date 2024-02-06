#include "./include/dataClass.h"
#include "./include/optionsAndMenu.h"

void delayMilliseconds(unsigned long int __millisSeconds)
{
    clock_t start = clock();
    while (clock() < start + __millisSeconds) {}
}

void printSplitLine(int __len, const char __style)
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

void PositiveConfidenceLimitsTable::splitString(std::string & __fileLineString, std::vector<std::string> & __subStrings) const
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

inline bool PositiveConfidenceLimitsTable::clearFileContent(const std::string & __filePath) const
{
    /*用 trunc 模式打开文件，回直接清空文件内容*/
    std::ofstream clearStream(__filePath, CLEAR_MODE);

    /*如果出现文件不存在，或者被其他进程占用等其他情况，则返回 false*/
    if (!clearStream.is_open()) { std::cerr << myLog.Error << "Can't open file: " << __filePath << '\n' << myLog.Original; return false; }

    clearStream.close();

    return true;
}

std::size_t PositiveConfidenceLimitsTable::searchCOPIndex(std::vector<std::string> & __fileLineArray, const std::string & __targetCOPString) const
{
    std::size_t lineArraySize = __fileLineArray.size();

    for (int index = 0; index < lineArraySize; ++index)
    {
        if (__fileLineArray[index].substr(0, COP_STRING_LENGTH) == __targetCOPString) { return (std::size_t)index; }
    }

    return -1;
}

bool PositiveConfidenceLimitsTable::readFile()
{
    /*以只读模式打开文件*/
    dataFileStream.open(filePath, READ_ONLY_MODE);

    /*如果出现文件不存在，或者被其他进程占用等其他情况，则返回 false*/
    if(!dataFileStream.is_open()) { std::cerr << myLog.Error << "Open File: " << filePath << " Failed.\n" << myLog.Original ; return false; }

    pclTable.clear();

    std::cout << myLog.Notify << "Load File: " << filePath << myLog.Original << '\n';

    /*由于读取文件某一行的字符串*/
    std::string fileLineStr;

    /*由 splitString 分割后的字串们存储在此处*/
    std::vector<std::string> subStrings;

    /*
        逐行读取文件内容，每读取一行，就把这一行的内容存入 fileLineStr，直到遇见 EOF 停止。
    */
    while (std::getline(dataFileStream, fileLineStr))
    {
        /*按空格分割字符串并存入 subStrings 动态数组中*/
        splitString(fileLineStr, subStrings);
        
        /*最后在末尾构建并插入这个结构体*/
        pclTable.push_back(PositiveConfidenceLimits(subStrings[0], std::stoi(subStrings[1]), std::stoi(subStrings[2]), std::stoi(subStrings[3])));

        /*每读取完一行，就需要对 subStrings 进行一次清理，以准备下一次读取。*/
        subStrings.clear();
    }

    /*排序*/
    sortPclTable();

    /*读取完毕后输出报告行数*/
    std::cout << myLog.Correct << "Load Complete. " << myLog.Original << " \n";

    dataFileStream.close(); // 关闭文件

    return true;
}

bool PositiveConfidenceLimitsTable::insertFile()
{
    /*以末尾插入模式打开文件*/
    dataFileStream.open(filePath, END_INSERT_MODE);

    /*如果出现文件不存在，或者被其他进程占用等其他情况，则返回 false*/
    if(!dataFileStream.is_open()) { std::cerr << myLog.Error << "Open File: " << filePath << " Failed.\n" << myLog.Original; return false; }

    std::string tempFileLineStr, fileLineString; 

    std::cout << myLog.Notify << "Inset data to File: " << filePath << " (Press q to back): \n" << myLog.Original;

    /*
        使用循环去读取 tableKey 中的内容，
        这样可以应对数据量大的情况，在表中有 100 甚至 1000 个键的时候不至于歇菜。
    */
    for (const char * key : tableKey)
    {
        std::cout << "Please enter: " << key << " (Press q to back): \n";
        std::getline(std::cin, tempFileLineStr);

        IF_QUIT(tempFileLineStr);

        /*
            处理输入非 Combination of Positives 键的情况，
            这个数据只需要过一遍 isNumber() 函数的检查 和 非空检查 即可。
        */
        if (key != tableKey[0])
        {
            while (!isNumber(tempFileLineStr) || tempFileLineStr.empty())
            {
                std::cerr << myLog.Warning << "Invalid Format!\n" << '\n' << myLog.Original;
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
                std::cerr << myLog.Warning << "Invalid Format!\n" << '\n' << myLog.Original;
                std::cout << "Enter " << key << ": \n";
                std::getline(std::cin, tempFileLineStr);
                IF_QUIT(tempFileLineStr);
            }

            while (binarySearch(tempFileLineStr) != -1)
            {
                std::cerr << myLog.Warning << "WARNING: Insert duplicate Combination of Positions! [" << tempFileLineStr << "]" << '\n' << myLog.Original;
                std::cout << "Enter " << key << ": \n";
                std::getline(std::cin, tempFileLineStr);
                IF_QUIT(tempFileLineStr);

                /*检查用户输入的格式是否为 #-#-# 如果不是就得重新输入，避免写入垃圾数据*/
                while (CHECK_COP_FORMAT(tempFileLineStr))
                {
                    std::cerr << myLog.Warning << "Invalid Format!\n" << '\n' << myLog.Original;
                    std::cout << "Enter " << key << ": \n";
                    std::getline(std::cin, tempFileLineStr);
                    IF_QUIT(tempFileLineStr);
                }
            }

            fileLineString += tempFileLineStr;
        }
    }

    dataFileStream << '\n' << fileLineString;

    std::cout << myLog.Correct << "OK!\tdata: [" << fileLineString << "]\tInsert Complete.\n" << myLog.Original;
    printSplitLine(70, '-');

    dataFileStream.close();

    return true;
}

bool PositiveConfidenceLimitsTable::modifyFileLine()
{
    dataFileStream.open(filePath, READ_WRITE_MODE);

    /*如果出现文件不存在，或者被其他进程占用等其他情况，则返回 false*/
    if(!dataFileStream.is_open()) 
    { 
        std::cerr << myLog.Error 
                  << "Open File: " << filePath << " Failed.\n" 
                  << myLog.Original; 
        return false; 
    }

    std::string tempLineString;                 // 用于暂存文件一行数据的字符串
    std::vector<std::string> fileLineArray;     // 存储文件每一行字符串的动态数组
    
    /*从文件中逐行读取字符串并插入动态数组末尾*/
    while (std::getline(dataFileStream, tempLineString))
    {
        fileLineArray.push_back(tempLineString);
    }

    /*用户输入 COP 字符串*/
    std::string tempCOPString;
    std::cout << myLog.Notify << "Modify data to File:" << getFilePath() << myLog.Original << '\n';
    std::cout << "Please enter" << tableKey[0] << "(Press q to back):" << '\n';
    std::getline(std::cin, tempCOPString);
    IF_QUIT(tempCOPString);

    /*检查字符串的合法性*/
    while (CHECK_COP_FORMAT(tempCOPString))
    {
        std::cerr << myLog.Warning << "Invalid Format!\n" << '\n' << myLog.Original;
        std::cout << "Please enter " << tableKey[0] << " (Press q to back):" << '\n';
        std::getline(std::cin, tempCOPString);
        IF_QUIT(tempCOPString);
    }

    /*在动态数组中搜索目标字符串，返回其下标*/
    std::size_t targetIndex = searchCOPIndex(fileLineArray, tempCOPString);
    if (targetIndex == -1)  // 若找不到，就报错并关闭文件后返回
    {
        std::cerr << myLog.Warning << tempCOPString << " Not in File: " << getFilePath() << '\n' << myLog.Original;
        std::cout << myLog.Correct << "Back To The Menu." << '\n' << myLog.Original;
        dataFileStream.close();
        delayMilliseconds(1600);
        return false;
    }

    std::cout << myLog.Correct 
              << "OK Find data [ " << fileLineArray.at(targetIndex) 
              << " ] in file: " << getFilePath() << '\n' 
              << myLog.Original;
    printSplitLine(70, '-');

    /*要新写入文件的字符串，先保存 COP 的值*/
    std::string newFileLineString = fileLineArray.at(targetIndex).substr(0, COP_STRING_LENGTH + 1);

    /*临时存储除 Combination of Positives 以外值的字符串*/
    std::string tempSubString;
    fileLineArray.at(targetIndex).clear();

    for (const char * key : tableKey)
    {
        if (strcmp(key, tableKey.at(0)) == 0) { continue; }

        std::cout << "Please enter: new " << key << " (Press q to back): \n";
        std::getline(std::cin, tempSubString);
        IF_QUIT(tempSubString);

        while (!isNumber(tempSubString) || tempSubString.empty())
        {
            std::cerr << myLog.Warning << "Invalid Format!\n" << '\n' << myLog.Original;
            std::cout << "Enter " << key << " again (Press q to back): \n";
            std::getline(std::cin, tempSubString);
            IF_QUIT(tempSubString);
        }
        tempSubString += " ";
        newFileLineString += tempSubString;
    }

    //std::cout << newFileLineString << '\n';

    fileLineArray.at(targetIndex) = newFileLineString;

    dataFileStream.close();

    clearFileContent(filePath);   // 清空文件所有内容

    /*重新打开文件，并做检查*/
    dataFileStream.open(filePath, END_INSERT_MODE);

    if(!dataFileStream.is_open()) 
    { 
        std::cerr << myLog.Error 
                  << "Open File: " << getFilePath() << " Failed.\n" 
                  << myLog.Original; 
        return false; 
    }

    std::size_t lineArraySize = fileLineArray.size();
    for (int index = 0; index < lineArraySize; ++index)
    {
        dataFileStream << fileLineArray[index];
        if (index != lineArraySize - 1) { dataFileStream << '\n'; }
    }

    std::cout << myLog.Correct << "OK! The new table line is: [ " << newFileLineString << "]\n" << myLog.Original;

    dataFileStream.close();

    std::cout << myLog.Correct << "Back To The Menu." << '\n' << myLog.Original;

    return false;
}

bool PositiveConfidenceLimitsTable::deleteFileLine()
{
    /*以输入输出模式打开目标文件*/
    dataFileStream.open(filePath, READ_WRITE_MODE);

    /*如果出现文件不存在，或者被其他进程占用等其他情况，则返回 false*/
    if (!dataFileStream.is_open()) 
    { 
        std::cerr << myLog.Error << "Open File: " << filePath << " Failed.\n" << myLog.Original; 
        return false;
    }

    std::cout << myLog.Notify << "Delete data to File: " << filePath << " (Press q to back): \n" << myLog.Original;

    std::string targetCOPString;                // 目标 COP 字符串 
    std::string tempLineString;                 // 用于暂存文件一行数据的字符串
    std::vector<std::string> fileLineArray;     // 存储文件每一行字符串的动态数组
    
    /*从文件中逐行读取字符串并插入动态数组末尾*/
    while (std::getline(dataFileStream, tempLineString))
    {
        fileLineArray.push_back(tempLineString);
    }

    /*用户输入目标 COP 字符串，并对输入的字符串进行格式检查*/
    std::cout << "Enter Combination of Positive You Want tO Delete (Format: #-#-#): \n";
    std::getline(std::cin, targetCOPString);
    IF_QUIT(targetCOPString);

    while (CHECK_COP_FORMAT(targetCOPString))
    {
        std::cerr << myLog.Warning << "Invalid Format!\n" << '\n' << myLog.Original;
        std::cout << "Enter Combination of Positive You Want TO Delete (Format: #-#-#): \n";
        std::getline(std::cin, targetCOPString);
        IF_QUIT(targetCOPString);
    }

    /*在动态数组中搜索目标字符串，返回其下标*/
    std::size_t targetIndex = searchCOPIndex(fileLineArray, targetCOPString);
    if (targetIndex == -1)
    {
        std::cerr << myLog.Warning << targetCOPString << " Not in File: " << filePath << '\n' << myLog.Original;
        std::cout << myLog.Correct << "Back To The Menu." << '\n' << myLog.Original;
        dataFileStream.close();
        return false;
    }
    std::string targetLineString = fileLineArray.at(targetIndex);

    /*然后在动态数组中擦除*/
    fileLineArray.erase(fileLineArray.begin() + targetIndex);

    dataFileStream.close();   // 暂时关闭文件

    clearFileContent(filePath);   // 清空文件所有内容

    /*重新打开文件，并做检查*/
    dataFileStream.open(filePath, READ_WRITE_MODE);

    if (!dataFileStream.is_open()) 
    { 
        std::cerr << myLog.Error << "Open File: " << filePath << " Failed.\n" << myLog.Original; 
        return false;
    }

    /*将动态数组中的所有数据写入文件*/
    std::size_t lineArraySize = fileLineArray.size();
    for (int index = 0; index < lineArraySize; ++index)
    {
        dataFileStream << fileLineArray[index];
        if (index != lineArraySize - 1) { dataFileStream << '\n'; }
    }

    dataFileStream.close();   // 关闭文件

    /*报告删除成功*/
    std::cout << myLog.Correct << "OK Delete [" << targetLineString << "] Successfully.\n" << myLog.Original;
    printSplitLine(70, '-');

    return true;
}

bool PositiveConfidenceLimitsTable::search(void)
{
    std::string target;

    std::cout << myLog.Notify << "Inset data form pclTable (Press q to back): \n" << myLog.Original;
    std::cout << "Enter Combination of Positive You Want to Search (Format: #-#-#): \n";
    std::getline(std::cin, target);
    IF_QUIT(target);

    while (CHECK_COP_FORMAT(target))
    {
        std::cerr << myLog.Warning << "Invalid Format!\n" << '\n' << myLog.Original;
        std::cout << "Enter Combination of Positives (Format: #-#-#): \n";
        std::getline(std::cin, target);
        IF_QUIT(target);
    }

    int targetIndex = binarySearch(target);

    if (targetIndex == -1) 
    { 
        std::clog << myLog.Warning << "Not Found Combination of Positive: [" << target << ']' << '\n' << myLog.Original;
        return false; 
    }
    else
    {
        std::cout << myLog.Correct << "OK!\t Find the target data: \n" << myLog.Original;
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
    __os << "Toatal File Line Count: " << __pclTable.pclTable.size() << '\n';
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