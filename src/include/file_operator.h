#ifndef _FILE_OPERATOR_
#define _FILE_OPERATOR_

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cctype>
#include <algorithm>
#include <chrono>
//#include <windows.h>

/*
    使用 macro 封装判断表达式：
    EXTRACT         将字符串 COP 从 #-#-# 的格式转换为一个 3 位整数用于比较
    CHECK_FORMAT    检查用户输入的字符串是否为 #-#-# 格式的
    IF_QUIT         检查用户是否输入了 q，以退出插入模式 或 查询模式
    TIMER          计算一个函数的执行时间
*/
#define EXTRACT(COP) (((COP[0] - '0') * 100) + ((COP[2] - '0') * 10) + (COP[4] - '0'))
#define CHECK_FORMAT(temp_str) !(std::isdigit(temp_str[0]) && std::isdigit(temp_str[2]) && std::isdigit(temp_str[4]) && temp_str[1] == '-' && temp_str[3] == '-' && temp_str.size() == 5)
#define IF_QUIT(temp_str) if(temp_str == "q") { std::clog << "Back To The Menu." << std::endl; return false; }

#define TIMER(Run) [&](){\
    auto t1 = std::chrono::system_clock::now();\
    auto res = Run;\
    auto t2 = std::chrono::system_clock::now();\
    auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();\
	std::cout << "execution time: " << dt << " ms." << std::endl;\
	return res;\
}()

/*
    最大可写入的数据量，当前不考虑动态增长的问题，
    不然就直接把结构体里面的成员全部换成 std::vector 类型了
*/
const int MAX_DATA_COUNT = 500;

/*
    表中的数据在结构体中使用类型如下：
    1. Combination Of Positives 可以使用 std::string 类型

    2. MPN_index                使用 int 类型

    3. 95% Confidence Limits 
        lower   使用 int 类型
        upper   使用 int 类型
*/
typedef struct _Data
{
    std::string COP;
    int MPN_index;

    int lower;
    int upper;

} Data;

/*创建 MAX_DATA_COUNT 个 Data 实例*/
static Data _data[MAX_DATA_COUNT];

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

/*
    结构体数组示例 _data 的排序规则：
    根据 Combination of Positives 的值进行升序排序
*/
const static auto SORT_RULES = [](Data & _a, Data & _b) -> bool { return EXTRACT(_a.COP) < EXTRACT(_b.COP); };

/**
 * @brief 逐个检查字符串中的字符是否为数字。
 * 
 * @param __str 传入的字符串
 * 
 * @return 一个布尔类型，代表这个字符串是不是数字。如 "123"
*/
inline bool is_number(const std::string & __str);

/**
 * 自制的延时函数，来代替 Windows 库中的 Sleep 函数。
 * 
 * @param _millis_seconds 要延迟的时间，精确到毫秒
 * 
 * @return non-return
*/
void delay_milliseconds(unsigned long int _millis_seconds);

/**
 * 提取 COP 字符串中的数字，比如：
 * 将 4-2-0 提取成 420
 * 
 * @param _cop 字符串 Combination of Positives，格式为（#-#-#）
 * 
 * @return 将字符串转换成数字后的值
*/
inline int extract_number(std::string & _cop);

/*交换两个结构体*/
void struct_swap(Data & _a, Data & _b);

/**
 * 将结构体 Data 内的所有数据按照 Combination of Positives 的值升序排列，
 * 
 * @param _data 指向结构体数组实例的指针
 * @param _line_count 当前文件的行数
*/
inline void sort_data_struct(Data * _data, const int _line_count);

/**
 * 二分法搜索，针对数据量较大的情况，能有效的提高效率。
 * 
 * @param _data     指向结构体数组实例的指针
 * @param _target   要在结构体数组内搜索的目标字符串
 * @param _line_count 当前文件的行数
 * 
 * @return 搜索结果，若存在，返回对应的下标。不存在则返回 -1
*/
int binary_search(Data * _data, std::string & _target, const int _line_count);

/**
 * 从文本文件中读入数据，并存入 Data 结构体中
 * 
 * @param _data 指向 Data 类型结构体的指针
 * @param _file_path 要读取文件的路径
 * 
 * @return 返回该文件的行数，目前只需要这个数据。
 *         以后可以设计并返回一个 `File_Attribute` 结构体，记录更详细的文件属性。
*/
int input(Data * _data, const std::string _file_path);

/**
 * 用户输入相关数据，并存入文件末尾
 * 
 * @param _file_path 文件路径
*/
bool insert(const std::string _file_path, int _line_count, Data * temp_datapointer);

/**
 * @brief 查询，用户输入 Combination of Positives 的值，查询对应的数据。
 * 数据量少（100 条以内）就直接暴力搜索，如果数据量量较多多（超过 1000 条）的话，考虑使用二分法搜索，
 * 如果还要多就考虑红黑树等更高级的数据结构。
 * 
 * @param _line_count 数据量
 * @param _data       指向结构体数组实例的指针
 * 
 * @return 返回一个布尔类型，表示是否搜索到
*/
bool search(Data * _data, const int _line_count);

/**
 * 读取完毕后，输出结构体的内容
 * 
 * @param _data 指向 Data 类型结构体的指针
 * @param _line_count 文件的行数，文件有几行，就需要循环几次输出。
 * 
 * @return non-return
*/
void show_contence(Data * _data, const int _line_count);

/**
 * 这个函数可以打印不同风格的分割线
 * @param length 分割线的长度
 * @param style  分割线的风格，如（------- ****** 或 ·········）
*/
void print_split_line(int length, char style);

#endif //_FILE_OPERATOR_