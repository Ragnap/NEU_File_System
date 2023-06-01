/**
 * @ ��դ�����Ҋ�����Τ��҃��Ǥ���
 * @Author       : RagnaLP
 * @Date         : 2023-05-23 15:05:59
 * @LastEditTime : 2023-06-01 15:21:23
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
    /**
     * @brief �Ƴ������ļ����±��Ӧ��һ���ļ�
     *
     */
    void Delete(int id) {
        items.erase(id);
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
    /**
     * @brief ����ָ��Ļ����ļ����±�ɾ���ļ�
     *
     */
    void Delete(int index) {
        for(int i = 0; i < items.size(); i++)
            if(items[i].index == index) {
                items.erase(items.begin() + i);
            }
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
     * @brief �Եݹ鷽ʽ��·���²���ĳ���ļ�
     *
     * @param file_name �ļ���
     * @param current_folder_id ��ǰ�ļ��еĻ����ļ�Ŀ¼�±�
     * @return pair<int,int> <Ŀ���ڻ����ļ�Ŀ¼����±�,�����ļ������ļ���Ŀ¼����±�>���ļ�������ʱ����<-1,-1>
     */
    pair<int, int> FindFile(string file_name, int current_folder_id) {
        // �������ļ�Ŀ¼�±�ת��Ϊ�ļ�Ŀ¼���
        current_folder_id = base_file_list.GetIndex(current_folder_id);
        for(FolderItem item: folders[current_folder_id].items) {
            // �������Ŀ¼��
            if(item.name == "." || item.name == "..")
                continue;
            // ������ļ������������
            if(base_file_list.GetIsFolder(item.index)) {
                pair<int, int> sub_result = FindFile(file_name, base_file_list.GetIndex(item.index));
                // ����Ѿ��ҵ���ֱ�ӷ���
                if(sub_result.first != -1)
                    return sub_result;
            }
            // ������ļ����ж��Ƿ�ΪĿ���ļ�
            else if(item.name == file_name)
                return make_pair(item.index, current_folder_id);
        }
        return make_pair(-1, -1);
    }

    /**
     * @brief �Եݹ鷽ʽ��ȡָ���ļ���ָ��·���µ������ļ���index���
     *
     * @param current_folder_id ��ǰ�ļ��еĻ����ļ�Ŀ¼�±�
     * @return vector<pair<int,string> > �����ļ���index���������
     */
    vector<pair<int, string>> GetFiles(int current_folder_id) {
        vector<pair<int, string>> result(0);
        // �������ļ�Ŀ¼�±�ת��Ϊ�ļ�Ŀ¼���
        current_folder_id = base_file_list.GetIndex(current_folder_id);
        for(FolderItem item: folders[current_folder_id].items) {
            // �������Ŀ¼��
            if(item.name == "." || item.name == "..")
                continue;
            // ������ļ������������
            if(base_file_list.GetIsFolder(item.index)) {
                vector<pair<int, string>> sub_result = GetFiles(base_file_list.GetIndex(item.index));
                result.insert(result.end(), sub_result.begin(), sub_result.end());
            }
            // ������ļ���ֱ�Ӹ���
            else {
                result.push_back(make_pair(base_file_list.GetIndex(item.index), item.name));
            }
        }
        return result;
    }
    /**
     * @brief �ݹ�ɾ����ǰ�ļ��������еĿ��ļ���(ֻ�����ļ��е��ļ���)��������ǰ�ļ��У���ɾ����Ŀ¼
     *
     * @param current_folder_id ��ǰ�ļ��еĻ����ļ�Ŀ¼�±�
     */
    void DeleteEmptyFolder(int current_folder_id) {
        // �������ļ�Ŀ¼�±�ת��Ϊ�ļ�Ŀ¼���
        current_folder_id = base_file_list.GetIndex(current_folder_id);
        int count = 0;
        vector<FolderItem> origin_folder = folders[current_folder_id].items;
        for(FolderItem item: origin_folder) {
            // �������Ŀ¼��
            if(item.name == "." || item.name == "..")
                continue;
            // ������ļ���������������
            if(base_file_list.GetIsFolder(item.index))
                DeleteEmptyFolder(item.index);
        }
        // ��ǰ�ļ����ѿգ������Ƴ�
        if(current_folder_id != 0 && folders[current_folder_id].items.size() == 2) {
            // �����ļ�Ŀ¼ɾ��һ��
            base_file_list.Delete(folders[current_folder_id].items[1].index);
            // ���ļ��м���һ��
            int upper_folder_id = folders[current_folder_id].items[0].index;
            folders[upper_folder_id].Delete(folders[current_folder_id].items[1].index);
            folders.erase(current_folder_id);
            // ��ɾ���˵�ǰ�򿪵�Ŀ¼������Ҫ������һ��Ŀ¼
            if(current_folder_id == now_folder_base_id) {
                now_folder_base_id = upper_folder_id;
                now_folder = base_file_list.GetIndex(now_folder_base_id);
                now_folder_name = base_file_list.GetName(now_folder_base_id);
            }
        }
    }

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
            // �ڸ�Ŀ¼��
            if(upper_folder_path.empty()) {
                folder_id = 0;
                folder_name = folder_path.substr(delimiter_pos + 1);
            }
            else {
                folder_id = base_file_list.GetIndex(Find(upper_folder_path));
                folder_name = folder_path.substr(delimiter_pos + 1);
            }
        }

        // �������ļ���Ŀ¼
        Folder new_folder;

        // �����ļ�Ŀ¼������һ��
        base_file_list.Add(base_file_count, folder_count, true, folder_name);

        // Ϊ���ļ��й����������·��Ŀ¼��
        new_folder.Add("..", folder_id);
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
     * @brief ��ȡָ���ļ���ָ��·���µ������ļ���index���
     *
     * @param file_path ָ��·��
     * @return vector<int> �����ļ���index���,����ʱ����һ��Ԫ�ص�vector (������,"")  (-1:�ļ������� -2:·������ -3:���ļ� -4:���ļ�)
     */
    vector<pair<int, string>> GetFiles(string file_path) {
        vector<pair<int, string>> result(0);
        // ���·��
        int id = Find(file_path);
        // ·������򲻴���
        if(id < 0) {
            result.push_back(make_pair(id, ""));
            return result;
        }
        // ������ļ���
        if(base_file_list.GetIsFolder(id)) {
            result = GetFiles(id);
            if(result.size() == 0)
                result.push_back(make_pair(-3, ""));
        }
        // ���ļ�
        else {
            result.push_back(make_pair(-4, ""));
            result.push_back(make_pair(base_file_list.GetIndex(id), base_file_list.GetName(id)));
        }
        return result;
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
            // �ڸ�Ŀ¼��
            if(upper_folder_path.empty()) {
                folder_id = 0;
                file_name = file_path.substr(delimiter_pos + 1);
            }
            else {
                folder_id = base_file_list.GetIndex(Find(upper_folder_path));
                file_name = file_path.substr(delimiter_pos + 1);
            }
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
     * @brief �ڵ�ǰ�ļ����¸����ļ�·����������·�����ļ�(��),֧�����ַ�ʽ:����·��(/)�����·��(./ ../ name)
     *
     * @param path ·��
     * @return int Ŀ���ڻ����ļ�Ŀ¼����±꣬�ļ�������ʱ����-1��·������ʱ����-2
     */
    int Find(string path) {
        return Find(path, now_folder_base_id);
    }

    /**
     * @brief ɾ����ָ���ļ����µ�һ�����ļ�
     *
     * @param file_name ���ļ���
     * @param path ָ��Ŀ¼
     * @return int 0:����ɾ�� -1:�ļ������� -2:·������
     */
    int DeleteFile(string file_name, string path = "./") {
        // ��λ�ļ���
        int id = Find(path);
        if(id < 0)
            return id;
        pair<int, int> pos;
        // ��·������һ���ļ���,�����е����ļ����в���
        if(base_file_list.GetIsFolder(id)) {
            pos = FindFile(file_name, id);
        }
        // ��·��ֱ�Ӿ���һ���ļ���ֻ��Ҫ��λ�����ļ���
        else {
            int folder_id, delimiter_pos = path.find_last_of('/');
            // ���Ǹ����ļ���������·��
            if(delimiter_pos == -1)
                folder_id = now_folder_base_id;
            // ����·���ϵĸ��ļ���
            else
                folder_id = Find(path.substr(0, delimiter_pos));
            pos = FindFile(file_name, folder_id);
        }

        // ɾ�������ļ����е��ļ�
        base_file_list.Delete(pos.first);
        // ɾ����Ӧ�ļ����µ��ļ���
        folders[pos.second].Delete(pos.first);
        return 0;
    }

    /**
     * @brief �������ļ�(��)
     *
     * @param path �ļ�·��
     * @param new_name ������
     * @return int 0:�����޸� -1:�ļ������� -2:·������
     */
    int RenameFile(string path, string new_name) {
        // ��λ�ļ���
        int id = Find(path);
        if(id < 0)
            return id;
        // ��λ���ļ���
        int folder_id, delimiter_pos = path.find_last_of('/');
        // ���Ǹ����ļ���������·��
        if(delimiter_pos == -1) {
            folder_id = folders[base_file_list.GetIndex(now_folder_base_id)].Find("..");
            // ��Ŀ¼û�и��ļ���
            folder_id = (folder_id == -1) ? 0 : folder_id;
        }
        // ����·���ϵĸ��ļ���
        else
            folder_id = Find(path.substr(0, delimiter_pos));

        // �޸��ļ����е�����
        for(int i = 0; i < folders[folder_id].items.size(); i++) {
            if(folders[folder_id].items[i].index == id) {
                folders[folder_id].items[i].name = new_name;
                break;
            }
        }
        // �޸Ļ����ļ����е�����
        base_file_list.items[id].name = new_name;
        return 0;
    }
    /**
     * @brief �ƶ��ļ���
     *
     * @param origin_path Դ�ļ�(��)·��
     * @param dest_path Ŀ���ļ���·��
     * @return int -4:Ŀ��·�������ļ��� -3:Ŀ��·������ -2:Դ·������ -1:Դ�ļ�������
     */
    int MoveFile(string origin_path, string dest_path) {
        // ��λԴ�ļ�
        int ori_id = Find(origin_path);
        if(ori_id < 0)
            return ori_id;
        // ��λĿ���ļ���
        int dest_id = Find(dest_path);
        if(dest_id < 0)
            return -3;
        // ��Ŀ�겻���ļ���
        if(!base_file_list.GetIsFolder(dest_id))
            return -4;
        // ��λԴ�ļ������ļ���
        int ori_folder_id, delimiter_pos = origin_path.find_last_of('/');
        // ���Ǹ����ļ���������·��
        if(delimiter_pos == -1) {
            ori_folder_id = folders[base_file_list.GetIndex(now_folder_base_id)].Find("..");
            // ��Ŀ¼û�и��ļ���
            ori_folder_id = (ori_folder_id == -1) ? 0 : ori_folder_id;
        }
        // ����·���ϵĸ��ļ���
        else
            ori_folder_id = Find(origin_path.substr(0, delimiter_pos));

        // Դ���ļ����Ƴ�
        folders[ori_folder_id].Delete(ori_id);
        // Ŀ���ļ�������
        folders[dest_id].Add(base_file_list.GetName(ori_id), ori_id);
    }
    /**
     * @brief �ݹ�ɾ��ָ���ļ��������еĿ��ļ���(ֻ�����ļ��е��ļ���)��������ǰ�ļ��У���ɾ����Ŀ¼
     *
     * @param path ָ��·��
     * @return int 0:����ɾ�� -1:�ļ������� -2:·������
     */
    int DeleteEmptyFolder(string path) {
        // ��λ�ļ���
        int index = Find(path);
        if(index < 0)
            return index;
        // ɾ��
        DeleteEmptyFolder(index);
        return 0;
    }
    /**
     * @brief ��ȡ�ļ���Ӧ��Ӳ��inode�±�
     *
     * @param file_path �ļ�·��
     * @return int Ӳ��inode�±�λ�ã��ļ�������ʱ����-1��·������ʱ����-2
     */
    int GetInodeID(string file_path) {
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
            if(folders[now_folder].items[j].name == "." || folders[now_folder].items[j].name == ".." || folders[now_folder].items[j].name[0] == '~')
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
     * @brief ��Ŀ¼�ļ�������ת���ɵ����ַ���
     *
     */
    string Save() {
        ostringstream data;
        // ������
        data << base_file_count << "\n";
        for(auto item: base_file_list.items) {
            data << item.first << " " << item.second.index << " " << item.second.is_folder << " " << item.second.name << "\n";
        }
        // �ļ��б�
        data << folders.size() << "\n";
        for(auto folder: folders) {
            data << folder.first << " " << folder.second.items.size() << "\n";
            for(auto item: folder.second.items) {
                data << item.name << " " << item.index << "\n";
            }
        }
        return data.str();
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
     * @brief ���ַ�����ʽ�����ݻָ�����
     *
     */
    void Load(string data) {
        istringstream in(data);
        int id, index, is_folder, siz;
        char name[50];
        // ������
        in >> base_file_count;
        for(int i = 0; i < base_file_count; i++) {
            in >> id >> index >> is_folder >> name;
            base_file_list.Add(id, index, is_folder, string(name));
        }
        // �ļ��б�
        in >> folder_count;
        for(int i = 0; i < folder_count; i++) {
            Folder new_folder;
            in >> id >> siz;
            for(int j = 0; j < siz; j++) {
                in >> name >> index;
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
        cout << "�����ļ�Ŀ¼ ��" << base_file_list.items.size() << "��: " << endl;
        cout << " ------------------------------------------" << endl;
        for(auto i: base_file_list.items) {
            cout << " | ";
            cout << setw(4) << left << i.second.id << " | ";
            cout << setw(4) << left << i.second.index << " | ";
            cout << setw(6) << left << (i.second.is_folder ? "folder" : "file") << " | ";
            cout << setw(15) << left << i.second.name;
            cout << " | " << endl;
        }
        cout << " ------------------------------------------" << endl;
        cout << endl;
        cout << "�ļ���Ŀ¼ ��" << folders.size() << "����" << endl;
        cout << " --------------------------" << endl;
        for(auto iter: folders) {
            cout << " | �ļ��� " << setw(15) << base_file_list.GetName(folders[iter.first].Find(".")) << " |" << endl;
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
