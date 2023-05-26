/**
 * @ ��դ�����Ҋ�����Τ��҃��Ǥ���
 * @Author       : RagnaLP
 * @Date         : 2023-05-23 15:00:16
 * @LastEditTime : 2023-05-26 09:00:45
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
    /// �û�����ģ��
    UserManager user_manager;
    /// Ŀ¼����ģ��
    MenuManager menu_manager;

    /**
     * @brief ���ϵͳ
     *
     */
    void Clear() {
        menu_manager.Clear();
        user_manager.Clear();
    }

public:
    /**
     * @brief ��ʼ�������ļ�ϵͳ
     *
     */
    void Initalize(string root_password) {
        menu_manager.Initialize();
        user_manager.Initialize(root_password);
    }

    /**
     * @brief ��¼���ļ�ϵͳ
     * @return int ��¼״̬,0:�������,-1:�û�������
     */
    int Login(string username, string password) {
        return user_manager.Login(username, password);
    }
    /**
     * @brief ע���ļ�ϵͳ�е��û�
     * @return ע���Ƿ�ɹ�
     */
    bool SignUp(string username, string password) {
        return user_manager.SignUp(username, password);
        // if(result) {
        //     cout << "ע��ɹ���" << endl;
        // }
        // else {
        //     cout << "�û����Ѵ��ڣ�" << endl;
        // }
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
    /**
     * @brief ��ת���µ�Ŀ¼
     *
     * @param folder_path ��Ŀ¼��·��
     * @return true ��ת�ɹ�
     * @return false ��תʧ��
     */
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
     * @brief ���浽�ļ�
     * Ŀ¼ϵͳ
     * �û�ϵͳ
     */
    bool Save() {
        FILE* f = fopen(FILE_PATH.c_str(), "w");
        if(f == NULL)
            return false;
        menu_manager.Save(f);
        user_manager.Save(f);
        fclose(f);
        return true;
    }
    /**
     * @brief ���ļ���ȡ
     *
     */
    bool Load() {
        Clear();
        FILE* f = fopen(FILE_PATH.c_str(), "r");
        if(f == NULL)
            return false;
        menu_manager.Load(f);
        user_manager.Load(f);
        fclose(f);
        return true;
    }
    /**
     * @brief ������еĵ�����Ϣ
     *
     */
    void Debug() {
        user_manager.Debug();
        menu_manager.Debug();
    }

} FileSystem;