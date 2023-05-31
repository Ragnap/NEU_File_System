/**
 * @ ��դ�����Ҋ�����Τ��҃��Ǥ���
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
     * @brief �����ǰ�еĺ���δ��������
     *
     */
    void ClearFollowInput() {
        char c = getchar();
        while(c != '\n')
            c = getchar();
    }
    /**
     * @brief ��ȡһ�п���̨����
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
     * @brief �����밴�շָ������������ɴ�
     *
     * @param str �������ַ���
     * @param separator �ָ��ַ���
     * @param separate_time ���ָ�����,Ĭ�����޴�
     * @return vector<string> ���ֽ��
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
     * @brief ��������Ķ�ȡһ���ַ��������ƻ�һ���еĺ�������
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
     * @brief ��һ�������и����ַ����������Կ��ַ�Ϊ�ָ�����ȡ���ɸ��ַ�������ֵ���÷�ʽ���ض�ȡ���
     *
     * @param str_num �ַ�������
     * @param strs ��ȡ�����ַ�����
     * @param check_function ���ڼ�������Ƿ�Ϸ��ĺ���,�Ϸ�ʱ����true
     * @return int ��ȡ״̬ 0:������ȡ -1:�������� -2:���벻�Ϸ�
     */
    int ReadString(int str_num, vector<string>& strs, bool (*check_function)(string) = nullptr) {
        string line = ReadLine();
        vector<string> res = Separate(line, " \t\n", str_num - 1);
        // ����У��
        if(res.size() != str_num)
            return -1;
        // �Ϸ���У��
        for(int i = 0; i < str_num; i++)
            if(check_function(res[i]) == false)
                return -2;
        strs = res;
        return 0;
    }

    /**
     * @brief ��ȡ��һ�����ַ�ǰ��һ���ַ���������һ���еĺ������룬��ֵ���÷�ʽ���ض�ȡ���
     *
     * @param str ��ȡ�����ַ���
     * @param check_function ���ڼ�������Ƿ�Ϸ��ĺ���,�Ϸ�ʱ����true
     * @return int ��ȡ״̬ 0:������ȡ -1:�������� -2:���벻�Ϸ�
     */
    int ReadString(string& str, bool (*check_function)(string) = nullptr) {
        vector<string> res;
        int result = ReadString(1, res, check_function);
        if(result == 0)
            str = res[0];
        return result;
    }

    /**
     * @brief ����ַ�����δ���ֶ��ֽ���ĸ,���ڱ���������ڱ�����ܵ��µ���ʾ����
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
     * @brief ����ַ������Ƿ�����ų��ַ����еĵ��ַ�
     *
     * @param str ������ַ���
     * @param exclude_chars �ų��ַ���
     * @return true �������ų��ַ�
     * @return false ��������һ���ų��ַ�
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
     * @brief ����ַ������Ƿ���ֿ����ַ�
     */
    bool CheckNoControlChar(string str) {
        for(int i = 0; i < str.length(); i++) {
            if(' ' > str[i] || '~' < str[i])
                return 0;
        }
        return 1;
    }

    /**
     * @brief ����ַ������Ƿ���ֿհ��ַ�
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
     * @brief ����Windows�¿���̨�����ɫ 0:�� 1:�� 2:�� 3:���� 4:�� 5:�� 6:�� 7:�� 8:�� 9:���� 10:���� 11:ǳ�� 12:���� 13:���� 14:���� 15:����
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
     * @brief ���ָ����ɫ��������һ�У���ָ�����д���
     *
     * @param str �������
     * @param follow_blank_line ���д�����Ĭ��Ϊ1
     * @param indent �����ַ�����Ĭ��Ϊ0
     * @param PrintInColor ��ɫ����ѡ��Ĭ��Ϊ��ɫ
     */
    void PrintLine(string str, int follow_blank_line = 1, int indent = 0, void (*PrintInColor)(string) = PrintInWhite) {
        while(indent--)
            cout << " ";
        PrintInColor(str);
        while(follow_blank_line--)
            cout << endl;
    }
    /**
     * @brief ���һ�к�ɫ�Ĵ�����Ϣ
     *
     * @param str ��������
     * @param follow_blank_line ���д�����Ĭ��Ϊ1
     * @param indent �����ַ���Ĭ��Ϊ4
     */
    void PrintError(string str, int follow_blank_line = 1, int indent = 4) {
        PrintLine(str, follow_blank_line, indent, PrintInRed);
    }
    /**
     * @brief ���һ�л�ɫ�ľ�����Ϣ
     *
     * @param str ��������
     * @param follow_blank_line ���д�����Ĭ��Ϊ1
     * @param indent �����ַ���Ĭ��Ϊ4
     */
    void PrintWarn(string str, int follow_blank_line = 1, int indent = 4) {
        PrintLine(str, follow_blank_line, indent, PrintInYellow);
    }
    /**
     * @brief ���һ����ɫ����ʾ��Ϣ
     *
     * @param str ��������
     * @param follow_blank_line ���д�����Ĭ��Ϊ1
     * @param indent �����ַ���Ĭ��Ϊ4
     */
    void PrintInfo(string str, int follow_blank_line = 1, int indent = 4) {
        PrintLine(str, follow_blank_line, indent, PrintInBlue);
    }
};
namespace CustomTransform {
    // GB2312��UTF8
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
    // GB2312��UTF8
    string GB2312_to_UTF8(string GB2312_str) {
        return GB2312_to_UTF8(GB2312_str.c_str());
    }

    // UTF8��GB2312
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
    // UTF8��GB2312
    string UTF8_to_GB2312(string UTF8_str) {
        return UTF8_to_GB2312(UTF8_str.c_str());
    }
};
using namespace CustomInput;
using namespace CustomOutput;
using namespace CustomTransform;

#endif