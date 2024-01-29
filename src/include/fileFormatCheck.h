#ifndef _FILE_FORMAT_CHECK_H_
#define _FILE_FORMAT_CHECK_H_

#include "./defs.h"

/**
 * @brief 检查文件的路径是否正确，主要检查两点：
 * 
 * @brief               1. 文件后缀是否为 .txt
 * @brief               2. 在后缀正确的情况下，能否成功打开这个文件
 * 
 * @param __filePath    文件路径
 * 
 * @return              non-return
*/
void checkPathFormat(std::string & __filePath);

#endif // _FILE_FORMAT_CHECK_H_