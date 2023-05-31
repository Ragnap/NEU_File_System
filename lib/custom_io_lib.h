/**
 * @ 青空だけが見たいのは我儘ですか
 * @Author       : RagnaLP
 * @Date         : 2023-05-30 14:38:13
 * @LastEditTime : 2023-05-31 09:42:16
 * @Description  :
 */
#ifndef CUSTOM_IO_LIB_INCLUDED_
#define CUSTOM_IO_LIB_INCLUDED_
#include <Windows.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

namespace CustomInput {
    /**
     * @brief 清除当前行的后续未读入内容
     *
     */
    void ClearFollowInput() {
        char c = getchar();
        while(c != '\n')
            c = getchar();
    }
    /**
     * @brief 获取一行控制台输入
     *
     * @return string
     */
    string ReadLine() {
        string line;
        char c = getchar();
        if(c == '\n')
            c = getchar();
        while(c != '\n') {
            line += c;
            c = getchar();
        }
        return line;
    }

    /**
     * @brief 将输入按照分隔符集划分若干次
     *
     * @param str 带划分字符串
     * @param separator 分隔字符集
     * @param separate_time 最大分隔次数,默认无限次
     * @return vector<string> 划分结果
     */
    vector<string> Separate(string str, string separator, int separate_time = -1) {
        vector<string> result;
        int start = 0;
        for(int i = 0; i < str.length(); i++) {
            int flag = 0;
            for(int j = 0; j < separator.length(); j++) {
                if(str[i] == separator[j] && (separate_time == -1 || result.size() < separate_time)) {
                    flag = 1;
                    break;
                }
            }
            if(flag) {
                if(i != start) {
                    result.push_back(str.substr(start, i - start));
                }
                start = i + 1;
            }
        }
        result.push_back(str.substr(start));
        return result;
    }
    /**
     * @brief 不带检验的读取一个字符串，不破坏一行中的后续输入
     */
    string ReadString() {
        string str;
        char c = getchar();
        if(c == '\n')
            c = getchar();
        while(c != '\n' && c != ' ' && c != '\t') {
            str += c;
            c = getchar();
        }
        return str;
    }
    /**
     * @brief 在一行输入中根据字符串数量，以空字符为分隔，读取若干个字符串，以值引用方式返回读取结果
     *
     * @param str_num 字符串数量
     * @param strs 读取到的字符串组
     * @param check_function 用于检查输入是否合法的函数,合法时返回true
     * @return int 读取状态 0:正常读取 -1:参数不足 -2:输入不合法
     */
    int ReadString(int str_num, vector<string>& strs, bool (*check_function)(string) = nullptr) {
        string line = ReadLine();
        vector<string> res = Separate(line, " \t\n", str_num - 1);
        // 数量校验
        if(res.size() != str_num)
            return -1;
        // 合法性校验
        for(int i = 0; i < str_num; i++)
            if(check_function(res[i]) == false)
                return -2;
        strs = res;
        return 0;
    }

    /**
     * @brief 读取第一个空字符前的一个字符串并丢弃一行中的后续输入，以值引用方式返回读取结果
     *
     * @param str 读取到的字符串
     * @param check_function 用于检查输入是否合法的函数,合法时返回true
     * @return int 读取状态 0:正常读取 -1:参数不足 -2:输入不合法
     */
    int ReadString(string& str, bool (*check_function)(string) = nullptr) {
        vector<string> res;
        int result = ReadString(1, res, check_function);
        if(result == 0)
            str = res[0];
        return result;
    }

    /**
     * @brief 检查字符串中未出现多字节字母,用于避免出现由于编码可能导致的显示问题
     *
     */
    bool CheckCharOneByte(string str) {
        for(int i = 0; i < str.length(); i++) {
            if(str[i] & (1 << 8)) {
                return 0;
            }
        }
        return 1;
    }

    /**
     * @brief 检查字符串中是否出现排除字符集中的的字符
     *
     * @param str 待检查字符串
     * @param exclude_chars 排除字符集
     * @return true 无任意排除字符
     * @return false 存在至少一个排除字符
     */
    bool CheckExculde(string str, string exclude_chars) {
        for(int i = 0; i < str.length(); i++) {
            for(int j = 0; j < exclude_chars.length(); j++) {
                if(str[i] == exclude_chars[j]) {
                    return 0;
                }
            }
        }
        return 1;
    }
    /**
     * @brief 检查字符串中是否出现控制字符
     */
    bool CheckNoControlChar(string str) {
        for(int i = 0; i < str.length(); i++) {
            if(' ' > str[i] || '~' < str[i])
                return 0;
        }
        return 1;
    }

