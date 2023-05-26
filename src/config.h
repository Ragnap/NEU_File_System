/**
 * @ ��դ�����Ҋ�����Τ��҃��Ǥ���
 * @Author       : RagnaLP
 * @Date         : 2023-05-23 14:57:55
 * @LastEditTime : 2023-05-26 18:11:25
 * @Description  : �ļ�ϵͳ�ĳ������õ��ı�׼��
 */

#ifndef CONFIG_INCLUDED_
#define CONFIG_INCLUDED_

#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <stack>
#include <string>
#include <vector>
using namespace std;

/// �ļ�·��
const string FILE_PATH = "./data.byd";
/// �������ݿ��ֽڴ�С
const int BLOCK_SIZE = 32;
/// ���ݿ�����
const int BLOCK_NUM = 16;
/// ����������������¼����
const int LIST_BLOCK_SIZE = BLOCK_SIZE / sizeof(int);
/// ��¼������������ݿ�������������������
const int LIST_BLOCK_NUM = BLOCK_NUM / (LIST_BLOCK_SIZE - 1);
/// �������ݿ��д�����ݵ�����ֽ���
const int DATA_BLOCK_SIZE = BLOCK_SIZE - sizeof(int);
/// ����ļ����ݵ����ݿ�����
const int DATA_BLOCK_NUM = BLOCK_NUM - LIST_BLOCK_NUM;

#endif