/**
 * @ 青空だけが見たいのは我儘ですか
 * @Author       : RagnaLP
 * @Date         : 2023-05-23 15:00:16
 * @LastEditTime : 2023-05-29 19:59:58
 * @Description  : 文件系统类
 */

#include "src/block_manager.h"
#include "src/config.h"
#include "src/menu_manager.h"
#include "src/user_manager.h"

/**
 * @brief 文件系统类
 * 负责与各个模块进行通讯，同时负责基本的交互
 */
typedef class FileSystem {
private:
    /// 用户管理模块
    UserManager user_manager;
    /// 目录管理模块
    MenuManager menu_manager;
    /// 块管理模块
    BlockMananger block_mananger;
    /**
     * @brief 清空系统
     *
     */
    void Clear() {
        block_mananger.Clear();
        menu_manager.Clear();
        user_manager.Clear();
    }

public:
    /**
     * @brief 初始化整个文件系统
     *
     */
    void Initalize(string root_password) {
        block_mananger.Initialize();
        menu_manager.Initialize();
        user_manager.Initialize(root_password);
    }

    /**
     * @brief 登录到文件系统
     * @return int 登录状态,0:密码错误,-1:用户不存在
     */
    int Login(string username, string password) {
        return user_manager.Login(username, password);
    }
    /**
     * @brief 注册文件系统中的用户
     * @return 注册是否成功
     */
    bool SignUp(string username, string password) {
        return user_manager.SignUp(username, password);
    }

    /**
     * @brief 创建文件夹
     *
     * @param folder_path 文件夹路径
     */
    bool CreateFolder(string folder_path) {
        return menu_manager.CreateFolder(folder_path);
    }

    /**
     * @brief 创建文件
     *
     * @param file_path 文件路径
     * @return int -2:地址错误,-1:磁盘块不足,0:存在同名文件,1:成功
     */
    int CreateFile(string file_path) {
        int block_id = block_mananger.CreateFile();
        if(block_id == -1)
            return -1;
        int result = menu_manager.CreateFile(file_path, block_id);
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
     * @brief 读取文件
     *
     * @param file_path 文件路径
     * @return pair<int, string> -2:路径错误 -1:无对应文件 0:成功
     * @todo 需要判读是否在内存中才可读取文件
     * @todo 判断用户权限
     */
    pair<int, string> ReadFile(string file_path) {
        int index = menu_manager.GetAddress(file_path);
        if(index < 0)
            return make_pair(-1, "");
        return make_pair(0, block_mananger.ReadFile(index));
    }
    /**
     * @brief 写入文件
     *
     * @param file_path 文件路径
     * @param content 写入的内容
     * @return int  -2:路径错误 -1:无对应文件,0:磁盘空间不足,1:成功
     * @todo 判断文件是否在内存中才可读取
     * @todo 判断用户权限
     */
    int WriteFile(string file_path, string content) {
        int index = menu_manager.GetAddress(file_path);
        if(index < 0)
            return index;
        return block_mananger.WriteFile(index, content);
    }

    /**
     * @brief 删除文件夹下的所有文件或指定文件
     *
     * @param file_path 文件（夹）路径
     * @return int -1:文件不存在 -2:路径错误
     */
    int DeleteFiles(string file_path) {
        // 获取所有的文件项的index与名称
        vector<pair<int, string>> files = menu_manager.GetFiles(file_path);
        if(files[0].first == -1 || files[0].first == -2)
            return files[0].first;
        // 删除单个文件
        if(files[0].first == -4) {
            menu_manager.DeleteFile(files[1].second, file_path);
            block_mananger.DeleteFile(files[1].first);
            return 0;
        }
        // 删除文件夹
        if(files[0].first != -3) {
            // 删除非空文件夹下的所有文件
            for(auto i: files) {
                menu_manager.DeleteFile(i.second, file_path);
                block_mananger.DeleteFile(i.first);
            }
        }
        // 删除空文件夹
        menu_manager.DeleteEmptyFolder(file_path);
        return 0;
    }
    /**
     * @brief 跳转到新的目录
     *
     * @param folder_path 新目录的路径
     * @return true 跳转成功
     * @return false 跳转失败
     */
    bool ChangeFolder(string folder_path) {
        return menu_manager.OpenFolder(folder_path);
    }
    /**
     * @brief 获取当前路径
     *
     */
    string GetCurrentPath() {
        return menu_manager.GetNowFolderName();
    }
    /**
     * @brief 获取当前用户名
     *
     */
    string GetCurrentUser() {
        return user_manager.GetCurrentUser();
    }
    /**
     * @brief 显示当前文件夹内容
     *
     */
    void ShowFolder() {
        menu_manager.ShowFolder();
    }

    /**
     * @brief 保存到文件
     * 目录系统
     * 用户系统
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
     * @brief 从文件读取
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
     * @brief 输出所有的调试信息
     *
     */
    void Debug() {
        block_mananger.Debug();
        user_manager.Debug();
        menu_manager.Debug();
    }

} FileSystem;