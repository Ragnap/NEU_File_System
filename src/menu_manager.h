/**
 * @ 青空だけが見たいのは我儘ですか
 * @Author       : RagnaLP
 * @Date         : 2023-05-23 15:05:59
 * @LastEditTime : 2023-05-25 09:08:52
 * @Description  : 目录处理相关类
 */

#include "config.h"

using namespace std;
/**
 * @brief 基本文件目录项类
 *
 */
typedef struct BaseFileListItem {
    // 基本文件目录ID
    int id;
    /// 文件(目录)地址,为文件夹时指向文件目录下标,为文件时指向对应的index_node
    int index;
    /// 标识该文件是否为文件
    bool is_folder;
    /// 该文件或者文件夹的名称
    string name;
} BaseFileListItem;

/**
 * @brief 基本文件目录类
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
     * @brief 在基本文件目录中创建一个新的项
     *
     * @param id 基本文件ID
     * @param index 文件(目录)地址,为文件夹时指向文件目录下标,为文件时指向对应的index_node
     * @param is_folder 是否为文件夹
     * @param name 名称
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
 * @brief 文件夹项类
 *
 */
typedef struct FolderItem {
    /// 文件(夹)名
    string name;
    /// 基本文件目录下标
    int index;
} FolderItem;

/**
 * @brief 单个文件目录类
 * 文件夹中的文件目录
 */
typedef struct Folder {
    /// 文件夹中的文件目录
    vector<FolderItem> items;

    /**
     * @brief 增加在当前文件夹下增加一项
     *
     * @param name 名称
     * @param index 基本文件目录下标
     * @return true 创建成功
     * @return false 存在同名文件
     */
    bool Add(string name, int index) {
        // 检查是否存在同名文件
        for(int i = 0; i < items.size(); i++)
            if(items[i].name == name)
                return false;
        // 创建
        FolderItem new_item;
        new_item.name = name;
        new_item.index = index;
        items.push_back(new_item);
        return true;
    }

    /**
     * @brief 查找在当前文件夹下是否存在某一路径，有则返回其在基本文件目录中的下标
     *
     * @param name 查找对象名
     * @return int 基本文件目录中的下标，未找到则返回-1
     */
    int Find(string name) {
        for(int i = 0; i < items.size(); i++)
            if(items[i].name == name)
                return items[i].index;
        return -1;
    }
} Folder;

/**
 * @brief 总文件目录系统类
 *
 */
