#ifndef _DATACLASS_H_
#define _DATACLASS_H_

#include "./defs.h"

class PositiveConfidenceLimitsTable
{
    private:
        /* 考虑动态增长问题，因此创建一个由该结构体为元素的动态数组。*/
        std::vector<PositiveConfidenceLimits> pclTable;
        
        /*目标文件的文件流*/
        std::fstream dataFileStream;

        /*目标文件路径*/
        std::string filePath;

    protected:
        /**
         * @brief           逐个检查字符串中的字符是否为数字。
         * 
         * @param __str     传入的字符串
         * 
         * @return          一个布尔类型，代表这个字符串是不是数字。如 "123"
        */
        inline bool isNumber(const std::string & __str) const;

        /**
         * @brief   将动态数组 pclTable 内的所有数据按照 Combination of Positives 的值升序排列。
         *          排序的规则：根据 Combination of Positives 的值进行升序排序
         * 
         * @return  non-return
        */
        inline void sortPclTable(void);

        /**
         * @brief  将 PCL 结构体的数据转换回字符串，以便在后面重新写入文件，
         *         让文件内的数据也变得有序。
         * 
         * @param  __pclStruct PCL 结构体的（只读）引用
         * 
         * @return  经过转换后的字符串
        */
        std::string pclStructToString(const PositiveConfidenceLimits & __pclStruct);

        /**
         * @brief               二分法搜索，在整个 pclTable 动态数组中搜素目标 COP，针对数据量较大的情况，能有效的提高效率（绝对比暴力搜索快）
         * 
         * @param __target      要在该数组内搜索的目标 Combination Of Positives 字符串
         * 
         * @return              搜索结果，若存在，返回对应的下标。若不存在则返回 -1（其实最先进的办法是返回迭代器，不过我懒得搞）
        */
        int binarySearch(const std::string & __target) const;

        /**
         * @brief                   按一定的规则分割字符串，然后将分割的字串存入一个动态数组中
         * 
         * @param __fileLineString  读取到的文件的一行字符串，为 #-#-# ## ## ##，其中后面的 ## 长度不限。
         * @param __subStrings      分割完字符串后存入的动态数组
         * 
         * @return                  non-return
        */
        void splitString(std::string & __fileLineString, std::vector<std::string> & __subStrings) const;

        /**
         * @brief           显示单个 PositiveConfidenceLimits 的内容
         * 
         * @param __os      标准输出流的引用
         * @param __tempPcl PositiveConfidenceLimits 结构体的引用
         * 
         * @return          non-return
        */
        inline void showOneLine(std::ostream & __os, PositiveConfidenceLimits & __tempPcl) const;
        
        /**
         * @brief               清空一个文本文件的所有内容
         * 
         * @param __filePath    目标文件路径
         * 
         * @return              文件是否成功打开以及是否完成清空操作
        */
        inline bool clearFileContent(const std::string & __filePath) const;

        /**
         * @brief           根据传入的动态数组和目标 COP 字符串，
         *                  去动态数组中找到这个字符串（顺序暴力搜索），并返回它的下标。
         * 
         * @param __fileLineArray   存储文件中读取所有行的数据的动态数组的引用
         * @param __targetCOPString 目标 COP 字符串
         * 
         * @return                  目标 COP 字符串在动态数组中的下标，若没找到则返回 -1         
        */
        std::size_t searchCOPIndex(std::vector<std::string> & __fileLineArray, const std::string & __targetCOPString) const;

    public:
        /**
         * @brief   构建函数，初始化文件路径
         * 
         * @param   __filePath 文件路径
        */
        PositiveConfidenceLimitsTable(std::string & __filePath) : filePath(__filePath) {}

        /**
         * @brief 获取当前文件路径
         * 
         * @return 当前文件路径
        */
        std::string & getFilePath(void) { return filePath; } const

        /**
         * @brief 重设文件路径
         * 
         * @param   __newFilePath 新文件路径
         * 
         * @return  non-return
        */
        void setFilePath(std::string & __newFilePath) {  filePath = __newFilePath; }

        /**
          * @brief              从文本文件中读入数据，并存入 pclTable 动态数组中
          * 
          * @return             读取和存储成功返回 true，否则返回 false
        */
        bool readFile(void);

        /**
         * @brief               用户输入相关数据，并存入文件末尾
         * 
         * @return              插入成功返回 true，否则返回 false
        */
        bool insertFile(void);

        /**
         * @brief               修改目标文件中一行中，除 Combination of Positives 的数据
         * 
         * @return              修改成功返回 true，否则返回 false
        */
        bool modifyFileLine(void);

        /**
         * @brief               用户输入 Combination of Positives 字符串，然后在文件中找到对应的这一行，将其删除
         * 
         * @return              删除成功返回 true，否则返回 false
        */
        bool deleteFileLine(void);

        /**
         * @brief           在动态数组 pclTable 中，寻找目标 COP 字符串，然后输出整行数据。
         * 
         * @return          是否查询成功
        */
        bool search(void);

        /**
         * @brief               输出 PositiveConfidenceLimitsTable 类的内容，主要是 pclTable 动态数组的内容。
         * 
         * @param __os          标准输出对象的引用
         * @param __pclTable    PositiveConfidenceLimitsTable 类对象的引用
         * 
         * @return              标准输出对象的引用，以支持链式调用
        */
        friend std::ostream & operator<<(std::ostream & __os, PositiveConfidenceLimitsTable & __pclTable);

        ~PositiveConfidenceLimitsTable() { dataFileStream.close(); };
};

#endif // _DATACLASS_H_