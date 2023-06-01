/**
 * @ 青空だけが見たいのは我儘ですか
 * @Author       : RagnaLP
 * @Date         : 2023-05-23 15:05:59
 * @LastEditTime : 2023-06-01 15:21:23
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
    /**
     * @brief 移除基本文件表下标对应的一个文件
     *
     */
    void Delete(int id) {
        items.erase(id);
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
    /**
     * @brief 根据指向的基本文件表下标删除文件
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

    /**
     * @brief 以递归方式根据文件路径查找文件(夹),支持两种方式:绝对路径(/)与相对路径(./ ../ name)
     *
     * @param path 路径
     * @param current_folder_id 当前文件夹的基本文件目录下标
     * @return int 目标在基本文件目录表的下标，文件不存在时返回-1，路径错误时返回-2
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
            if(next_id == -1) {
                // 路径错误
                if(path.find('/') != string::npos)
                    return -2;
                // 未找到对应文件
                else
                    return -1;
            }
            // 无后续文件夹完成查找
            if(next_path.empty())
                return next_id;
            // 否则在对应的文件夹下继续寻找
            else
                return Find(next_path, next_id);
        }
    }

    /**
     * @brief 以递归方式在路径下查找某个文件
     *
     * @param file_name 文件名
     * @param current_folder_id 当前文件夹的基本文件目录下标
     * @return pair<int,int> <目标在基本文件目录表的下标,所在文件夹在文件夹目录里的下标>，文件不存在时返回<-1,-1>
     */
    pair<int, int> FindFile(string file_name, int current_folder_id) {
        // 将基本文件目录下标转换为文件目录编号
        current_folder_id = base_file_list.GetIndex(current_folder_id);
        for(FolderItem item: folders[current_folder_id].items) {
            // 跳过相对目录项
            if(item.name == "." || item.name == "..")
                continue;
            // 如果是文件夹则继续遍历
            if(base_file_list.GetIsFolder(item.index)) {
                pair<int, int> sub_result = FindFile(file_name, base_file_list.GetIndex(item.index));
                // 如果已经找到则直接返回
                if(sub_result.first != -1)
                    return sub_result;
            }
            // 如果是文件则判断是否为目标文件
            else if(item.name == file_name)
                return make_pair(item.index, current_folder_id);
        }
        return make_pair(-1, -1);
    }

    /**
     * @brief 以递归方式获取指定文件或指定路径下的所有文件的index编号
     *
     * @param current_folder_id 当前文件夹的基本文件目录下标
     * @return vector<pair<int,string> > 所有文件的index编号与名称
     */
    vector<pair<int, string>> GetFiles(int current_folder_id) {
        vector<pair<int, string>> result(0);
        // 将基本文件目录下标转换为文件目录编号
        current_folder_id = base_file_list.GetIndex(current_folder_id);
        for(FolderItem item: folders[current_folder_id].items) {
            // 跳过相对目录项
            if(item.name == "." || item.name == "..")
                continue;
            // 如果是文件夹则继续遍历
            if(base_file_list.GetIsFolder(item.index)) {
                vector<pair<int, string>> sub_result = GetFiles(base_file_list.GetIndex(item.index));
                result.insert(result.end(), sub_result.begin(), sub_result.end());
            }
            // 如果是文件则直接附加
            else {
                result.push_back(make_pair(base_file_list.GetIndex(item.index), item.name));
            }
        }
        return result;
    }
    /**
     * @brief 递归删除当前文件夹下所有的空文件夹(只包含文件夹的文件夹)，包括当前文件夹，不删除根目录
     *
     * @param current_folder_id 当前文件夹的基本文件目录下标
     */
    void DeleteEmptyFolder(int current_folder_id) {
        // 将基本文件目录下标转换为文件目录编号
        current_folder_id = base_file_list.GetIndex(current_folder_id);
        int count = 0;
        vector<FolderItem> origin_folder = folders[current_folder_id].items;
        for(FolderItem item: origin_folder) {
            // 跳过相对目录项
            if(item.name == "." || item.name == "..")
                continue;
            // 如果是文件夹则继续遍历检查
            if(base_file_list.GetIsFolder(item.index))
                DeleteEmptyFolder(item.index);
        }
        // 当前文件夹已空，进行移除
        if(current_folder_id != 0 && folders[current_folder_id].items.size() == 2) {
            // 基本文件目录删除一项
            base_file_list.Delete(folders[current_folder_id].items[1].index);
            // 父文件夹减少一项
            int upper_folder_id = folders[current_folder_id].items[0].index;
            folders[upper_folder_id].Delete(folders[current_folder_id].items[1].index);
            folders.erase(current_folder_id);
            // 若删除了当前打开的目录，则需要返回上一级目录
            if(current_folder_id == now_folder_base_id) {
                now_folder_base_id = upper_folder_id;
                now_folder = base_file_list.GetIndex(now_folder_base_id);
                now_folder_name = base_file_list.GetName(now_folder_base_id);
            }
        }
    }

public:
    /**
     * @brief 清空数据
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
     * @brief 初始化目录
     *
     */
    void Initialize() {
        // 初始化相关数据
        Clear();
        // 创建创建初始根目录
        Folder root;
        // 基本文件目录中新增一项
        base_file_list.Add(0, 0, true, "root");
        // 根目录相对路径项
        root.Add(".", 0);
        // 文件夹目录新增一项
        folders[0] = root;
        // 更新记数
        base_file_count++;
        folder_count++;
    }

    /**
     * @brief 在当前路径下创建一个新的文件夹
     *
     * @param folder_path 新文件夹路径
     * @return int 正常创建时返回0，已存在同名文件时返回-1，路径错误时返回-2
     */
    int CreateFolder(string folder_path) {
        // 检查路径
        int check = Find(folder_path);
        // 路径错误
        if(check == -2)
            return -2;
        // 已存在同名文件
        if(check != -1)
            return -1;
        string folder_name;
        int folder_id = now_folder;

        // 确定文件夹位置

        int delimiter_pos = folder_path.find_last_of('/');
        // 就是个纯文件，不包含路径
        if(delimiter_pos == -1) {
            folder_name = folder_path;
        }
        else {
            // 查找对应的文件夹
            string upper_folder_path = folder_path.substr(0, delimiter_pos);
            // 在根目录下
            if(upper_folder_path.empty()) {
                folder_id = 0;
                folder_name = folder_path.substr(delimiter_pos + 1);
            }
            else {
                folder_id = base_file_list.GetIndex(Find(upper_folder_path));
                folder_name = folder_path.substr(delimiter_pos + 1);
            }
        }

        // 创建新文件夹目录
        Folder new_folder;

        // 基本文件目录中新增一项
        base_file_list.Add(base_file_count, folder_count, true, folder_name);

        // 为新文件夹关联增加相对路径目录项
        new_folder.Add("..", folder_id);
        new_folder.Add(".", base_file_count);

        // 文件夹目录新增一项
        folders[folder_count] = new_folder;
        // 当前文件夹新增一项
        folders[folder_id].Add(folder_name, base_file_count);

        // 更新记数
        base_file_count++;
        folder_count++;

        return true;
    }

    /**
     * @brief 获取指定文件或指定路径下的所有文件的index编号
     *
     * @param file_path 指定路径
     * @return vector<int> 所有文件的index编号,错误时返回一个元素的vector (错误编号,"")  (-1:文件不存在 -2:路径错误 -3:无文件 -4:单文件)
     */
    vector<pair<int, string>> GetFiles(string file_path) {
        vector<pair<int, string>> result(0);
        // 检查路径
        int id = Find(file_path);
        // 路径错误或不存在
        if(id < 0) {
            result.push_back(make_pair(id, ""));
            return result;
        }
        // 如果是文件夹
        if(base_file_list.GetIsFolder(id)) {
            result = GetFiles(id);
            if(result.size() == 0)
                result.push_back(make_pair(-3, ""));
        }
        // 单文件
        else {
            result.push_back(make_pair(-4, ""));
            result.push_back(make_pair(base_file_list.GetIndex(id), base_file_list.GetName(id)));
        }
        return result;
    }

    /**
     * @brief 在当前路径下创建一个新的文件
     *
     * @param file_path 新文件名称与路径
     * @return int 正常创建时返回0，已存在同名文件时返回-1，路径错误时返回-2
     */
    int CreateFile(string file_path, int inode_id) {
        // 检查路径
        int check = Find(file_path);
        // 路径错误
        if(check == -2)
            return -2;
        // 已存在同名文件
        if(check != -1)
            return -1;

        string file_name;
        int folder_id = now_folder;

        // 确定文件夹位置

        int delimiter_pos = file_path.find_last_of('/');
        // 就是个纯文件，不包含路径
        if(delimiter_pos == -1) {
            file_name = file_path;
        }
        else {
            // 查找对应的文件夹
            string upper_folder_path = file_path.substr(0, delimiter_pos);
            // 在根目录下
            if(upper_folder_path.empty()) {
                folder_id = 0;
                file_name = file_path.substr(delimiter_pos + 1);
            }
            else {
                folder_id = base_file_list.GetIndex(Find(upper_folder_path));
                file_name = file_path.substr(delimiter_pos + 1);
            }
        }

        // 基本文件目录中新增一项
        base_file_list.Add(base_file_count, inode_id, false, file_name);
        // 对应的文件夹新增一项
        folders[folder_id].Add(file_name, base_file_count);

        // 更新记数
        base_file_count++;
        return 0;
    }

    /**
     * @brief 在当前文件夹下根据文件路径查找完整路径的文件(夹),支持两种方式:绝对路径(/)与相对路径(./ ../ name)
     *
     * @param path 路径
     * @return int 目标在基本文件目录表的下标，文件不存在时返回-1，路径错误时返回-2
     */
    int Find(string path) {
        return Find(path, now_folder_base_id);
    }

    /**
     * @brief 删除在指定文件夹下的一个子文件
     *
     * @param file_name 子文件名
     * @param path 指定目录
     * @return int 0:正常删除 -1:文件不存在 -2:路径错误
     */
    int DeleteFile(string file_name, string path = "./") {
        // 定位文件夹
        int id = Find(path);
        if(id < 0)
            return id;
        pair<int, int> pos;
        // 该路径就是一个文件夹,在所有的子文件夹中查找
        if(base_file_list.GetIsFolder(id)) {
            pos = FindFile(file_name, id);
        }
        // 该路径直接就是一个文件，只需要定位所在文件夹
        else {
            int folder_id, delimiter_pos = path.find_last_of('/');
            // 就是个纯文件，不包含路径
            if(delimiter_pos == -1)
                folder_id = now_folder_base_id;
            // 查找路径上的父文件夹
            else
                folder_id = Find(path.substr(0, delimiter_pos));
            pos = FindFile(file_name, folder_id);
        }

        // 删除基本文件表中的文件
        base_file_list.Delete(pos.first);
        // 删除对应文件夹下的文件项
        folders[pos.second].Delete(pos.first);
        return 0;
    }

    /**
     * @brief 重命名文件(夹)
     *
     * @param path 文件路径
     * @param new_name 新名称
     * @return int 0:正常修改 -1:文件不存在 -2:路径错误
     */
    int RenameFile(string path, string new_name) {
        // 定位文件夹
        int id = Find(path);
        if(id < 0)
            return id;
        // 定位父文件夹
        int folder_id, delimiter_pos = path.find_last_of('/');
        // 就是个纯文件，不包含路径
        if(delimiter_pos == -1) {
            folder_id = folders[base_file_list.GetIndex(now_folder_base_id)].Find("..");
            // 根目录没有父文件夹
            folder_id = (folder_id == -1) ? 0 : folder_id;
        }
        // 查找路径上的父文件夹
        else
            folder_id = Find(path.substr(0, delimiter_pos));

        // 修改文件夹中的名称
        for(int i = 0; i < folders[folder_id].items.size(); i++) {
            if(folders[folder_id].items[i].index == id) {
                folders[folder_id].items[i].name = new_name;
                break;
            }
        }
        // 修改基本文件表中的名称
        base_file_list.items[id].name = new_name;
        return 0;
    }
    /**
     * @brief 移动文件夹
     *
     * @param origin_path 源文件(夹)路径
     * @param dest_path 目标文件夹路径
     * @return int -4:目标路径不是文件夹 -3:目标路径错误 -2:源路径错误 -1:源文件不存在
     */
    int MoveFile(string origin_path, string dest_path) {
        // 定位源文件
        int ori_id = Find(origin_path);
        if(ori_id < 0)
            return ori_id;
        // 定位目标文件夹
        int dest_id = Find(dest_path);
        if(dest_id < 0)
            return -3;
        // 若目标不是文件夹
        if(!base_file_list.GetIsFolder(dest_id))
            return -4;
        // 定位源文件所在文件夹
        int ori_folder_id, delimiter_pos = origin_path.find_last_of('/');
        // 就是个纯文件，不包含路径
        if(delimiter_pos == -1) {
            ori_folder_id = folders[base_file_list.GetIndex(now_folder_base_id)].Find("..");
            // 根目录没有父文件夹
            ori_folder_id = (ori_folder_id == -1) ? 0 : ori_folder_id;
        }
        // 查找路径上的父文件夹
        else
            ori_folder_id = Find(origin_path.substr(0, delimiter_pos));

        // 源父文件夹移出
        folders[ori_folder_id].Delete(ori_id);
        // 目标文件夹新增
        folders[dest_id].Add(base_file_list.GetName(ori_id), ori_id);
    }
    /**
     * @brief 递归删除指定文件夹下所有的空文件夹(只包含文件夹的文件夹)，包括当前文件夹，不删除根目录
     *
     * @param path 指定路径
     * @return int 0:正常删除 -1:文件不存在 -2:路径错误
     */
    int DeleteEmptyFolder(string path) {
        // 定位文件夹
        int index = Find(path);
        if(index < 0)
            return index;
        // 删除
        DeleteEmptyFolder(index);
        return 0;
    }
    /**
     * @brief 获取文件对应的硬盘inode下标
     *
     * @param file_path 文件路径
     * @return int 硬盘inode下标位置，文件不存在时返回-1，路径错误时返回-2
     */
    int GetInodeID(string file_path) {
        int index = Find(file_path);
        if(index < 0)
            return index;
        return base_file_list.GetIndex(index);
    }

    /**
     * @brief 打开文件夹,支持两种方式:绝对路径(/)与相对路径(./ ../ name)
     *
     * @param folder_path 文件夹路径
     * @return true 打开成功
     * @return false 路径错误打开失败
     */
    bool OpenFolder(string folder_path) {
        if(folder_path.empty())
            return false;
        int folder_id = Find(folder_path, now_folder_base_id);
        // 如果没有对应路径
        if(folder_id < 0)
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
     * @brief 获取当前所在文件夹名称
     *
     * @return string 文件夹名称
     */
    string GetNowFolderName() {
        return now_folder_name;
    }

    /**
     * @brief 显示当前文件夹下所有文件
     *
     */
    void ShowFolder() {
        cout << endl;
        for(int j = 0; j < folders[now_folder].items.size(); j++) {
            if(folders[now_folder].items[j].name == "." || folders[now_folder].items[j].name == ".." || folders[now_folder].items[j].name[0] == '~')
                continue;
            cout << " ";
            cout << setw(15) << left << folders[now_folder].items[j].name << " ";
            cout << setw(6) << left << (base_file_list.items[folders[now_folder].items[j].index].is_folder ? "文件夹" : "文件");
            cout << endl;
        }
        cout << endl;
    }

    /**
     * @brief 保存目录数据
     * 基本表项数量
     * 基本表项
     * 文件夹目录数量
     * 当前文件夹编号
     * 当前文件夹目录项数
     * 当前文件夹目录项
     */
    void Save(FILE* file) {
        // 基本表
        fprintf(file, "%d\n", base_file_count);
        for(auto item: base_file_list.items) {
            fprintf(file, "%d %d %d %s\n", item.first, item.second.index, item.second.is_folder, item.second.name.c_str());
        }
        // 文件夹表
        fprintf(file, "%d\n", folders.size());
        for(auto folder: folders) {
            fprintf(file, "%d %d\n", folder.first, folder.second.items.size());
            for(auto item: folder.second.items) {
                fprintf(file, "%s %d\n", item.name.c_str(), item.index);
            }
        }
    }
    /**
     * @brief 将目录文件的数据转换成单个字符串
     *
     */
    string Save() {
        ostringstream data;
        // 基本表
        data << base_file_count << "\n";
        for(auto item: base_file_list.items) {
            data << item.first << " " << item.second.index << " " << item.second.is_folder << " " << item.second.name << "\n";
        }
        // 文件夹表
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
     * @brief 从文件恢复目录数据
     *
     */
    void Load(FILE* file) {
        int id, index, is_folder, siz;
        char name[50];
        // 基本表
        fscanf(file, "%d", &base_file_count);
        for(int i = 0; i < base_file_count; i++) {
            fscanf(file, "%d %d %d %s", &id, &index, &is_folder, name);
            base_file_list.Add(id, index, is_folder, string(name));
        }
        // 文件夹表
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
     * @brief 从字符串形式的数据恢复数据
     *
     */
    void Load(string data) {
        istringstream in(data);
        int id, index, is_folder, siz;
        char name[50];
        // 基本表
        in >> base_file_count;
        for(int i = 0; i < base_file_count; i++) {
            in >> id >> index >> is_folder >> name;
            base_file_list.Add(id, index, is_folder, string(name));
        }
        // 文件夹表
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
     * @brief 输出调试信息
     * 包含基本文件目录和所有的文件夹目录
     */
    void Debug() {
        cout << "基本文件目录 共" << base_file_list.items.size() << "个: " << endl;
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
        cout << "文件夹目录 共" << folders.size() << "个：" << endl;
        cout << " --------------------------" << endl;
        for(auto iter: folders) {
            cout << " | 文件夹 " << setw(15) << base_file_list.GetName(folders[iter.first].Find(".")) << " |" << endl;
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
