/**
 * @ 青空だけが見たいのは我儘ですか
 * @Author       : RagnaLP
 * @Date         : 2023-05-23 15:17:14
 * @LastEditTime : 2023-05-26 17:37:23
 * @Description  :
 */

#include "file_system.h"

FileSystem file_system;
int main() {
    // 指令
    string ope = "";
    // 操作数
    string arg[5];
    file_system.Load();
    // 检测是否存在已有系统
    // int flag = 0;
    // ifstream file(FILE_PATH.c_str());
    // if(file.good()) {
    //     cout << "发现已存在系统，是否载入？ (Y/n)" << endl;
    //     cin >> ope;
    //     if(ope == "" || ope == "Y")
    //         flag = 1;
    //     else
    //         flag = 0;
    // }
    // file.close();
    // 系统数据载入
    // if(flag == 1) {
    //     file_system.Load();
    // }
    // else {
    //     cout << "初次使用系统，请设置root用户密码：" << endl;
    //     cin >> arg[0];
    //     file_system.Initalize(arg[0]);
    // }
    // 系统每次循环对应单个用户
    while(1) {
        system("cls");
        // 登录系统
        // while(1) {
        //     string username, password;
        //     cout << "用户名: ";
        //     cin >> username;
        //     cout << "密  码: ";
        //     cin >> password;
        //     int result = file_system.Login(username, password);
        //     if(result == -1)
        //         cout << "用户不存在！" << endl << endl;
        //     else if(result == 0)
        //         cout << "密码错误！" << endl << endl;
        //     else {
        //         cout << endl << "欢迎登录！" << endl << endl;
        //         break;
        //     }
        // }
        file_system.Login("root", "admin");
        // 系统处理
        while(1) {
            cout << "[" << file_system.GetCurrentUser() << "@FileSystem " << file_system.GetCurrentPath() << "]# ";

            cin >> ope;
            if(ope == "ls") {
                file_system.ShowFolder();
            }
            else if(ope == "cd") {
                cin >> arg[0];
                if(!file_system.ChangeFolder(arg[0]))
                    cout << "\t文件路径出错" << endl;
            }
            else if(ope == "mkdir") {
                cin >> arg[0];
                if(!file_system.CreateFolder(arg[0]))
                    cout << "\t存在同名文件夹" << endl;
                else
                    cout << "\t创建成功" << endl;
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
            else
                cout << "指令错误!" << endl;
        }
    }

    system("pause");
    return 0;
}