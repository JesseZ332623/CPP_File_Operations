#ifndef _OPTIONS_AND_MENU_H_
#define _OPTIONS_AND_MENU_H_

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