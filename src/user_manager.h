/**
 * @ 青空だけが見たいのは我儘ですか
 * @Author       : RagnaLP
 * @Date         : 2023-05-23 15:05:59
 * @LastEditTime : 2023-05-23 15:58:19
 * @Description  : 用户处理相关类
 */

#include "config.h"

/**
 * @brief 单个用户的信息类
 * 包含用于登录的用户名与密码
 */
typedef struct User {
    string username;
    string password;
} User;

/**
 * @brief 包含当前系统中所有用户的用户管理类
 *
 */
typedef class UserManager {
private:
    /// 已存在的用户信息
    vector<User> users;
    /// 当前系统用户
    User current_user;

public:
    /**
     * @brief 登录
     *
     * @param username 用户名
     * @param password 密码
     * @return int 登录状态,0:密码错误,-1:用户不存在
     */
    int Login(string username, string password) {
        for(auto user: users) {
            if(user.username == username) {
                if(user.password == password)
                    return 1;
                else {
                    current_user = user;
                    return 0;
                }
            }
        }
        return -1;
    }
    /**
     * @brief 注册
     *
     * @param username 用户名
     * @param password 密码
     * @return bool 注册状态
     */
    bool SignUp(string username, string password) {
        for(auto user: users) {
            if(user.username == username) {
                return false;
            }
        }
        User new_user;
        new_user.username = username;
        new_user.password = password;
        users.push_back(new_user);
        return true;
    }
    /**
     * @brief 获取当前用户名
     *
     * @return string 当前用户名
     */
    string GetCurrentUser() {
        return current_user.username;
    }
} UserManager;
