/**
 * @ ��դ�����Ҋ�����Τ��҃��Ǥ���
 * @Author       : RagnaLP
 * @Date         : 2023-05-23 15:05:59
 * @LastEditTime : 2023-05-25 09:08:52
 * @Description  : Ŀ¼���������
 */

#include "config.h"

using namespace std;
/**
 * @brief �����ļ�Ŀ¼����
 *
 */
typedef struct BaseFileListItem {
    // �����ļ�Ŀ¼ID
    int id;
    /// �ļ�(Ŀ¼)��ַ,Ϊ�ļ���ʱָ���ļ�Ŀ¼�±�,Ϊ�ļ�ʱָ���Ӧ��index_node
    int index;
    /// ��ʶ���ļ��Ƿ�Ϊ�ļ�
    bool is_folder;
    /// ���ļ������ļ��е�����
    string name;
} BaseFileListItem;

/**
 * @brief �����ļ�Ŀ¼��
 *
 */
typedef struct BaseFileList {
    map<int, BaseFileListItem> items;

    int GetIndex(int id) {
        if(items.find(id) == items.end())
            return -1;
        else
            return items[id].index;
    }
    string GetName(int id) {
        if(items.find(id) == items.end())
            return "";
        else
            return items[id].name;
    }
    bool GetIsFolder(int id) {
        if(items.find(id) == items.end())
            return 0;
        else
            return items[id].is_folder;
    }

    /**
     * @brief �ڻ����ļ�Ŀ¼�д���һ���µ���
     *
     * @param id �����ļ�ID
     * @param index �ļ�(Ŀ¼)��ַ,Ϊ�ļ���ʱָ���ļ�Ŀ¼�±�,Ϊ�ļ�ʱָ���Ӧ��index_node
     * @param is_folder �Ƿ�Ϊ�ļ���
     * @param name ����
     */
    void Add(int id, int index, bool is_folder, string name) {
        BaseFileListItem new_item;
        new_item.id = id;
        new_item.index = index;
        new_item.is_folder = is_folder;
        new_item.name = name;
        items[id] = new_item;
    }
} BaseFileList;

/**
 * @brief �ļ�������
 *
 */
typedef struct FolderItem {
    /// �ļ�(��)��
    string name;
    /// �����ļ�Ŀ¼�±�
    int index;
} FolderItem;

/**
 * @brief �����ļ�Ŀ¼��
 * �ļ����е��ļ�Ŀ¼
 */
typedef struct Folder {
    /// �ļ����е��ļ�Ŀ¼
    vector<FolderItem> items;

    /**
     * @brief �����ڵ�ǰ�ļ���������һ��
     *
     * @param name ����
     * @param index �����ļ�Ŀ¼�±�
     * @return true �����ɹ�
     * @return false ����ͬ���ļ�
     */
    bool Add(string name, int index) {
        // ����Ƿ����ͬ���ļ�
        for(int i = 0; i < items.size(); i++)
            if(items[i].name == name)
                return false;
        // ����
        FolderItem new_item;
        new_item.name = name;
        new_item.index = index;
        items.push_back(new_item);
        return true;
    }

    /**
     * @brief �����ڵ�ǰ�ļ������Ƿ����ĳһ·�������򷵻����ڻ����ļ�Ŀ¼�е��±�
     *
     * @param name ���Ҷ�����
     * @return int �����ļ�Ŀ¼�е��±꣬δ�ҵ��򷵻�-1
     */
    int Find(string name) {
        for(int i = 0; i < items.size(); i++)
            if(items[i].name == name)
                return items[i].index;
        return -1;
    }
} Folder;

/**
 * @brief ���ļ�Ŀ¼ϵͳ��
 *
 */
