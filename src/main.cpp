#include "./include/file_operator.h"

int main(int argc, char const *argv[])
{
    system("cls");
    /*文件路径*/
    std::string file_path; //= "./data/data_file.txt";

    /*统计的文件行数*/
    int line_count = 0;

    /*
        菜单枚举类型，功能如下：
        INSERT  插入数据到文件末尾
        LOAD    从文件加载数据到结构体，然后再排序结构体，最后输出至屏幕或其他地方
        SEARCH  根据 Combination of Positives 查询对应的数据
    */
    enum MENU {INSERT = '1', LOAD, SEARCH};

    std::cout << "Enter The PATH of Data File: (Press q to qute):\n";
    std::getline(std::cin, file_path);

    //line_count = input(_data, file_path);

    while ((file_path.length() >= 3) && (file_path.substr(file_path.length() - 3, 3) != "txt"))
    {
        if (file_path == "q") { exit(EXIT_SUCCESS); }
        system("cls");
        std::cerr << "Invalid PATH! (File Format Error, Only Support .txt File).\n";
        std::getline(std::cin, file_path);
    }

    /*先从文件读取数据到结构体，返回这个文件的行数*/
    while (((line_count = input(_data, file_path)) == -1))
    {
        if (file_path == "q") { exit(EXIT_SUCCESS); }
        system("cls");
        std::cerr << "Invalid PATH! (Can Not Open File).\n";
        std::cout << "Enter The PATH of Data File: (Press q to qute):\n";
        std::getline(std::cin, file_path);

        while ((file_path.length() >= 3) && (file_path.substr(file_path.length() - 3, 3) != "txt"))
        {
            if (file_path == "q") { exit(EXIT_SUCCESS); }
            system("cls");
            std::cerr << "Invalid PATH! (File Format Error, Only Support .txt File).\n";
            std::getline(std::cin, file_path);
        }
    }

    /*展示结构体内容*/
    show_contence(_data, line_count);

    /*用户的选择*/
    char choice;
    std::cout << "Enter your choice: \n";
    std::cout << "1. insert data   2. load and display data  3. search data (enter q to quit)\n";
    std::cout << "Your Choice: ";
    std::cin >> choice;

    while (choice != 'q')
    {
        while (std::getchar() != '\n') { continue; }
        switch (choice)
        {
            case INSERT:
                system("cls");
                insert(file_path, line_count, _data);
                break;
            
            case LOAD:
                system("cls");
                line_count = input(_data, file_path);
                show_contence(_data, line_count);
                break;

            case SEARCH:
                system("cls");
                line_count = input(_data, file_path);
                search(_data, line_count);
                break;

            default:
                std::cout << "invalid choice." << std::endl;
                break;
        }
        std::cout << "Enter your choice: \n";
        std::cout << "1. insert data   2. load and display data  3. search data (enter q to quit)\n";
        std::cout << "Your Choice: ";
        std::cin >> choice;
    }

    std::cout << "Done.\n";

    return EXIT_SUCCESS;
}