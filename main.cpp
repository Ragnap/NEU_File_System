/**
 * @ ��դ�����Ҋ�����Τ��҃��Ǥ���
 * @Author       : RagnaLP
 * @Date         : 2023-05-23 15:17:14
 * @LastEditTime : 2023-05-26 20:46:37
 * @Description  :
 */

#include "file_system.h"

FileSystem file_system;
int main() {
    // ָ��
    string ope = "";
    // ������
    string arg[5];
    // �������
    int result;

    // ����Ƿ��������ϵͳ
    int flag = 0;
    ifstream file(FILE_PATH.c_str());
    if(file.good()) {
        cout << "�����Ѵ���ϵͳ���Ƿ����룿 (Y/n)" << endl;
        cin >> ope;
        if(ope == "" || ope == "Y")
            flag = 1;
        else
            flag = 0;
    }
    file.close();
    // ϵͳ��������
    if(flag == 1) {
        file_system.Load();
    }
    else {
        cout << "����ʹ��ϵͳ��������root�û����룺" << endl;
        cin >> arg[0];
        file_system.Initalize(arg[0]);
    }
    // ϵͳÿ��ѭ����Ӧ�����û�
    while(1) {
        system("cls");
        // ��¼ϵͳ
        while(1) {
            string username, password;
            cout << "�û���: ";
            cin >> username;
            cout << "��  ��: ";
            cin >> password;
            int result = file_system.Login(username, password);
            if(result == -1)
                cout << "�û������ڣ�" << endl << endl;
            else if(result == 0)
                cout << "�������" << endl << endl;
            else {
                cout << endl << "��ӭ��¼��" << endl << endl;
                break;
            }
        }
        // ϵͳ����
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
            else if(ope == "create") {
                cin >> arg[0];
                int result = file_system.CreateFile(arg[0]);
                if(result == 0)
                    cout << "\t����ͬ���ļ���" << endl;
                else if(result == -1)
                    cout << "\t���̿�ռ䲻��" << endl;
                else
                    cout << "\t�����ɹ�" << endl;
            }
            else if(ope == "read") {
                cin >> arg[0];
                pair<int, string> result = file_system.ReadFile(arg[0]);
                if(result.first == -1)
                    cout << "\t�ļ�������" << endl;
                else {
                    cout << "\t��ȡ�ɹ�������ȡ�� " << result.second.size() << " ���ֽ�:" << endl << endl;
                    cout << result.second << endl << endl;
                }
            }
            else if(ope == "write") {
                cin >> arg[0] >> arg[1];
                int result = file_system.WriteFile(arg[0], arg[1]);
                if(result == -1)
                    cout << "\t�ļ�������" << endl;
                else if(result == -1)
                    cout << "\t���̿ռ䲻��" << endl;
                else
                    cout << "\tд��ɹ�" << endl;
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
                cout << "ָ�����!" << endl;
        }
    }

    system("pause");
    return 0;
}