/**
 * @ 青空だけが見たいのは我儘ですか
 * @Author       : RagnaLP
 * @Date         : 2023-05-23 15:00:16
 * @LastEditTime : 2023-05-26 17:35:09
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
     * @param folder_name 文件夹名
     */
    bool CreateFolder(string folder_name) {
        return menu_manager.CreateFolder(folder_name);
    }

    /**
     * @brief 创建文件
     *
     * @param file_name 文件名
     */
    bool CreateFile(string file_name) {
        return menu_manager.CreateFile(file_name, 100);
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