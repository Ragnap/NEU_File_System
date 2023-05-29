/**
 * @ ��դ�����Ҋ�����Τ��҃��Ǥ���
 * @Author       : RagnaLP
 * @Date         : 2023-05-23 15:00:16
 * @LastEditTime : 2023-05-29 15:58:31
 * @Description  : �ļ�ϵͳ��
 */

#include "src/block_manager.h"
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
    /// �����ģ��
    BlockMananger block_mananger;
    /**
     * @brief ���ϵͳ
     *
     */
    void Clear() {
        block_mananger.Clear();
        menu_manager.Clear();
        user_manager.Clear();
    }

public:
    /**
     * @brief ��ʼ�������ļ�ϵͳ
     *
     */
    void Initalize(string root_password) {
        block_mananger.Initialize();
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
     * @return int -2:��ַ����,-1:���̿鲻��,0:����ͬ���ļ�,1:�ɹ�
     */
    int CreateFile(string file_name) {
        int block_id = block_mananger.CreateFile();
        if(block_id == -1)
            return -1;
        int result = menu_manager.CreateFile(file_name, block_id);
        if(result == -1) {
            block_mananger.DeleteFile(block_id);
            return 0;
        }
        if(result == -2) {
            block_mananger.DeleteFile(block_id);
            return -2;
        }
        return 1;
    }
    /**
     * @brief ��ȡ�ļ�
     *
     * @param file_name �ļ���
     * @return pair<int, string> -2:·������ -1:�޶�Ӧ�ļ� 0:�ɹ�
     * @todo ��Ҫ�ж��Ƿ����ڴ��вſɶ�ȡ�ļ�
     * @todo �ж��û�Ȩ��
     */
    pair<int, string> ReadFile(string file_name) {
        int index = menu_manager.GetAddress(file_name);
        if(index < 0)
            return make_pair(-1, "");
        return make_pair(0, block_mananger.ReadFile(index));
    }
    /**
     * @brief д���ļ�
     *
     * @param file_name �ļ���
     * @param content д�������
     * @return int  -2:·������ -1:�޶�Ӧ�ļ�,0:���̿ռ䲻��,1:�ɹ�
     * @todo �ж��ļ��Ƿ����ڴ��вſɶ�ȡ
     * @todo �ж��û�Ȩ��
     */
    int WriteFile(string file_name, string content) {
        int index = menu_manager.GetAddress(file_name);
        if(index < 0)
            return index;
        return block_mananger.WriteFile(index, content);
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
     */
    string GetCurrentPath() {
        return menu_manager.GetNowFolderName();
    }
    /**
     * @brief ��ȡ��ǰ�û���
     *
     */
    string GetCurrentUser() {
        return user_manager.GetCurrentUser();
    }
    /**
     * @brief ��ʾ��ǰ�ļ�������
     *
     */
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
        block_mananger.Save(f);
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
        block_mananger.Load(f);
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
        block_mananger.Debug();
        user_manager.Debug();
        menu_manager.Debug();
    }

} FileSystem;