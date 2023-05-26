/**
 * @ 青空だけが見たいのは我儘ですか
 * @Author       : RagnaLP
 * @Date         : 2023-05-23 14:57:55
 * @LastEditTime : 2023-05-26 18:11:25
 * @Description  : 文件系统的常量与用到的标准库
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

/// 文件路径
const string FILE_PATH = "./data.byd";
/// 单块数据块字节大小
const int BLOCK_SIZE = 32;
/// 数据块总数
const int BLOCK_NUM = 16;
/// 单个成组链中最大记录条数
const int LIST_BLOCK_SIZE = BLOCK_SIZE / sizeof(int);
/// 记录成组链表的数据块数量（成组链组数）
const int LIST_BLOCK_NUM = BLOCK_NUM / (LIST_BLOCK_SIZE - 1);
/// 单个数据块中存放数据的最大字节数
const int DATA_BLOCK_SIZE = BLOCK_SIZE - sizeof(int);
/// 存放文件数据的数据块总数
const int DATA_BLOCK_NUM = BLOCK_NUM - LIST_BLOCK_NUM;

#endif