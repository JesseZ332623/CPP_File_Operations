#include "./file_operator.h"


void print_split_line(int length, char style)
{
    for (int len = 0; len < length; ++len)
    {
        std::putchar(style);
    }
    std::putchar('\n');
}


inline int extract_number(std::string & _cop)
{

/*这种方法通用性更高，但还有更简洁的*/
#if false
    std::stringstream ss_cop(_cop);
    int number = 0;
    for (auto ch : _cop) 
    { 
        if (ch != '-') { ss_cop << ch; }
    }

    ss_cop >> number;

#endif
    /*
        将字符串 #-#-# 的数字提取出来，变成一个 3 位整数，
        这比使用 std::stringstream 更加简洁，但只能适应这一种格式。
    */
    return EXTRACT(_cop);
}

void struct_swap(Data & _a, Data & _b)
{
    Data  _temp = _a;

    _a = _b;

    _b = _temp;
}

inline void sort_data_struct(Data * _data, const int _line_count)
{
/*选择排序怎么可能快的过 std::sort 呢*/
#if false
    for (int index = 0; index < _line_count; ++index)
    {
        int min_index = index;
        for (int _index = index + 1; _index < _line_count; ++_index)
        {
            if (extract_number(_data[_index].COP) < extract_number(_data[min_index].COP))
            {
                min_index = _index;
            }
        }

        struct_swap(_data[index], _data[min_index]);
    }
#endif
    //auto sort_rules = [](Data & _a, Data & _b) -> bool { return EXTRACT(_a.COP) < EXTRACT(_b.COP); };
    std::sort(_data, _data + _line_count, SORT_RULES);

}

int binary_search(Data * _data, std::string & _target, const int _line_count)
{
    int begin = 0;
    int end = _line_count;

    while (begin <= end)
    {
        int mid_index = begin + (end - begin) / 2;

        if (EXTRACT(_data[mid_index].COP) == EXTRACT(_target)) { return mid_index; }
        else if (EXTRACT(_data[mid_index].COP) < EXTRACT(_target)) { begin = mid_index + 1; }
        else if (EXTRACT(_data[mid_index].COP) > EXTRACT(_target)) { end = mid_index - 1; }
    }

    return -1;
}

int input(Data * _data, const std::string _file_path)
{
    /*检查入参，_data 不得为空*/
    if (!_data) { std::cout << "null argument: _data" << std::endl; return -1; }

    /*
        打开以读模式（std::ios::in）要读取的文件 data_file.txt

        _read_file 是 std::ifstream 类的对象，
        将一个文件路径字符串传入，它会自行匹配构造函数。

        当然，也可以这么写：

        std::ifstream _read_file;
        _read_file.open("./data_file.txt");
    */
    std::ifstream _read_file(_file_path, std::ios::in);

    /*创建一个字符串类，到时候调用 std::getline() 逐行读取*/
    std::string _file_line_str;

    /*统计该文本文件的行数*/
    int line_count = 0;

    /*判断文件是否打开成功*/
    if (!_read_file.is_open()) 
    { 
        std::cout << "Open " << _file_path << " failed..." << std::endl;
        return -1;
    }
    std::cout << "Load File: " << _file_path << std::endl;
    
    /*逐行读取并按照相应的格式存入结构体*/
    while (std::getline(_read_file, _file_line_str))
    {
        /*当然，如果文件的行数超过了预设的最大行数，就报错并跳出循环*/
        if (line_count >= MAX_DATA_COUNT) { std::cout << "The Struct Data if full!" << std::endl; break; }

/*
    该代码块用于分割字符串，将 "4-2-0 22 9 56"
    分割成 "4-2-0" "22" "9" "56" 这样的子串（不想转成 C 风格字符串再使用 std::strtol() 和 std::strtok() (逃)）
*/
#if true

        std::stringstream sub_string(_file_line_str);
        std::vector<std::string> _split_result;
        std::string _temp_str;

        while (std::getline(sub_string, _temp_str, ' '))
        {
            _split_result.push_back(_temp_str);
        } 
#endif
        /*
            将分割的字串逐一存入结构体之中
            std::stoi() 用于将一个 std::string 类型转化成整数。
        */
        _data[line_count].COP = _split_result[0];
        _data[line_count].MPN_index = std::stoi(_split_result[1]);
        _data[line_count].lower = std::stoi(_split_result[2]);
        _data[line_count].upper = std::stoi(_split_result[3]);

        ++line_count;
    }

    /*报告读取完毕和文件的行数*/
    std::cout << "Load Complete. file line: " << line_count << " \n";
    //printf("Length of string COP: %zd %d\n", _data[0].COP.length(), std::strcmp(_data[0].COP.c_str(), "1-1-3"));

    /*排序，按照 Combination of Positives 升序排序*/
    sort_data_struct(_data, line_count);

    /*读取完毕后记得关闭该文件*/
    _read_file.close();

    /*返回文件行数*/
    return line_count;
}

