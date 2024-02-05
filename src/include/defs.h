#ifndef __DEFS_H_
#define __DEFS_H_

#include <fstream>
#include <vector>
#include <cctype>
#include <algorithm>
#include <chrono>
#include <sstream>
#include <cstring>
#include "./terminalColor.h"

/*软件版本*/
#define SOFTWARE_VERSION "ver 0.6.5"

/*
    BINARY_SEARCH                       选择二分法搜索
    SEQUENTIAL_BRUTE_FORCE_SEARCH       选择顺序暴力搜索
*/
#define BINARY_SEARCH true
#define SEQUENTIAL_BRUTE_FORCE_SEARCH false

#define COP_STRING_LENGTH 5             // Combination Of Positives 字符串的长度

#define READ_ONLY_MODE  std::ios_base::in                       // 只读打开文件标志位
#define READ_WRITE_MODE (std::ios_base::in | std::ios_base::out)  // 可读可写模式
#define END_INSERT_MODE std::ios_base::app                      // 末尾插入模式打开标志位
#define CLEAR_MODE      std::ios_base::trunc                    // 清理模式打开文件标志位 
   
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
#define IF_QUIT(temp_str) if(temp_str == "q") { std::clog << myLog.Correct << "Back To The Menu." << '\n' << myLog.Original; dataFileStream.close(); return false; }

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

        PositiveConfidenceLimits() = default;

        PositiveConfidenceLimits(std::string & __COP, int __mpnIndex, int __low, int __up) 
        : COP(__COP), mpnIndex(__mpnIndex), lower(__low), upper(__up) {}
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

/**
    * @brief                    自制的延时函数，来代替 Windows 库中的 Sleep 函数。
    * 
    * @param __millisSeconds    要延迟的时间，精确到毫秒
    * 
    * @return                   non-return
*/
void delayMilliseconds(unsigned long int __millisSeconds);

 /**
    * @brief            这个函数可以打印不同风格的分割线
    * 
    * @param __len      分割线的长度
    * @param __style    分割线的风格，如（------- ****** 或 ·········）
    * 
    * @return           non-return
*/
void printSplitLine(int __len, const char __style);

#endif // __DEFS_H_