/**
 * @ 青空だけが見たいのは我儘ですか
 * @Author       : RagnaLP
 * @Date         : 2023-05-23 15:00:16
 * @LastEditTime : 2023-05-23 20:11:24
 * @Description  : 文件系统类
 */

#include "src/config.h"
#include "src/menu_manager.h"
#include "src/user_manager.h"

/**
 * @brief 文件系统类
 * 负责与各个模块进行通讯，同时负责基本的交互
 */
typedef class FileSystem {
private:
public:
    /// 用户管理模块
    UserManager user_manager;
    /// 目录管理模块
    MenuManager menu_manager;

    /**
     * @brief 初始化整个文件系统
     *
     */
    void Initalize() {
        menu_manager.Initialize();
    }

    /**
     * @brief 登录到文件系统
     *
     */
    void Login(string username, string password) {
        int result = user_manager.Login(username, password);
        if(result == -1) {
            cout << "用户不存在！" << endl;
        }
        if(result == 0) {
            cout << "密码错误！" << endl;
        }
    }
    /**
     * @brief 注册文件系统中的用户
     *
     */
    void SignUp(string username, string password) {
        bool result = user_manager.SignUp(username, password);
        if(result) {
            cout << "注册成功！" << endl;
        }
        else {
            cout << "用户名已存在！" << endl;
        }
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

    bool ChangeFolder(string folder_path) {
        return menu_manager.OpenFolder(folder_path);
    }
    /**
     * @brief 获取当前路径
     *
     * @return string
     */
    string GetCurrentPath() {
        return menu_manager.GetNowFolderName();
    }
    string GetCurrentUser() {
        return user_manager.GetCurrentUser();
    }
    void ShowFolder() {
        menu_manager.ShowFolder();
    }
    /**
     * @brief 输出所有的调试信息
     *
     */
    void Debug() {
        menu_manager.Debug();
    }
} FileSystem;