    /**
     * @brief 检查字符串中是否出现空白字符
     */
    bool CheckNoBlank(string str) {
        for(int i = 0; i < str.length(); i++) {
            if(' ' == str[i] || '\n' == str[i] || '\t' == str[i])
                return 0;
        }
        return 1;
    }
};
namespace CustomOutput {

    /*
     * @brief 设置Windows下控制台输出颜色 0:黑 1:蓝 2:绿 3:湖蓝 4:红 5:紫 6:黄 7:白 8:灰 9:淡蓝 10:淡绿 11:浅绿 12:淡红 13:淡紫 14:淡黄 15:亮白
     */
    void SetColor(int color) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    }
    void PrintInRed(string str) {
        SetColor(4);
        cout << str;
        SetColor(7);
    }
    void PrintInYellow(string str) {
        SetColor(6);
        cout << str;
        SetColor(7);
    }
    void PrintInBlue(string str) {
        SetColor(9);
        cout << str;
        SetColor(7);
    }
    void PrintInBlack(string str) {
        SetColor(0);
        cout << str;
        SetColor(7);
    }
    void PrintInWhite(string str) {
        cout << str;
    }

    /**
     * @brief 输出指定颜色与缩进的一行，并指定换行次数
     *
     * @param str 输出内容
     * @param follow_blank_line 换行次数，默认为1
     * @param indent 缩进字符数，默认为0
     * @param PrintInColor 颜色函数选择，默认为白色
     */
    void PrintLine(string str, int follow_blank_line = 1, int indent = 0, void (*PrintInColor)(string) = PrintInWhite) {
        while(indent--)
            cout << " ";
        PrintInColor(str);
        while(follow_blank_line--)
            cout << endl;
    }
    /**
     * @brief 输出一行红色的错误信息
     *
     * @param str 错误内容
     * @param follow_blank_line 换行次数，默认为1
     * @param indent 缩进字符，默认为4
     */
    void PrintError(string str, int follow_blank_line = 1, int indent = 4) {
        PrintLine(str, follow_blank_line, indent, PrintInRed);
    }
    /**
     * @brief 输出一行黄色的警告信息
     *
     * @param str 错误内容
     * @param follow_blank_line 换行次数，默认为1
     * @param indent 缩进字符，默认为4
     */
    void PrintWarn(string str, int follow_blank_line = 1, int indent = 4) {
        PrintLine(str, follow_blank_line, indent, PrintInYellow);
    }
    /**
     * @brief 输出一行蓝色的提示信息
     *
     * @param str 错误内容
     * @param follow_blank_line 换行次数，默认为1
     * @param indent 缩进字符，默认为4
     */
    void PrintInfo(string str, int follow_blank_line = 1, int indent = 4) {
        PrintLine(str, follow_blank_line, indent, PrintInBlue);
    }
};
namespace CustomTransform {
    // GB2312到UTF8
    string GB2312_to_UTF8(const char* GB2312_str) {
        int len = MultiByteToWideChar(CP_ACP, 0, GB2312_str, -1, NULL, 0);
        wchar_t* wstr = new wchar_t[len + 1];
        memset(wstr, 0, len + 1);
        MultiByteToWideChar(CP_ACP, 0, GB2312_str, -1, wstr, len);
        len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
        char* str = new char[len + 1];
        memset(str, 0, len + 1);
        WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
        delete[] wstr;
        string UTF8_str = str;
        delete[] str;
        return UTF8_str;
    }
    // GB2312到UTF8
    string GB2312_to_UTF8(string GB2312_str) {
        return GB2312_to_UTF8(GB2312_str.c_str());
    }

    // UTF8到GB2312
    string UTF8_to_GB2312(const char* UTF8_str) {
        int len = MultiByteToWideChar(CP_UTF8, 0, UTF8_str, -1, NULL, 0);
        wchar_t* wstr = new wchar_t[len + 1];
        memset(wstr, 0, len + 1);
        MultiByteToWideChar(CP_UTF8, 0, UTF8_str, -1, wstr, len);
        len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
        char* str = new char[len + 1];
        WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
        delete[] wstr;
        string GB2312_str = str;
        delete[] str;
        return GB2312_str;
    }
    // UTF8到GB2312
    string UTF8_to_GB2312(string UTF8_str) {
        return UTF8_to_GB2312(UTF8_str.c_str());
    }
};
using namespace CustomInput;
using namespace CustomOutput;
using namespace CustomTransform;

#endif