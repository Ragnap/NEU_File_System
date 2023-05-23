/**
 * @ ��դ�����Ҋ�����Τ��҃��Ǥ���
 * @Author       : RagnaLP
 * @Date         : 2023-05-23 15:05:59
 * @LastEditTime : 2023-05-23 15:58:19
 * @Description  : �û����������
 */

#include "config.h"

/**
 * @brief �����û�����Ϣ��
 * �������ڵ�¼���û���������
 */
typedef struct User {
    string username;
    string password;
} User;

/**
 * @brief ������ǰϵͳ�������û����û�������
 *
 */
typedef class UserManager {
private:
    /// �Ѵ��ڵ��û���Ϣ
    vector<User> users;
    /// ��ǰϵͳ�û�
    User current_user;

public:
    /**
     * @brief ��¼
     *
     * @param username �û���
     * @param password ����
     * @return int ��¼״̬,0:�������,-1:�û�������
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
     * @brief ע��
     *
     * @param username �û���
     * @param password ����
     * @return bool ע��״̬
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
     * @brief ��ȡ��ǰ�û���
     *
     * @return string ��ǰ�û���
     */
    string GetCurrentUser() {
        return current_user.username;
    }
} UserManager;
