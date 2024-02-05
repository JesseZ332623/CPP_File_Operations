#ifndef _OPTIONS_AND_MENU_H_
#define _OPTIONS_AND_MENU_H_

#define TABLE_KEY_COUNT 4
#define MENU_COUNT 6

#include <array>

/*
    菜单枚举类型，标志位说明如下：

    BACK    退回到输入文件路径的界面
    INSERT  插入数据到文件末尾
    LOAD    从文件加载数据到结构体，然后再排序结构体，最后输出至屏幕，文件或其他程序
    DELETE  输入 Combination of Positives 在文件检索对应的行并删除该行
    SEARCH  根据 Combination of Positives 查询对应的数据
    QUIT    退出系统
*/
enum MENU { BACK = '0', INSERT, LOAD, DELETE, SEARCH, QUIT = 'q' }; 

/*
    对应表中的 4 个键，
    有了这个字符串数组，就可以用循环来控制输入。
    当然，这里不能用 printf 去读取，% C 有可能会被误认为格式字符。
*/
const std::array<const char *, TABLE_KEY_COUNT> tableKey = 
{
    "Combination of Positives (Format: #-#-#)",
    "MPN index (per 100 ml)",
    "95% Confidence Limits of Lower",
    "95% Confidence Limits of Upper"
};

/*
    主菜单列表：
    0. 返回文件路径输入界面
    1. 插入模式
    2. 加载并输出文件
    3. 删除文件数据
    4. 搜索文件数据
    5. 退出系统
*/
const std::array<const char *, MENU_COUNT> menuTable = 
{
    "0. Back to File PATH Input",
    "1. Insert data",
    "2. Load and display data",
    "3. Delete data",
    "4. Search data",
    "q. Exit system"
};

/**
 * @brief 在终端输出菜单
 * 
 * @return  non-return
*/
void showMenu();

/*显示 版本号，详情和帮助*/
void showVersion();
void showInfo();
void showHelp();

/**
 * @brief 根据用户在终端输入的参数，输出不同的信息
 * 
 * @param __optionString    用户输入的选项字符串，如 -v -h -info 等
 * 
 * @return                  non-return
*/
void loadOptions(const char * __optionString);

#endif // _OPTIONS_AND_MENU_H_