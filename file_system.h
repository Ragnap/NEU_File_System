/**
 * @ ��դ�����Ҋ�����Τ��҃��Ǥ���
 * @Author       : RagnaLP
 * @Date         : 2023-05-23 15:00:16
 * @LastEditTime : 2023-05-23 16:24:15
 * @Description  : �ļ�ϵͳ��
 */

#include "src/config.h"
#include "src/user_manager.h"
/**
 * @brief �ļ�ϵͳ��
 * ���������ģ�����ͨѶ��ͬʱ��������Ľ���
 */
typedef class FileSystem {
private:
    /// �û�����ģ��
    UserManager user_manager;

public:
    /**
     * @brief ��¼���ļ�ϵͳ
     *
     */
    void Login(string username, string password) {
        int result = user_manager.Login(username, password);
        if(result == -1) {
            cout << "�û������ڣ�" << endl;
        }
        if(result == 0) {
            cout << "�������" << endl;
        }
    }
    /**
     * @brief ע���ļ�ϵͳ�е��û�
     *
     */
    void SignUp(string username, string password) {
        bool result = user_manager.SignUp(username, password);
        if(result) {
            cout << "ע��ɹ���" << endl;
        }
        else {
            cout << "�û����Ѵ��ڣ�" << endl;
        }
    }
} FileSystem;