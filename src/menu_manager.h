/**
 * @ ��դ�����Ҋ�����Τ��҃��Ǥ���
 * @Author       : RagnaLP
 * @Date         : 2023-05-23 15:05:59
 * @LastEditTime : 2023-05-29 15:59:53
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
    /**
     * @brief �������
     *
     */
    void Clear() {
        base_file_count = 0;
        base_file_list.items.clear();

        folder_count = 0;
        folders.clear();

        now_folder = 0;
        now_folder_base_id = 0;
        now_folder_name = "root";
    }
    /**
     * @brief ��ʼ��Ŀ¼
     *
     */
    void Initialize() {
        // ��ʼ���������
        Clear();
        // ����������ʼ��Ŀ¼
        Folder root;
        // �����ļ�Ŀ¼������һ��
        base_file_list.Add(0, 0, true, "root");
        // ��Ŀ¼���·����
        root.Add(".", 0);
        // �ļ���Ŀ¼����һ��
        folders[0] = root;
        // ���¼���
        base_file_count++;
        folder_count++;
    }

    /**
     * @brief �ڵ�ǰ·���´���һ���µ��ļ���
     *
     * @param folder_path ���ļ���·��
     * @return int ��������ʱ����0���Ѵ���ͬ���ļ�ʱ����-1��·������ʱ����-2
     */
    int CreateFolder(string folder_path) {
        // ���·��
        int check = Find(folder_path);
        // ·������
        if(check == -2)
            return -2;
        // �Ѵ���ͬ���ļ�
        if(check != -1)
            return -1;

        string folder_name;
        int folder_id = now_folder;

        // ȷ���ļ���λ��

        int delimiter_pos = folder_path.find_last_of('/');
        // ���Ǹ����ļ���������·��
        if(delimiter_pos == -1) {
            folder_name = folder_path;
        }
        else {
            // ���Ҷ�Ӧ���ļ���
            string upper_folder_path = folder_path.substr(0, delimiter_pos);
            folder_id = Find(upper_folder_path);
            folder_name = folder_path.substr(delimiter_pos + 1);
        }

        // �������ļ���Ŀ¼
        Folder new_folder;

        // �����ļ�Ŀ¼������һ��
        base_file_list.Add(base_file_count, folder_count, true, folder_name);

        // Ϊ���ļ��й����������·��Ŀ¼��
        new_folder.Add("..", now_folder_base_id);
        new_folder.Add(".", base_file_count);

        // �ļ���Ŀ¼����һ��
        folders[folder_count] = new_folder;
        // ��ǰ�ļ�������һ��
        folders[folder_id].Add(folder_name, base_file_count);

        // ���¼���
        base_file_count++;
        folder_count++;

        return true;
    }

    /**
     * @brief �ڵ�ǰ·���´���һ���µ��ļ�
     *
     * @param file_path ���ļ�������·��
     * @return int ��������ʱ����0���Ѵ���ͬ���ļ�ʱ����-1��·������ʱ����-2
     */
    int CreateFile(string file_path, int inode_id) {
        // ���·��
        int check = Find(file_path);
        // ·������
        if(check == -2)
            return -2;
        // �Ѵ���ͬ���ļ�
        if(check != -1)
            return -1;

        string file_name;
        int folder_id = now_folder;

        // ȷ���ļ���λ��

        int delimiter_pos = file_path.find_last_of('/');
        // ���Ǹ����ļ���������·��
        if(delimiter_pos == -1) {
            file_name = file_path;
        }
        else {
            // ���Ҷ�Ӧ���ļ���
            string upper_folder_path = file_path.substr(0, delimiter_pos);
            folder_id = Find(upper_folder_path);
            file_name = file_path.substr(delimiter_pos + 1);
        }

        // �����ļ�Ŀ¼������һ��
        base_file_list.Add(base_file_count, inode_id, false, file_name);
        // ��Ӧ���ļ�������һ��
        folders[folder_id].Add(file_name, base_file_count);

        // ���¼���
        base_file_count++;
        return 0;
    }
    /**
     * @brief �Եݹ鷽ʽ�����ļ�·�������ļ�(��),֧�����ַ�ʽ:����·��(/)�����·��(./ ../ name)
     *
     * @param path ·��
     * @param current_folder_id ��ǰ�ļ��еĻ����ļ�Ŀ¼�±�
     * @return int Ŀ���ڻ����ļ�Ŀ¼����±꣬�ļ�������ʱ����-1��·������ʱ����-2
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
            if(next_id == -1) {
                // ·������
                if(path.find('/') != string::npos)
                    return -2;
                // δ�ҵ���Ӧ�ļ�
                else
                    return -1;
            }
            // �޺����ļ�����ɲ���
            if(next_path.empty())
                return next_id;
            // �����ڶ�Ӧ���ļ����¼���Ѱ��
            else
                return Find(next_path, next_id);
        }
    }

    /**
     * @brief �ڵ�ǰ�ļ����¸����ļ�·�������ļ�(��),֧�����ַ�ʽ:����·��(/)�����·��(./ ../ name)
     *
     * @param path ·��
     * @return int Ŀ���ڻ����ļ�Ŀ¼����±꣬�ļ�������ʱ����-1��·������ʱ����-2
     */
    int Find(string path) {
        return Find(path, now_folder_base_id);
    }
    /**
     * @brief ��ȡ�ļ��ĵ�һ�����ݿ�λ��
     *
     * @param file_path �ļ���
     * @return int ��һ�����ݿ�λ�ã��ļ�������ʱ����-1��·������ʱ����-2
     */
    int GetAddress(string file_path) {
        int index = Find(file_path);
        if(index < 0)
            return index;
        return base_file_list.GetIndex(index);
    }
    /**
     * @brief ���ļ���,֧�����ַ�ʽ:����·��(/)�����·��(./ ../ name)
     *
     * @param folder_path �ļ���·��
     * @return true �򿪳ɹ�
     * @return false ·�������ʧ��
     */
    bool OpenFolder(string folder_path) {
        if(folder_path.empty())
            return false;
        int folder_id = Find(folder_path, now_folder_base_id);
        // ���û�ж�Ӧ·��
        if(folder_id < 0)
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
     * @brief ��ȡ��ǰ�����ļ�������
     *
     * @return string �ļ�������
     */
    string GetNowFolderName() {
        return now_folder_name;
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
     * @brief ����Ŀ¼����
     * ������������
     * ��������
     * �ļ���Ŀ¼����
     * ��ǰ�ļ��б��
     * ��ǰ�ļ���Ŀ¼����
     * ��ǰ�ļ���Ŀ¼��
     */
    void Save(FILE* file) {
        // ������
        fprintf(file, "%d\n", base_file_count);
        for(auto item: base_file_list.items) {
            fprintf(file, "%d %d %d %s\n", item.first, item.second.index, item.second.is_folder, item.second.name.c_str());
        }
        // �ļ��б�
        fprintf(file, "%d\n", folders.size());
        for(auto folder: folders) {
            fprintf(file, "%d %d\n", folder.first, folder.second.items.size());
            for(auto item: folder.second.items) {
                fprintf(file, "%s %d\n", item.name.c_str(), item.index);
            }
        }
    }
    /**
     * @brief ���ļ��ָ�Ŀ¼����
     *
     */
    void Load(FILE* file) {
        int id, index, is_folder, siz;
        char name[50];
        // ������
        fscanf(file, "%d", &base_file_count);
        for(int i = 0; i < base_file_count; i++) {
            fscanf(file, "%d %d %d %s", &id, &index, &is_folder, name);
            base_file_list.Add(id, index, is_folder, string(name));
        }
        // �ļ��б�
        fscanf(file, "%d", &folder_count);
        for(int i = 0; i < folder_count; i++) {
            Folder new_folder;
            fscanf(file, "%d %d", &id, &siz);
            for(int j = 0; j < siz; j++) {
                fscanf(file, "%s %d", name, &index);
                new_folder.Add(string(name), index);
            }
            folders[id] = new_folder;
        }
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
