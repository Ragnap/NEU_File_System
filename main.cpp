/**
 * @ ��դ�����Ҋ�����Τ��҃��Ǥ���
 * @Author       : RagnaLP
 * @Date         : 2023-05-23 15:17:14
 * @LastEditTime : 2023-05-25 15:51:44
 * @Description  :
 */

#include "file_system.h"

FileSystem file_system;
int main() {
    file_system.Initalize();
    file_system.SignUp("root", "admin");
    file_system.Login("root", "123");
    file_system.CreateFolder("hello");
    file_system.CreateFolder("world");
    file_system.CreateFile("test.txt");

    // ָ��
    string ope;
    // ������
    string arg[5];

    while(1) {
        cout << "[" << file_system.GetCurrentUser() << "@FileSystem " << file_system.GetCurrentPath() << "]# ";

        cin >> ope;
        if(ope == "ls") {
            file_system.ShowFolder();
        }
        else if(ope == "cd") {
            cin >> arg[0];
            if(!file_system.ChangeFolder(arg[0]))
                cout << "\t�ļ�·������" << endl;
        }
        else if(ope == "mkdir") {
            cin >> arg[0];
            if(!file_system.CreateFolder(arg[0]))
                cout << "\t����ͬ���ļ���" << endl;
            else
                cout << "\t�����ɹ�" << endl;
        }
        else if(ope == "save") {
            file_system.Save();
        }

        else if(ope == "load") {
            file_system.Load();
        }
        else if(ope == "debug")
            file_system.Debug();
        else
            cout << "ָ�����!" << endl;
    }
    system("pause");
    return 0;
}