typedef class MenuManager {
private:
    /// 基本文件目录表
    BaseFileList base_file_list;
    /// 文件夹目录表
    map<int, Folder> folders;
    /// 当前路径
    string now_folder_name;
    /// 当前路径对应的文件夹目录下标
    int now_folder;
    /// 当前路径对应的基本文件目录下标
    int now_folder_base_id;
    /// 当前基本文件编号记数
    int base_file_count;
    /// 当前文件夹目录记数
    int folder_count;

public:
    string GetNowFolderName() {
        return now_folder_name;
    }
    /**
     * @brief 初始化目录
     *
     */
    void Initialize() {
        // 初始化相关数据
        base_file_count = 0;
        base_file_list.items.clear();

        folder_count = 0;
        folders.clear();

        now_folder = 0;
        now_folder_base_id = 0;
        now_folder_name = "root";
        // 创建创建初始根目录
        Folder root;
        // 基本文件目录中新增一项
        base_file_list.Add(0, 0, true, "root");
        // 根目录相对路径项
        root.Add(".", 0);
        // 文件夹目录新增一项
        folders[0] = root;
    }

    /**
     * @brief 在当前路径下创建一个新的文件夹
     *
     * @param folder_name 新文件夹名称
     * @return true 成功创建
     * @return false 创建失败，存在同名文件
     */
    bool CreateFolder(string folder_name) {
        // 检查是否重名
        if(folders[now_folder].Find(folder_name) != -1)
            return false;
        // 创建新文件夹目录
        Folder new_folder;
        folder_count++;
        // 基本文件目录中新增一项
        base_file_count++;
        base_file_list.Add(base_file_count, folder_count, true, folder_name);

        // 为新文件夹关联增加相对路径目录项
        new_folder.Add("..", now_folder_base_id);
        new_folder.Add(".", base_file_count);

        // 文件夹目录新增一项
        folders[folder_count] = new_folder;
        // 当前文件夹新增一项
        folders[now_folder].Add(folder_name, base_file_count);
        return true;
    }

    /**
     * @brief 在当前路径下创建一个新的文件
     *
     * @param file_name 新文件名称
     * @return true 成功创建
     * @return false 创建失败，存在同名文件
     */
    bool CreateFile(string file_name, int inode_id) {
        // 检查是否重名
        if(folders[now_folder].Find(file_name) != -1)
            return false;
        // 基本文件目录中新增一项
        base_file_count++;
        base_file_list.Add(base_file_count, inode_id, false, file_name);
        // 当前文件夹新增一项
        folders[now_folder].Add(file_name, base_file_count);
        return true;
    }
    /**
     * @brief 以递归方式根据文件路径查找文件(夹),支持两种方式:绝对路径(/)与相对路径(./ ../ name)
     *
     * @param path 路径
     * @param current_folder_id 当前文件夹的基本文件目录下标
     * @return int 目标在基本文件目录表的下标，文件不存在时返回-1
     */
    int Find(string path, int current_folder_id) {
        // 将基本文件目录下标转换为文件目录编号
        current_folder_id = base_file_list.GetIndex(current_folder_id);
        if(path[0] == '/') {
            // 绝对路径模式
            // 单输入"/"时跳转到根目录
            if(path.length() == 1)
                return 0;
            else
                return Find(path.substr(1), 0);
        }
        else {
            // 相对路径模式
            int delimiter_pos = path.find('/');
            // 当前文件夹下需要打开的文件夹
            string next_folder = (delimiter_pos == -1 ? path : path.substr(0, delimiter_pos));
            // 后续需要打开的文件夹
            string next_path = (delimiter_pos == -1 ? "" : path.substr(delimiter_pos + 1));
            // 在当前文件夹下查找
            int next_id = folders[current_folder_id].Find(next_folder);
            // 未找到后续路径或者无后续文件夹，终止搜索
            if(next_id == -1 || next_path.empty())
                return next_id;
            // 否则在对应的文件夹下继续寻找
            else
                return Find(next_path, next_id);
        }
    }

    /**
     * @brief 打开文件夹,支持两种方式:绝对路径(/)与相对路径(./ ../ name)
     *
     * @param folder_path 文件夹路径
     * @return true 打开成功
     * @return false 不存在文件夹打开失败
     */
    bool OpenFolder(string folder_path) {
        if(folder_path.empty())
            return false;
        int folder_id = Find(folder_path, now_folder_base_id);
        // 如果没有对应路径
        if(folder_id == -1)
            return false;
        // 如果是同名文件
        if(!base_file_list.GetIsFolder(folder_id))
            return false;
        else {
            // 进行跳转
            now_folder_base_id = folder_id;
            now_folder = base_file_list.GetIndex(folder_id);
            now_folder_name = base_file_list.GetName(folder_id);
            return true;
        }
    }
    /**
     * @brief 显示当前文件夹下所有文件
     *
     */
    void ShowFolder() {
        cout << endl;
        for(int j = 0; j < folders[now_folder].items.size(); j++) {
            if(folders[now_folder].items[j].name == "." || folders[now_folder].items[j].name == "..")
                continue;
            cout << " ";
            cout << setw(15) << left << folders[now_folder].items[j].name << " ";
            cout << setw(6) << left << (base_file_list.items[folders[now_folder].items[j].index].is_folder ? "文件夹" : "文件");
            cout << endl;
        }
        cout << endl;
    }
    /**
     * @brief 输出调试信息
     * 包含基本文件目录和所有的文件夹目录
     */
    void Debug() {
        cout << "基本文件目录 共" << base_file_count << "个: " << endl;
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
        cout << "文件夹目录 共" << folder_count << "个：" << endl;
        cout << " --------------------------" << endl;
        for(auto iter: folders) {
            cout << " | 文件夹" << iter.first << ":               |" << endl;
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
