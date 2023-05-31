/**
 * @ 青空だけが見たいのは我儘ですか
 * @Author       : RagnaLP
 * @Date         : 2023-05-23 15:17:14
 * @LastEditTime : 2023-05-31 20:51:46
 * @Description  :
 */

#include "file_system.h"
#include "lib/custom_io_lib.h"
#undef CreateFile
FileSystem file_system;
int main() {
    // 指令
    string ope = "";
    // 操作数
    vector<string> arg;
    // 操作结果
    int result;

    // 检测是否存在已有系统
    int flag = 0;
    ifstream file(FILE_PATH.c_str());
    if(file.good()) {
        cout << "发现已存在系统，是否载入？ (Y/n)" << endl;
        cin >> ope;
        if(ope == "" || ope == "Y")
            flag = 1;
        else
            flag = 0;
    }
    file.close();
    // 系统数据载入
    if(flag == 1) {
        file_system.Load();
    }
    else {
        cout << "初次使用系统，请设置root用户密码：" << endl;
        while(1) {
            int input_check = ReadString(1, arg, CheckCharOneByte);
            if(input_check == -1)
                PrintError("输入参数不足");
            else if(input_check == -2)
                PrintError("参数非法");
            else
                break;
        }
        file_system.Initalize(arg[0]);
    }
    // 系统每次循环对应单个用户
    while(1) {
        system("cls");
        // 登录系统
        while(1) {
            string username, password;
            cout << "用户名: ";
            ReadString(username, CheckNoControlChar);
            cout << "密  码: ";
            ReadString(password, CheckNoControlChar);

            int result = file_system.Login(username, password);
            if(result == -1)
                PrintError("用户不存在！");
            else if(result == 0)
                PrintError("密码错误！");
            else {
                PrintInfo("欢迎登录！");
                break;
            }
        }
        // 系统处理
        while(1) {
            PrintLine("[" + file_system.GetCurrentUser() + "@FileSystem ", 0);
            PrintWarn(file_system.GetCurrentPath(), 0, 0);
            PrintLine("]# ", 0);

            ope = ReadString();
            if(ope == "ls") {
                file_system.ShowFolder();
            }
            else if(ope == "cd") {
                int input_check = ReadString(1, arg, CheckCharOneByte);
                if(input_check != 0) {
                    if(input_check == -1)
                        PrintError("输入参数不足");
                    else if(input_check == -2)
                        PrintError("参数非法");
                    continue;
                }
                if(!file_system.ChangeFolder(arg[0]))
                    PrintError("路径错误");
            }
            else if(ope == "mkdir") {
                int input_check = ReadString(1, arg, CheckCharOneByte);
                if(input_check != 0) {
                    if(input_check == -1)
                        PrintError("输入参数不足");
                    else if(input_check == -2)
                        PrintError("参数非法");
                    continue;
                }
                int result = file_system.CreateFolder(arg[0]);
                if(result == -1)
                    PrintError("存在同名文件夹");
                else if(result == -2)
                    PrintError("路径错误");
                else
                    PrintInfo("创建成功");
            }
            else if(ope == "create") {
                int input_check = ReadString(1, arg, CheckCharOneByte);
                if(input_check != 0) {
                    if(input_check == -1)
                        PrintError("输入参数不足");
                    else if(input_check == -2)
                        PrintError("参数非法");
                    continue;
                }
                int result = file_system.CreateFile(arg[0]);
                if(result == 0)
                    PrintError("存在同名文件夹");
                else if(result == -1)
                    PrintError("磁盘块空间不足");
                else if(result == -2)
                    PrintError("路径错误");
                else
                    PrintInfo("创建成功");
            }
            else if(ope == "open") {
                int input_check = ReadString(1, arg, CheckCharOneByte);
                if(input_check != 0) {
                    if(input_check == -1)
                        PrintError("输入参数不足");
                    else if(input_check == -2)
                        PrintError("参数非法");
                    continue;
                }
                int result = file_system.OpenFile(arg[0]);
                if(result == -1)
                    cout << "\t文件不存在" << endl;
                else if(result == -2)
                    PrintError("路径错误");
                else if(result == -3)
                    PrintError("文件已打开");
                else if(result == -4)
                    PrintError("内存不足");
                else if(result == -5)
                    PrintError("可用inode数量不足");
                else
                    PrintInfo("打开成功，当前内存已使用" + to_string(file_system.GetCapacity().first) + "，共 " + to_string(file_system.GetCapacity().second));
            }
            else if(ope == "close") {
                int input_check = ReadString(1, arg, CheckCharOneByte);
                if(input_check != 0) {
                    if(input_check == -1)
                        PrintError("输入参数不足");
                    else if(input_check == -2)
                        PrintError("参数非法");
                    continue;
                }
                int result = file_system.CloseFile(arg[0]);
                if(result == -1)
                    cout << "\t文件不存在" << endl;
                else if(result == -2)
                    PrintError("路径错误");
                else if(result == -3)
                    PrintError("文件未打开");
                else
                    PrintInfo("关闭成功，当前内存已使用" + to_string(file_system.GetCapacity().first) + "，共 " + to_string(file_system.GetCapacity().second));
            }
            else if(ope == "read") {
                int input_check = ReadString(1, arg, CheckCharOneByte);
                if(input_check != 0) {
                    if(input_check == -1)
                        PrintError("输入参数不足");
                    else if(input_check == -2)
                        PrintError("参数非法");
                    continue;
                }
                pair<int, string> result = file_system.ReadFile(arg[0]);
                if(result.first == -1)
                    cout << "\t文件不存在" << endl;
                else if(result.first == -2)
                    PrintError("路径错误");
                else if(result.first == -3)
                    PrintError("文件未打开");
                else {
                    PrintInfo("读取成功，共读取到" + to_string(result.second.size()) + " 个字节:", 2);
                    PrintLine(result.second, 2);
                }
            }
            else if(ope == "write") {
                int input_check = ReadString(2, arg, CheckCharOneByte);
                if(input_check != 0) {
                    if(input_check == -1)
                        PrintError("输入参数不足");
                    else if(input_check == -2)
                        PrintError("参数非法");
                    continue;
                }
                int result = file_system.WriteFile(arg[0], arg[1]);
                if(result == -1)
                    PrintError("文件不存在");
                else if(result == -2)
                    PrintError("路径错误");
                else if(result == 0)
                    PrintError("磁盘块空间不足");
                else if(result == -3)
                    PrintError("文件未打开");
                else if(result == -4)
                    PrintError("权限不足");
                else if(result == -5)
                    PrintError("内存不足");
                else
                    PrintInfo("写入成功");
            }
            else if(ope == "delete") {
                int input_check = ReadString(1, arg, CheckCharOneByte);
                if(input_check != 0) {
                    if(input_check == -1)
                        PrintError("输入参数不足");
                    else if(input_check == -2)
                        PrintError("参数非法");
                    continue;
                }
                int result = file_system.DeleteFiles(arg[0]);
                if(result == -1)
                    PrintError("文件不存在");
                else if(result == -2)
                    PrintError("路径错误");
                else
                    PrintInfo("删除成功");
            }
            else if(ope == "save") {
                file_system.Save();
            }
            else if(ope == "load") {
                file_system.Load();
            }
            else if(ope == "debug")
                file_system.Debug();
            else if(ope == "logout") {
                file_system.Save();
                break;
            }
            else if(ope == "exit") {
                file_system.Save();
                return 0;
            }
            else if(ope == "help") {
                cout << endl << "\t可用指令如下：" << endl << endl;
                cout << "\t " << setw(30) << left << "ls"
                     << " - 显示当前文件夹下的内容" << endl;
                cout << "\t " << setw(30) << left << "cd <path>"
                     << " - 按照路径进入到某个文件夹下" << endl;
                cout << "\t " << setw(30) << left << "mkdir <path/name>"
                     << " - 在指定路径下创建一个文件夹" << endl;
                cout << "\t " << setw(30) << left << "create <path/name>"
                     << " - 在指定路径下创建一个文件" << endl;
                cout << "\t " << setw(30) << left << "open <path/name>"
                     << " - 打开指定路径下的一个文件，载入内存" << endl;
                cout << "\t " << setw(30) << left << "close <path/name>"
                     << " - 关闭指定路径下的一个文件，移出内存" << endl;
                cout << "\t " << setw(30) << left << "read <path/name>"
                     << " - 读取指定路径下的一个文件" << endl;
                cout << "\t " << setw(30) << left << "write <path/name> <content>"
                     << " - 将内容写入到指定路径下的一个文件" << endl;
                cout << "\t " << setw(30) << left << "delete <path/name>"
                     << " - 删除路径对应的文件夹的所有内容或路径对应的文件" << endl;
                cout << "\t " << setw(30) << left << "save"
                     << " - 保存整个文件系统" << endl;
                cout << "\t " << setw(30) << left << "load"
                     << " - 读取整个文件系统" << endl;
                cout << "\t " << setw(30) << left << "logout"
                     << " - 登出当前用户" << endl;
                cout << "\t " << setw(30) << left << "exit"
                     << " - 退出并保存文件系统" << endl;
                cout << "\t " << setw(30) << left << "debug"
                     << " - 输出调试用的文件系统的信息" << endl;
                cout << endl;
            }
            else {
                PrintError("指令错误!");
                ClearFollowInput();
            }
        }
    }

    system("pause");
    return 0;
}