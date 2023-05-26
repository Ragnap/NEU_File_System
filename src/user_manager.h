/**
 * @ 青空だけが見たいのは我儘ですか
 * @Author       : RagnaLP
 * @Date         : 2023-05-23 15:05:59
 * @LastEditTime : 2023-05-25 16:57:40
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
     * @brief 清空用户系统
     *
     */
    void Clear() {
        users.clear();
    }
    /**
     * @brief 初始化用户系统，需要提供管理员密码
     *
     */
    void Initialize(string root_password) {
        users.clear();
        SignUp("root", root_password);
    }

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
                if(user.password == password) {
                    current_user = user;
                    return 1;
                }
                else {
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
    /**
     * @brief 保存所有用户信息
     *
     */
    void Save(FILE* file) {
        fprintf(file, "%d\n", users.size());
        for(auto user: users) {
            fprintf(file, "%s %s\n", user.username.c_str(), user.password.c_str());
        }
    }
    /**
     * @brief 读取所有用户信息
     *
     */
    void Load(FILE* file) {
        char username[50];
        char password[50];
        int siz;
        fscanf(file, "%d", &siz);
        for(int i = 0; i < siz; i++) {
            fscanf(file, "%s %s\n", username, password);
            User new_user;
            new_user.username = username;
            new_user.password = password;
            users.push_back(new_user);
        }
    }
    /**
     * @brief 输出调试信息
     * 包含所有的用户
     */
    void Debug() {
        cout << "用户目录 共" << users.size() << "个: " << endl;
        cout << " ----------------------------------" << endl;
        for(int i = 0; i < users.size(); i++) {
            cout << " | ";
            cout << setw(11) << left << users[i].username << " | ";
            cout << setw(15) << left << users[i].password;
            cout << " | " << endl;
        }
        cout << " ----------------------------------" << endl;
        cout << endl;
    }
} UserManager;