bool insert(const std::string _file_path, int _line_count, Data * temp_datapointer)
{
    /*以末尾插入模式（std::ios::app）打开文件*/
    std::ofstream _write_file(_file_path, std::ios::app);
    std::string _data_string;
    std::string _temp_string;

    std::cout << "Enter Data to Insert (Press q to back): \n";

    /*输入 Combination of Positives*/
    std::cout << "Enter Combination of Positives (Format: #-#-#): \n";
    std::getline(std::cin, _temp_string);

    /*若输入了 q 就退出插入模式，回到菜单*/
    IF_QUIT(_temp_string);
   
    /*检查用户输入的格式是否为 #-#-# 如果不是就得重新输入，避免写入垃圾数据*/
    while (CHECK_FORMAT(_temp_string))
    {
        std::cerr << "Invalid Format!\n" << std::endl;
        std::clog << "Enter Combination of Positives (Format: #-#-#): \n";
        std::getline(std::cin, _temp_string);
        IF_QUIT(_temp_string);

#if false
        if (binary_search(temp_datapointer, _temp_string, _line_count) != -1)
        {
            std::cerr << "WARNING: Insert duplicate Combination of Positions! " << _temp_string << std::endl;
            std::clog << "Enter Combination of Positives (Format: #-#-#): \n";
            std::getline(std::cin, _temp_string);
            IF_QUIT(_temp_string);

            //while (std::getchar() != '\n') { continue; }
        }

        //printf("bin search return index: %d\n", binary_search(temp_datapointer, _temp_string, _line_count));
#endif
    }

    /*
        若输入的数据通过了格式检查，就来到重复数据检测环节。
        若在结构体中找到了用户输入的数据，则证明输入了重复数据
    */
    while (binary_search(temp_datapointer, _temp_string, _line_count) != -1)
    {
        std::cerr << "WARNING: Insert duplicate Combination of Positions! " << _temp_string << std::endl;
        std::clog << "Enter Combination of Positives (Format: #-#-#): \n";
        std::getline(std::cin, _temp_string);
        IF_QUIT(_temp_string);

        /*检查用户输入的格式是否为 #-#-# 如果不是就得重新输入，避免写入垃圾数据*/
        while (CHECK_FORMAT(_temp_string))
        {
            std::cerr << "Invalid Format!\n" << std::endl;
            std::clog << "Enter Combination of Positives (Format: #-#-#): \n";
            std::getline(std::cin, _temp_string);
            IF_QUIT(_temp_string);
        }
    }

    /*使用 += 拼接字符串*/
    _data_string += _temp_string;

    /*输入 Enter MPN index*/
    std::cout << "Enter MPN index (per 100 ml): \n";
    std::getline(std::cin, _temp_string);
    IF_QUIT(_temp_string);

    _data_string += " ";
    _data_string += _temp_string;

    /*输入 Lower*/
    std::cout << "Enter 95% Confidence Limits of Lower: \n";
    std::getline(std::cin, _temp_string);
    IF_QUIT(_temp_string);

    _data_string += " ";
    _data_string += _temp_string;

    /*输入 Upper*/
    std::cout << "Enter 95% Confidence Limits of Upper: \n";
    std::getline(std::cin, _temp_string);
    IF_QUIT(_temp_string);

    _data_string += " ";
    _data_string += _temp_string;

    /*最终 把字符串 _data_string 拼接成 #-#-# ## ## ## 的形式，写入文件末尾*/

    _write_file << '\n' << _data_string;

    std::cout << "OK!\tdata:[" << _data_string << "]\tInsert Complete.\n";
    print_split_line(70, '-');

    /*关闭*/
    _write_file.close();

    return true;
}

