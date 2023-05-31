/**
 * @ ��դ�����Ҋ�����Τ��҃��Ǥ���
 * @Author       : RagnaLP
 * @Date         : 2023-05-23 15:00:16
 * @LastEditTime : 2023-05-31 20:50:20
 * @Description  : �ļ�ϵͳ��
 */

#include "src/block_manager.h"
#include "src/config.h"
#include "src/memory_manager.h"
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
    BlockMananger block_manager;
    /// �ڴ����ģ��
    MemoryManager memory_manager;
    /**
     * @brief ���ϵͳ
     *
     */
    void Clear() {
        block_manager.Clear();
        menu_manager.Clear();
        user_manager.Clear();
        memory_manager.Clear();
    }
    /**
     * @brief ��ȡĿ¼����,Ŀ¼���ݿ�Ź̶�Ϊ BLOCK_NUM-1
     */
    string ReadMenu() {
        return block_manager.ReadFile(BLOCK_NUM - 1);
    }
    /**
     * @brief ��ȡinode�ڵ����ݣ����ݿ�Ź̶�Ϊ BLOCK_NUM-2
     */
    string ReadMemory() {
        return block_manager.ReadFile(BLOCK_NUM - 2);
    };
    /**
     * @brief ��ȡ�û�����,���ݿ�Ź̶�Ϊ BLOCK_NUM-3
     */
    string ReadUser() {
        return block_manager.ReadFile(BLOCK_NUM - 3);
    }

public:
    /**
     * @brief ��ʼ�������ļ�ϵͳ
     *
     */
    void Initalize(string root_password) {
        block_manager.Initialize();
        memory_manager.Initialize();
        menu_manager.Initialize();
        user_manager.Initialize(root_password);
        CreateFile("/~menu", "SYSTEM");
        CreateFile("/~inode", "SYSTEM");
        CreateFile("/~user", "SYSTEM");
        OpenFile("/~menu");
        OpenFile("/~inode");
        OpenFile("/~user");
        WriteFile("/~menu", menu_manager.Save(), "SYSTEM");
        WriteFile("/~user", user_manager.Save(), "SYSTEM");
        WriteFile("/~inode", memory_manager.Save(), "SYSTEM");
        getchar();
        getchar();
        CloseFile("/~menu");
        CloseFile("/~inode");
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
        int block_id = block_manager.CreateFile();
        if(block_id == -1)
            return -1;
        int inode_id = memory_manager.CreateFile(block_id, (user.empty() ? GetCurrentUser() : user));
        if(inode_id == -1) {
            block_manager.DeleteFile(block_id);
            return -3;
        }
        int result = menu_manager.CreateFile(file_path, inode_id);
        if(result == -1) {
            block_manager.DeleteFile(block_id);
            return 0;
        }
        if(result == -2) {
            block_manager.DeleteFile(block_id);
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
        string content = block_manager.ReadFile(memory_manager.GetDiscAddress(inode));
        int result = memory_manager.OpenFile(inode, content);
        if(result < 0)
            return result - 2;
        return 0;
    }
    /**
     * @brief �ر��ļ����Ƴ��ڴ�
     *
     * @param file_path �ļ�·��
     * @return int -3:δ�� -2:·������ -1:�޶�Ӧ�ļ� 0:�ɹ�
     */
    int CloseFile(string file_path) {
        int inode = menu_manager.GetInodeID(file_path);
        if(inode < 0)
            return inode;
        int result = memory_manager.CloseFile(inode);
        if(result < 0)
            return -3;
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
        if(!memory_manager.IsOpened(inode))
            return make_pair(-3, "");
        return make_pair(0, memory_manager.ReadFile(inode));
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
        if(!memory_manager.IsOpened(inode))
            return -3;
        // д�뵽�ڴ�
        int result = memory_manager.WriteFile(inode, (user.empty() ? GetCurrentUser() : user), content);
        if(result < 0)
            return result - 2;
        // д�뵽����
        return block_manager.WriteFile(memory_manager.GetDiscAddress(inode), content);
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
            int disc_address = memory_manager.GetDiscAddress(files[1].first);
            // ������޸�Ȩ�޲���ɾ��
            if(memory_manager.DeleteFile(files[1].first, GetCurrentUser()))
                block_manager.DeleteFile(disc_address);
            return 0;
        }
        // ɾ���ļ���
        if(files[0].first != -3) {
            // ����ɾ���ǿ��ļ����µ������ļ�
            for(auto i: files) {
                menu_manager.DeleteFile(i.second, file_path);
                int disc_address = memory_manager.GetDiscAddress(i.first);
                // ������޸�Ȩ�޲���ɾ��
                if(memory_manager.DeleteFile(i.first, GetCurrentUser()))
                    block_manager.DeleteFile(disc_address);
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
     * @brief �����ڴ�ʹ�����
     *
     * @return pair<int,int> (��ʹ�ã���)
     */
    pair<int, int> GetCapacity() {
        return memory_manager.GetCapacity();
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
        WriteFile("/~inode", memory_manager.Save());
        block_manager.Save(f);
        fclose(f);
        // ����Ϊ���ۿɿ��������ļ�
        FILE* detail_f = fopen(DETAIL_FILE_PATH.c_str(), "w");
        menu_manager.Save(detail_f);
        user_manager.Save(detail_f);
        memory_manager.Save(detail_f);
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
        block_manager.Load(f);
        menu_manager.Load(ReadMenu());
        memory_manager.Load(ReadMemory());
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
        memory_manager.Debug();
        block_manager.Debug();
        user_manager.Debug();
        menu_manager.Debug();
    }

} FileSystem;