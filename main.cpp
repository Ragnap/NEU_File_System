/**
 * @ ��դ�����Ҋ�����Τ��҃��Ǥ���
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
    // ָ��
    string ope = "";
    // ������
    vector<string> arg;
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
        while(1) {
            int input_check = ReadString(1, arg, CheckCharOneByte);
            if(input_check == -1)
                PrintError("�����������");
            else if(input_check == -2)
                PrintError("�����Ƿ�");
            else
                break;
        }
        file_system.Initalize(arg[0]);
    }
    // ϵͳÿ��ѭ����Ӧ�����û�
    while(1) {
        system("cls");
        // ��¼ϵͳ
        while(1) {
            string username, password;
            cout << "�û���: ";
            ReadString(username, CheckNoControlChar);
            cout << "��  ��: ";
            ReadString(password, CheckNoControlChar);

            int result = file_system.Login(username, password);
            if(result == -1)
                PrintError("�û������ڣ�");
            else if(result == 0)
                PrintError("�������");
            else {
                PrintInfo("��ӭ��¼��");
                break;
            }
        }
        // ϵͳ����
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
                        PrintError("�����������");
                    else if(input_check == -2)
                        PrintError("�����Ƿ�");
                    continue;
                }
                if(!file_system.ChangeFolder(arg[0]))
                    PrintError("·������");
            }
            else if(ope == "mkdir") {
                int input_check = ReadString(1, arg, CheckCharOneByte);
                if(input_check != 0) {
                    if(input_check == -1)
                        PrintError("�����������");
                    else if(input_check == -2)
                        PrintError("�����Ƿ�");
                    continue;
                }
                int result = file_system.CreateFolder(arg[0]);
                if(result == -1)
                    PrintError("����ͬ���ļ���");
                else if(result == -2)
                    PrintError("·������");
                else
                    PrintInfo("�����ɹ�");
            }
            else if(ope == "create") {
                int input_check = ReadString(1, arg, CheckCharOneByte);
                if(input_check != 0) {
                    if(input_check == -1)
                        PrintError("�����������");
                    else if(input_check == -2)
                        PrintError("�����Ƿ�");
                    continue;
                }
                int result = file_system.CreateFile(arg[0]);
                if(result == 0)
                    PrintError("����ͬ���ļ���");
                else if(result == -1)
                    PrintError("���̿�ռ䲻��");
                else if(result == -2)
                    PrintError("·������");
                else
                    PrintInfo("�����ɹ�");
            }
            else if(ope == "open") {
                int input_check = ReadString(1, arg, CheckCharOneByte);
                if(input_check != 0) {
                    if(input_check == -1)
                        PrintError("�����������");
                    else if(input_check == -2)
                        PrintError("�����Ƿ�");
                    continue;
                }
                int result = file_system.OpenFile(arg[0]);
                if(result == -1)
                    cout << "\t�ļ�������" << endl;
                else if(result == -2)
                    PrintError("·������");
                else if(result == -3)
                    PrintError("�ļ��Ѵ�");
                else if(result == -4)
                    PrintError("�ڴ治��");
                else if(result == -5)
                    PrintError("����inode��������");
                else
                    PrintInfo("�򿪳ɹ�����ǰ�ڴ���ʹ��" + to_string(file_system.GetCapacity().first) + "���� " + to_string(file_system.GetCapacity().second));
            }
            else if(ope == "close") {
                int input_check = ReadString(1, arg, CheckCharOneByte);
                if(input_check != 0) {
                    if(input_check == -1)
                        PrintError("�����������");
                    else if(input_check == -2)
                        PrintError("�����Ƿ�");
                    continue;
                }
                int result = file_system.CloseFile(arg[0]);
                if(result == -1)
                    cout << "\t�ļ�������" << endl;
                else if(result == -2)
                    PrintError("·������");
                else if(result == -3)
                    PrintError("�ļ�δ��");
                else
                    PrintInfo("�رճɹ�����ǰ�ڴ���ʹ��" + to_string(file_system.GetCapacity().first) + "���� " + to_string(file_system.GetCapacity().second));
            }
            else if(ope == "read") {
                int input_check = ReadString(1, arg, CheckCharOneByte);
                if(input_check != 0) {
                    if(input_check == -1)
                        PrintError("�����������");
                    else if(input_check == -2)
                        PrintError("�����Ƿ�");
                    continue;
                }
                pair<int, string> result = file_system.ReadFile(arg[0]);
                if(result.first == -1)
                    cout << "\t�ļ�������" << endl;
                else if(result.first == -2)
                    PrintError("·������");
                else if(result.first == -3)
                    PrintError("�ļ�δ��");
                else {
                    PrintInfo("��ȡ�ɹ�������ȡ��" + to_string(result.second.size()) + " ���ֽ�:", 2);
                    PrintLine(result.second, 2);
                }
            }
            else if(ope == "write") {
                int input_check = ReadString(2, arg, CheckCharOneByte);
                if(input_check != 0) {
                    if(input_check == -1)
                        PrintError("�����������");
                    else if(input_check == -2)
                        PrintError("�����Ƿ�");
                    continue;
                }
                int result = file_system.WriteFile(arg[0], arg[1]);
                if(result == -1)
                    PrintError("�ļ�������");
                else if(result == -2)
                    PrintError("·������");
                else if(result == 0)
                    PrintError("���̿�ռ䲻��");
                else if(result == -3)
                    PrintError("�ļ�δ��");
                else if(result == -4)
                    PrintError("Ȩ�޲���");
                else if(result == -5)
                    PrintError("�ڴ治��");
                else
                    PrintInfo("д��ɹ�");
            }
            else if(ope == "delete") {
                int input_check = ReadString(1, arg, CheckCharOneByte);
                if(input_check != 0) {
                    if(input_check == -1)
                        PrintError("�����������");
                    else if(input_check == -2)
                        PrintError("�����Ƿ�");
                    continue;
                }
                int result = file_system.DeleteFiles(arg[0]);
                if(result == -1)
                    PrintError("�ļ�������");
                else if(result == -2)
                    PrintError("·������");
                else
                    PrintInfo("ɾ���ɹ�");
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
                cout << endl << "\t����ָ�����£�" << endl << endl;
                cout << "\t " << setw(30) << left << "ls"
                     << " - ��ʾ��ǰ�ļ����µ�����" << endl;
                cout << "\t " << setw(30) << left << "cd <path>"
                     << " - ����·�����뵽ĳ���ļ�����" << endl;
                cout << "\t " << setw(30) << left << "mkdir <path/name>"
                     << " - ��ָ��·���´���һ���ļ���" << endl;
                cout << "\t " << setw(30) << left << "create <path/name>"
                     << " - ��ָ��·���´���һ���ļ�" << endl;
                cout << "\t " << setw(30) << left << "open <path/name>"
                     << " - ��ָ��·���µ�һ���ļ��������ڴ�" << endl;
                cout << "\t " << setw(30) << left << "close <path/name>"
                     << " - �ر�ָ��·���µ�һ���ļ����Ƴ��ڴ�" << endl;
                cout << "\t " << setw(30) << left << "read <path/name>"
                     << " - ��ȡָ��·���µ�һ���ļ�" << endl;
                cout << "\t " << setw(30) << left << "write <path/name> <content>"
                     << " - ������д�뵽ָ��·���µ�һ���ļ�" << endl;
                cout << "\t " << setw(30) << left << "delete <path/name>"
                     << " - ɾ��·����Ӧ���ļ��е��������ݻ�·����Ӧ���ļ�" << endl;
                cout << "\t " << setw(30) << left << "save"
                     << " - ���������ļ�ϵͳ" << endl;
                cout << "\t " << setw(30) << left << "load"
                     << " - ��ȡ�����ļ�ϵͳ" << endl;
                cout << "\t " << setw(30) << left << "logout"
                     << " - �ǳ���ǰ�û�" << endl;
                cout << "\t " << setw(30) << left << "exit"
                     << " - �˳��������ļ�ϵͳ" << endl;
                cout << "\t " << setw(30) << left << "debug"
                     << " - ��������õ��ļ�ϵͳ����Ϣ" << endl;
                cout << endl;
            }
            else {
                PrintError("ָ�����!");
                ClearFollowInput();
            }
        }
    }

    system("pause");
    return 0;
}