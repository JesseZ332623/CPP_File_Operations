#ifndef _DATACLASS_H_
#define _DATACLASS_H_

#include "./defs.h"

class PositiveConfidenceLimitsTable
{
    private:
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
            std::string COP;
            int mpnIndex;

            int lower;
            int upper;
        };

        /* 考虑动态增长问题，因此创建一个由该结构体为元素的动态数组。*/
        std::vector<PositiveConfidenceLimits> pclTable;

        /*排序的规则：根据 Combination of Positives 的值进行升序排序*/
        const bool compareRule = [](PositiveConfidenceLimits & __a, PositiveConfidenceLimits & __b) { return EXTRACT_COP_TO_NUMBER(__a.COP) <  EXTRACT_COP_TO_NUMBER(__b.COP); };

        /**
         * 自制的延时函数，来代替 Windows 库中的 Sleep 函数。
         * 
         * @param __millisSeconds 要延迟的时间，精确到毫秒
         * 
         * @return non-return
        */
        void delayMilliseconds(unsigned long int __millisSeconds);

        /**
         * @brief 逐个检查字符串中的字符是否为数字。
         * 
         * @param __str 传入的字符串
         * 
         * @return 一个布尔类型，代表这个字符串是不是数字。如 "123"
        */
        inline bool isNumber(const std::string & __str);

        /**
         * 这个函数可以打印不同风格的分割线
         * @param __len 分割线的长度
         * @param __style  分割线的风格，如（------- ****** 或 ·········）
        */
        inline void printSplitLine(int __len, char __style);

        /**
         * 将动态数组 pclTable 内的所有数据按照 Combination of Positives 的值升序排列。
         * 
         * @param __pclTable pclTable 动态数组的引用
         * 
         * @return non-return
        */
        inline void sortPclTable(std::vector<PositiveConfidenceLimits> & __pclTable);

        /**
         * 二分法搜索，针对数据量较大的情况，能有效的提高效率。
         * 
         * @param __pclTable pclTable 动态数组的引用
         * @param _target   要在该数组内搜索的目标字符串
         * 
         * @return 搜索结果，若存在，返回对应的下标。不存在则返回 -1
        */
        int binarySearch(std::vector<PositiveConfidenceLimits> & __pclTable, std::string & __target);

    public:
        /**
          * 从文本文件中读入数据，并存入 pclTable 动态数组中
          * 
          * @param __pclTable pclTable 动态数组的引用
          * @param __filePath 要读取文件的路径
          * 
          * @return non-return
        */
            void readFile(std::vector<PositiveConfidenceLimits> & __pclTable, const std::string __filePath);

        /**
          * 读取完毕后，输出该表的内容
          * 
          * @param __pclTable pclTable 动态数组的引用
          * 
          * @return non-return
          */
            void showContence(std::vector<PositiveConfidenceLimits> & __pclTable);
};

#endif // _DATACLASS_H_