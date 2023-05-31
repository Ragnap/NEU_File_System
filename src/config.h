/**
 * @ ��դ�����Ҋ�����Τ��҃��Ǥ���
 * @Author       : RagnaLP
 * @Date         : 2023-05-23 14:57:55
 * @LastEditTime : 2023-05-31 15:55:45
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
#include <sstream>
#include <stack>
#include <string>
#include <vector>
using namespace std;

/// �ļ�·��
const string FILE_PATH = "./data.byd";
/// ���ӻ����ļ�·��
const string DETAIL_FILE_PATH = "./detail_data.byd";
/// �������ݿ��ֽڴ�С
const int BLOCK_SIZE = 32;
/// ���ݿ�����
const int BLOCK_NUM = 64;
/// ����������������¼����
const int LIST_BLOCK_SIZE = BLOCK_SIZE / sizeof(int);
/// ��¼������������ݿ�������������������
const int LIST_BLOCK_NUM = BLOCK_NUM / (LIST_BLOCK_SIZE - 1);
/// �������ݿ��д�����ݵ�����ֽ���
const int DATA_BLOCK_SIZE = BLOCK_SIZE - sizeof(int);
/// ����ļ����ݵ����ݿ�����
const int DATA_BLOCK_NUM = BLOCK_NUM - LIST_BLOCK_NUM;

/// �ڴ���С
const int MEM_BLOCK_SIZE = 32;
/// �ڴ������
const int MEM_BLOCK_NUM = 32;
/// inode����
const int INODE_NUM = 10;
#endif