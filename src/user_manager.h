/**
 * @ ��դ�����Ҋ�����Τ��҃��Ǥ���
 * @Author       : RagnaLP
 * @Date         : 2023-05-23 15:05:59
 * @LastEditTime : 2023-05-25 16:57:40
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
     * @brief ����û�ϵͳ
     *
     */
    void Clear() {
        users.clear();
    }
    /**
     * @brief ��ʼ���û�ϵͳ����Ҫ�ṩ����Ա����
     *
     */
    void Initialize(string root_password) {
        users.clear();
        SignUp("root", root_password);
    }

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
    /**
     * @brief ���������û���Ϣ
     *
     */
    void Save(FILE* file) {
        fprintf(file, "%d\n", users.size());
        for(auto user: users) {
            fprintf(file, "%s %s\n", user.username.c_str(), user.password.c_str());
        }
    }
    /**
     * @brief ��ȡ�����û���Ϣ
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
     * @brief ���������Ϣ
     * �������е��û�
     */
    void Debug() {
        cout << "�û�Ŀ¼ ��" << users.size() << "��: " << endl;
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
