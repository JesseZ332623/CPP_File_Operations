#ifndef __DEFS_H_
#define __DEFS_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>
#include <algorithm>
#include <chrono>
#include <sstream>
#include "./terminalColor.h"
   
/*
    EXTRACT_COP_TO_NUMBER           将字符串 COP 从 #-#-# 的格式转换为一个 3 位整数用于比较
*/
#define EXTRACT_COP_TO_NUMBER(COP) (((COP[0] - '0') * 100) + ((COP[2] - '0') * 10) + (COP[4] - '0'))

/*
    CHECK_COP_FORMAT                检查用户输入的字符串是否为 #-#-# 格式的
*/
#define CHECK_COP_FORMAT(temp_str) !(std::isdigit(temp_str[0]) && std::isdigit(temp_str[2]) && std::isdigit(temp_str[4]) && temp_str[1] == '-' && temp_str[3] == '-' && temp_str.size() == 5)

/*
    IF_QUIT                         检查用户是否输入了 q，以退出插入模式 或 查询模式
*/
#define IF_QUIT(temp_str) if(temp_str == "q") { std::clog << myLog.Correct << "Back To The Menu." << '\n' << myLog.Original; return false; }

/*
    TIMER                           计算一个函数的执行时间
*/
#define TIMER(Run) [&]() { \
    auto t1 = std::chrono::system_clock::now(); \
    Run; \
    auto t2 = std::chrono::system_clock::now(); \
    auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count(); \
    return dt; \
}()

/*
    BINARY_SEARCH                       选择二分法搜索
    SEQUENTIAL_BRUTE_FORCE_SEARCH       选择暴力顺序搜索
*/
#define BINARY_SEARCH true
#define SEQUENTIAL_BRUTE_FORCE_SEARCH false

/*
    菜单枚举类型，功能如下：
    INSERT  插入数据到文件末尾
    LOAD    从文件加载数据到结构体，然后再排序结构体，最后输出至屏幕或其他地方
    SEARCH  根据 Combination of Positives 查询对应的数据
*/
enum MENU {QUIT = '0', INSERT, LOAD, SEARCH}; 

/*
    表中的数据在结构体中使用类型如下：
    1. Combination Of Positives 可以使用 std::string 类型

    2. MPN_index                使用 int 类型

    3. 95% Confidence Limits 
    lower   使用 int 类型
    upper   使用 int 类型
*/
struct PositiveConfidenceLimits
{
    public:
        std::string COP;
        int mpnIndex;

        int lower;
        int upper;

        PositiveConfidenceLimits() {}

        PositiveConfidenceLimits(std::string & __COP, int __mpnIndex, int __low, int __up) 
        : COP(__COP), mpnIndex(__mpnIndex), lower(__low), upper(__up) {}
};

/*
    对应表中的 4 个键，
    有了这个字符串数组，就可以用循环来控制输入。
    当然，这里不能用 printf 去读取，% C 有可能会被误认为格式字符。
*/
const std::vector<const char *> tableKey = 
{
    "Combination of Positives (Format: #-#-#)",
    "MPN index (per 100 ml)",
    "95% Confidence Limits of Lower",
    "95% Confidence Limits of Upper"
};

typedef struct LogLevel
{
    TerminalTextColor Original;
    TerminalTextColor Notify;
    TerminalTextColor Warning;
    TerminalTextColor Correct;
    TerminalTextColor Error;

    LogLevel(int O = WHITE, int N = BLUE, int W = YELLO, int C = GREEN, int E = RED) :
    Original(O), Notify(N), Warning(W), Correct(C), Error(E) {}
} MyLog;

/*一个简单的日志库，用于不同类型消息的输出*/
static MyLog myLog;

#endif // __DEFS_H_