typedef class MenuManager {
private:
    /// �����ļ�Ŀ¼��
    BaseFileList base_file_list;
    /// �ļ���Ŀ¼��
    map<int, Folder> folders;
    /// ��ǰ·��
    string now_folder_name;
    /// ��ǰ·����Ӧ���ļ���Ŀ¼�±�
    int now_folder;
    /// ��ǰ·����Ӧ�Ļ����ļ�Ŀ¼�±�
    int now_folder_base_id;
    /// ��ǰ�����ļ���ż���
    int base_file_count;
    /// ��ǰ�ļ���Ŀ¼����
    int folder_count;

public:
    string GetNowFolderName() {
        return now_folder_name;
    }
    /**
     * @brief ��ʼ��Ŀ¼
     *
     */
    void Initialize() {
        // ��ʼ���������
        base_file_count = 0;
        base_file_list.items.clear();

        folder_count = 0;
        folders.clear();

        now_folder = 0;
        now_folder_base_id = 0;
        now_folder_name = "root";
        // ����������ʼ��Ŀ¼
        Folder root;
        // �����ļ�Ŀ¼������һ��
        base_file_list.Add(0, 0, true, "root");
        // ��Ŀ¼���·����
        root.Add(".", 0);
        // �ļ���Ŀ¼����һ��
        folders[0] = root;
    }

    /**
     * @brief �ڵ�ǰ·���´���һ���µ��ļ���
     *
     * @param folder_name ���ļ�������
     * @return true �ɹ�����
     * @return false ����ʧ�ܣ�����ͬ���ļ�
     */
    bool CreateFolder(string folder_name) {
        // ����Ƿ�����
        if(folders[now_folder].Find(folder_name) != -1)
            return false;
        // �������ļ���Ŀ¼
        Folder new_folder;
        folder_count++;
        // �����ļ�Ŀ¼������һ��
        base_file_count++;
        base_file_list.Add(base_file_count, folder_count, true, folder_name);

        // Ϊ���ļ��й����������·��Ŀ¼��
        new_folder.Add("..", now_folder_base_id);
        new_folder.Add(".", base_file_count);

        // �ļ���Ŀ¼����һ��
        folders[folder_count] = new_folder;
        // ��ǰ�ļ�������һ��
        folders[now_folder].Add(folder_name, base_file_count);
        return true;
    }

    /**
     * @brief �ڵ�ǰ·���´���һ���µ��ļ�
     *
     * @param file_name ���ļ�����
     * @return true �ɹ�����
     * @return false ����ʧ�ܣ�����ͬ���ļ�
     */
    bool CreateFile(string file_name, int inode_id) {
        // ����Ƿ�����
        if(folders[now_folder].Find(file_name) != -1)
            return false;
        // �����ļ�Ŀ¼������һ��
        base_file_count++;
        base_file_list.Add(base_file_count, inode_id, false, file_name);
        // ��ǰ�ļ�������һ��
        folders[now_folder].Add(file_name, base_file_count);
        return true;
    }
    /**
     * @brief �Եݹ鷽ʽ�����ļ�·�������ļ�(��),֧�����ַ�ʽ:����·��(/)�����·��(./ ../ name)
     *
     * @param path ·��
     * @param current_folder_id ��ǰ�ļ��еĻ����ļ�Ŀ¼�±�
     * @return int Ŀ���ڻ����ļ�Ŀ¼����±꣬�ļ�������ʱ����-1
     */
    int Find(string path, int current_folder_id) {
        // �������ļ�Ŀ¼�±�ת��Ϊ�ļ�Ŀ¼���
        current_folder_id = base_file_list.GetIndex(current_folder_id);
        if(path[0] == '/') {
            // ����·��ģʽ
            // ������"/"ʱ��ת����Ŀ¼
            if(path.length() == 1)
                return 0;
            else
                return Find(path.substr(1), 0);
        }
        else {
            // ���·��ģʽ
            int delimiter_pos = path.find('/');
            // ��ǰ�ļ�������Ҫ�򿪵��ļ���
            string next_folder = (delimiter_pos == -1 ? path : path.substr(0, delimiter_pos));
            // ������Ҫ�򿪵��ļ���
            string next_path = (delimiter_pos == -1 ? "" : path.substr(delimiter_pos + 1));
            // �ڵ�ǰ�ļ����²���
            int next_id = folders[current_folder_id].Find(next_folder);
            // δ�ҵ�����·�������޺����ļ��У���ֹ����
            if(next_id == -1 || next_path.empty())
                return next_id;
            // �����ڶ�Ӧ���ļ����¼���Ѱ��
            else
                return Find(next_path, next_id);
        }
    }

    /**
     * @brief ���ļ���,֧�����ַ�ʽ:����·��(/)�����·��(./ ../ name)
     *
     * @param folder_path �ļ���·��
     * @return true �򿪳ɹ�
     * @return false �������ļ��д�ʧ��
     */
    bool OpenFolder(string folder_path) {
        if(folder_path.empty())
            return false;
        int folder_id = Find(folder_path, now_folder_base_id);
        // ���û�ж�Ӧ·��
        if(folder_id == -1)
            return false;
        // �����ͬ���ļ�
        if(!base_file_list.GetIsFolder(folder_id))
            return false;
        else {
            // ������ת
            now_folder_base_id = folder_id;
            now_folder = base_file_list.GetIndex(folder_id);
            now_folder_name = base_file_list.GetName(folder_id);
            return true;
        }
    }
    /**
     * @brief ��ʾ��ǰ�ļ����������ļ�
     *
     */
    void ShowFolder() {
        cout << endl;
        for(int j = 0; j < folders[now_folder].items.size(); j++) {
            if(folders[now_folder].items[j].name == "." || folders[now_folder].items[j].name == "..")
                continue;
            cout << " ";
            cout << setw(15) << left << folders[now_folder].items[j].name << " ";
            cout << setw(6) << left << (base_file_list.items[folders[now_folder].items[j].index].is_folder ? "�ļ���" : "�ļ�");
            cout << endl;
        }
        cout << endl;
    }
    /**
     * @brief ���������Ϣ
     * ���������ļ�Ŀ¼�����е��ļ���Ŀ¼
     */
    void Debug() {
        cout << "�����ļ�Ŀ¼ ��" << base_file_count << "��: " << endl;
        cout << " ------------------------------------------" << endl;
        for(int i = 0; i < base_file_list.items.size(); i++) {
            cout << " | ";
            cout << setw(4) << left << base_file_list.items[i].id << " | ";
            cout << setw(4) << left << base_file_list.items[i].index << " | ";
            cout << setw(6) << left << (base_file_list.items[i].is_folder ? "folder" : "file") << " | ";
            cout << setw(15) << left << base_file_list.items[i].name;
            cout << " | " << endl;
        }
        cout << " ------------------------------------------" << endl;
        cout << endl;
        cout << "�ļ���Ŀ¼ ��" << folder_count << "����" << endl;
        cout << " --------------------------" << endl;
        for(auto iter: folders) {
            cout << " | �ļ���" << iter.first << ":               |" << endl;
            for(int j = 0; j < iter.second.items.size(); j++) {
                cout << " | ";
                cout << setw(15) << left << iter.second.items[j].name << " | ";
                cout << setw(4) << left << iter.second.items[j].index;
                cout << " | " << endl;
            }

            cout << " --------------------------" << endl;
        }
    }

} MenuManager;
