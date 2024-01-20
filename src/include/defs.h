#ifndef __DEFS_H_
#define __DEFS_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>
#include <algorithm>
#include <chrono>
   
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
#define IF_QUIT(temp_str) if(temp_str == "q") { std::clog << "Back To The Menu." << std::endl; return false; }

/*
    TIMER                           计算一个函数的执行时间
*/
#define TIMER(Run) [&](){\
    auto t1 = std::chrono::system_clock::now();\
    auto res = Run;\
    auto t2 = std::chrono::system_clock::now();\
    auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();\
	std::cout << "execution time: " << dt << " ms." << std::endl;\
	return res;\
}()

/*
    对应表中的 4 个键，
    有了这个字符串数组，就可以用循环来控制输入。
    当然，这里不能用 printf 去读取，% C 有可能会被误认为格式字符。
*/
const std::vector<const char *> table_key = 
{
    "Combination of Positives (Format: #-#-#)",
    "MPN index (per 100 ml)",
    "95% Confidence Limits of Lower",
    "95% Confidence Limits of Upper"
};

#endif // __DEFS_H_