bool search(Data * _data, const int _line_count)
{
    /*判断入参，_data 为空 或 _line_count 为 -1 时，报错并返回。*/
    if (!_data || _line_count == -1) { std::cout << "Display Failed...." << std::endl; return false; }

    std::string search_value;
    std::cout << "Enter Combination of Positive (Format: #-#-#): \n";
    std::getline(std::cin, search_value);
    IF_QUIT(search_value);

    while (CHECK_FORMAT(search_value))
    {
        std::clog << "Invalid Format!\n" << std::endl;
        std::cout << "Enter Combination of Positives (Format: #-#-#): \n";
        std::getline(std::cin, search_value);
        IF_QUIT(search_value);
    }
        
/*暴力搜索过于缓慢了，改用二分法搜索更合适*/
#if false
    for (int index = 0; index < _line_count; ++index)
    {
        //printf("%d %d\n", EXTRACT(_data[index].COP), EXTRACT(search_value));
        if (_data[index].COP == search_value)
        {
            std::cout << "OK!\t find data: \n";
            print_split_line(70, '-');
            std::cout << "Combination of Positives" << '\t' << "MPN index(per 100 ml)" << '\t' << "Lower" << '\t' << "Upper" << '\n';
            print_split_line(70, '-');
            std::cout << _data[index].COP << "\t\t\t\t" 
                  << _data[index].MPN_index << "\t\t\t" << _data[index].lower << '\t' << _data[index].upper << '\n';
            print_split_line(70, '-');

            return;
        }
    }
#endif
    
    int target_index = 0;
    /*二分法搜索不到 10 个数据，太快了 O(1)，所以计时功能目前毫无意义*/
    TIMER((target_index =  binary_search(_data, search_value, _line_count)));
    
    if (target_index == -1) 
    { 
        std::clog << "Not Found Combination of Positive: [" << search_value << ']' << std::endl;
        return false;
    } 
    else
    {
        std::cout << "OK!\t find data: \n";
        print_split_line(70, '-');
        std::cout << "Combination of Positives" << '\t' << "MPN index(per 100 ml)" << '\t' << "Lower" << '\t' << "Upper" << '\n';
        print_split_line(70, '-');
        std::cout << _data[target_index].COP << "\t\t\t\t" << _data[target_index].MPN_index << 
         "\t\t\t" << _data[target_index].lower << '\t' << _data[target_index].upper << '\n';
        print_split_line(70, '-');
    }

    return true;
}
    

void show_contence(Data * _data, const int _line_count)
{
    /*判断入参，_data 为空 或 _line_count 为 -1 时，报错并返回。*/
    if (!_data || _line_count == -1) { std::cout << "Display Failed...." << std::endl; return; }

    print_split_line(70, '-');
    std::cout << "Combination of Positives" << '\t' << "MPN index(per 100 ml)" << '\t' << "Lower" << '\t' << "Upper" << '\n';
    print_split_line(70, '-');
    for (int index = 0; index < _line_count; ++index)
    {
        std::cout << _data[index].COP << "\t\t\t\t" 
                  << _data[index].MPN_index << "\t\t\t" << _data[index].lower << '\t' << _data[index].upper << '\n';
                  Sleep(60);
    }
    print_split_line(70, '-');
}