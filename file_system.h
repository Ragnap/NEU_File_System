/**
 * @ ��դ�����Ҋ�����Τ��҃��Ǥ���
 * @Author       : RagnaLP
 * @Date         : 2023-05-23 15:00:16
 * @LastEditTime : 2023-05-23 20:11:24
 * @Description  : �ļ�ϵͳ��
 */

#include "src/config.h"
#include "src/menu_manager.h"
#include "src/user_manager.h"

/**
 * @brief �ļ�ϵͳ��
 * ���������ģ�����ͨѶ��ͬʱ��������Ľ���
 */
typedef class FileSystem {
private:
public:
    /// �û�����ģ��
    UserManager user_manager;
    /// Ŀ¼����ģ��
    MenuManager menu_manager;

    /**
     * @brief ��ʼ�������ļ�ϵͳ
     *
     */
    void Initalize() {
        menu_manager.Initialize();
    }

    /**
     * @brief ��¼���ļ�ϵͳ
     *
     */
    void Login(string username, string password) {
        int result = user_manager.Login(username, password);
        if(result == -1) {
            cout << "�û������ڣ�" << endl;
        }
        if(result == 0) {
            cout << "�������" << endl;
        }
    }
    /**
     * @brief ע���ļ�ϵͳ�е��û�
     *
     */
    void SignUp(string username, string password) {
        bool result = user_manager.SignUp(username, password);
        if(result) {
            cout << "ע��ɹ���" << endl;
        }
        else {
            cout << "�û����Ѵ��ڣ�" << endl;
        }
    }

    /**
     * @brief �����ļ���
     *
     * @param folder_name �ļ�����
     */
    bool CreateFolder(string folder_name) {
        return menu_manager.CreateFolder(folder_name);
    }

    /**
     * @brief �����ļ�
     *
     * @param file_name �ļ���
     */
    bool CreateFile(string file_name) {
        return menu_manager.CreateFile(file_name, 100);
    }

    bool ChangeFolder(string folder_path) {
        return menu_manager.OpenFolder(folder_path);
    }
    /**
     * @brief ��ȡ��ǰ·��
     *
     * @return string
     */
    string GetCurrentPath() {
        return menu_manager.GetNowFolderName();
    }
    string GetCurrentUser() {
        return user_manager.GetCurrentUser();
    }
    void ShowFolder() {
        menu_manager.ShowFolder();
    }
    /**
     * @brief ������еĵ�����Ϣ
     *
     */
    void Debug() {
        menu_manager.Debug();
    }
} FileSystem;