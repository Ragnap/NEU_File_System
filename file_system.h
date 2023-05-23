/**
 * @ 青空だけが見たいのは我儘ですか
 * @Author       : RagnaLP
 * @Date         : 2023-05-23 15:00:16
 * @LastEditTime : 2023-05-23 16:24:15
 * @Description  : 文件系统类
 */

#include "src/config.h"
#include "src/user_manager.h"
/**
 * @brief 文件系统类
 * 负责与各个模块进行通讯，同时负责基本的交互
 */
typedef class FileSystem {
private:
    /// 用户管理模块
    UserManager user_manager;

public:
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
} FileSystem;