/**
 * @ ��դ�����Ҋ�����Τ��҃��Ǥ���
 * @Author       : RagnaLP
 * @Date         : 2023-05-23 15:00:16
 * @LastEditTime : 2023-05-31 19:46:25
 * @Description  : �ļ�ϵͳ��
 */

#include "src/block_manager.h"
#include "src/config.h"
#include "src/memery_manager.h"
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
    /// �ڴ����ģ��
    MemeryManager memery_mananger;
    /**
     * @brief ���ϵͳ
     *
     */
    void Clear() {
        block_mananger.Clear();
        menu_manager.Clear();
        user_manager.Clear();
        memery_mananger.Clear();
    }
    /**
     * @brief ��ȡĿ¼����,Ŀ¼���ݿ�Ź̶�Ϊ BLOCK_NUM-1
     */
    string ReadMenu() {
        return block_mananger.ReadFile(BLOCK_NUM - 1);
    }
    /**
     * @brief ��ȡ�û�����
     */
    string ReadUser() {
        return ReadFile("/~user").second;
    }

public:
    /**
     * @brief ��ʼ�������ļ�ϵͳ
     *
     */
    void Initalize(string root_password) {
        block_mananger.Initialize();
        memery_mananger.Initialize();
        menu_manager.Initialize();
        user_manager.Initialize(root_password);
        CreateFile("/~menu", "SYSTEM");
        CreateFile("/~user", "SYSTEM");
        OpenFile("/~menu");
        OpenFile("/~user");
        WriteFile("/~menu", menu_manager.Save(), "SYSTEM");
        WriteFile("/~user", user_manager.Save(), "SYSTEM");
        CloseFile("/~menu");
        CloseFile("/~user");
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
     * @param folder_path �ļ���·��
     */
    bool CreateFolder(string folder_path) {
        return menu_manager.CreateFolder(folder_path);
    }

    /**
     * @brief �����ļ�
     *
     * @param file_path �ļ�·��
     * @param user �����û�
     * @return int -3:Ӳ��inode�������� -2:��ַ����,-1:���̿鲻��,0:����ͬ���ļ�,1:�ɹ�
     */
    int CreateFile(string file_path, string user = "") {
        int block_id = block_mananger.CreateFile();
        if(block_id == -1)
            return -1;
        int inode_id = memery_mananger.CreateFile(block_id, (user.empty() ? GetCurrentUser() : user));
        if(inode_id == -1) {
            block_mananger.DeleteFile(block_id);
            return -3;
        }
        int result = menu_manager.CreateFile(file_path, inode_id);
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
     * @brief ���ļ����ڴ�
     *
     * @param file_path �ļ�·��
     * @return int -5:�ڴ�inode���� -4:�ڴ治�� -3:�Ѿ��� -2:·������ -1:�޶�Ӧ�ļ� 0:�ɹ�
     */
    int OpenFile(string file_path) {
        int inode = menu_manager.GetInodeID(file_path);
        if(inode < 0)
            return inode;
        string content = block_mananger.ReadFile(memery_mananger.GetDiscAddress(inode));
        int result = memery_mananger.OpenFile(inode, content);
        if(result < 0)
            return result - 2;
        return 0;
    }
    /**
     * @brief ��ȡ�ļ�
     *
     * @param file_path �ļ�·��
     * @return pair<int, string> -3:�ļ�δ�� -2:·������ -1:�޶�Ӧ�ļ� 0:�ɹ�
     */
    pair<int, string> ReadFile(string file_path) {
        int inode = menu_manager.GetInodeID(file_path);
        if(inode < 0)
            return make_pair(inode, "");
        if(!memery_mananger.IsOpened(inode))
            return make_pair(-3, "");
        return make_pair(0, memery_mananger.ReadFile(inode));
    }
    /**
     * @brief д���ļ�
     *
     * @param file_path �ļ�·��
     * @param content д�������
     * @param user ������������Ĭ��Ϊ��ǰ��¼�û�
     * @return int  -5:�ڴ治�� -4:Ȩ�޲��� -3:�ļ�δ�� -2:·������ -1:�޶�Ӧ�ļ�,0:���̿ռ䲻��,1:�ɹ�
     * @todo �ж��ļ��Ƿ����ڴ��вſɶ�ȡ
     * @todo �ж��û�Ȩ��
     */
    int WriteFile(string file_path, string content, string user = "") {
        int inode = menu_manager.GetInodeID(file_path);
        if(inode < 0)
            return inode;
        if(!memery_mananger.IsOpened(inode))
            return -3;
        // д�뵽�ڴ�
        int result = memery_mananger.WriteFile(inode, (user.empty() ? GetCurrentUser() : user), content);
        if(result < 0)
            return result - 2;
        // д�뵽����
        return block_mananger.WriteFile(memery_mananger.GetDiscAddress(inode), content);
    }

    /**
     * @brief ɾ���ļ����µ������ļ���ָ���ļ�
     *
     * @param file_path �ļ����У�·��
     * @return int -1:�ļ������� -2:·������
     */
    int DeleteFiles(string file_path) {
        // ��ȡ���е��ļ����index������
        vector<pair<int, string>> files = menu_manager.GetFiles(file_path);
        if(files[0].first == -1 || files[0].first == -2)
            return files[0].first;
        // ɾ�������ļ�
        if(files[0].first == -4) {
            menu_manager.DeleteFile(files[1].second, file_path);
            int disc_address = memery_mananger.GetDiscAddress(files[1].first);
            // ������޸�Ȩ�޲���ɾ��
            if(memery_mananger.DeleteFile(files[1].first, GetCurrentUser()))
                block_mananger.DeleteFile(disc_address);
            return 0;
        }
        // ɾ���ļ���
        if(files[0].first != -3) {
            // ����ɾ���ǿ��ļ����µ������ļ�
            for(auto i: files) {
                menu_manager.DeleteFile(i.second, file_path);
                int disc_address = memery_mananger.GetDiscAddress(i.first);
                // ������޸�Ȩ�޲���ɾ��
                if(memery_mananger.DeleteFile(i.first, GetCurrentUser()))
                    block_mananger.DeleteFile(disc_address);
            }
        }
        // ɾ�����ļ���
        menu_manager.DeleteEmptyFolder(file_path);
        return 0;
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
        // ���浽�����ļ�
        WriteFile("/~menu", menu_manager.Save());
        WriteFile("/~user", user_manager.Save());
        block_mananger.Save(f);
        fclose(f);
        // ����Ϊ���ۿɿ��������ļ�
        FILE* detail_f = fopen(DETAIL_FILE_PATH.c_str(), "w");
        menu_manager.Save(detail_f);
        user_manager.Save(detail_f);
        fclose(detail_f);
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
        // �Ӵ����ļ�����
        block_mananger.Load(f);
        menu_manager.Load(ReadMenu());
        user_manager.Load(ReadUser());
        fclose(f);
        // �������ļ�����
        // FILE* detail_f = fopen(DETAIL_FILE_PATH.c_str(), "r");
        // menu_manager.Load(detail_f);
        // user_manager.Load(detail_f);
        // fclose(detail_f);
        return true;
    }
    /**
     * @brief ������еĵ�����Ϣ
     *
     */
    void Debug() {
        memery_mananger.Debug();
        block_mananger.Debug();
        user_manager.Debug();
        menu_manager.Debug();
    }

} FileSystem;