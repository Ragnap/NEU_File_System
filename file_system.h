/**
 * @ 青空だけが見たいのは我儘ですか
 * @Author       : RagnaLP
 * @Date         : 2023-05-23 15:00:16
 * @LastEditTime : 2023-05-31 20:50:20
 * @Description  : 文件系统类
 */

#include "src/block_manager.h"
#include "src/config.h"
#include "src/memory_manager.h"
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
    BlockMananger block_manager;
    /// 内存管理模块
    MemoryManager memory_manager;
    /**
     * @brief 清空系统
     *
     */
    void Clear() {
        block_manager.Clear();
        menu_manager.Clear();
        user_manager.Clear();
        memory_manager.Clear();
    }
    /**
     * @brief 读取目录数据,目录数据块号固定为 BLOCK_NUM-1
     */
    string ReadMenu() {
        return block_manager.ReadFile(BLOCK_NUM - 1);
    }
    /**
     * @brief 读取inode节点数据，数据块号固定为 BLOCK_NUM-2
     */
    string ReadMemory() {
        return block_manager.ReadFile(BLOCK_NUM - 2);
    };
    /**
     * @brief 读取用户数据,数据块号固定为 BLOCK_NUM-3
     */
    string ReadUser() {
        return block_manager.ReadFile(BLOCK_NUM - 3);
    }

public:
    /**
     * @brief 初始化整个文件系统
     *
     */
    void Initalize(string root_password) {
        block_manager.Initialize();
        memory_manager.Initialize();
        menu_manager.Initialize();
        user_manager.Initialize(root_password);
        CreateFile("/~menu", "SYSTEM");
        CreateFile("/~inode", "SYSTEM");
        CreateFile("/~user", "SYSTEM");
        OpenFile("/~menu");
        OpenFile("/~inode");
        OpenFile("/~user");
        WriteFile("/~menu", menu_manager.Save(), "SYSTEM");
        WriteFile("/~user", user_manager.Save(), "SYSTEM");
        WriteFile("/~inode", memory_manager.Save(), "SYSTEM");
        getchar();
        getchar();
        CloseFile("/~menu");
        CloseFile("/~inode");
        CloseFile("/~user");
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
     * @param user 操作用户
     * @return int -3:硬盘inode数量不足 -2:地址错误,-1:磁盘块不足,0:存在同名文件,1:成功
     */
    int CreateFile(string file_path, string user = "") {
        int block_id = block_manager.CreateFile();
        if(block_id == -1)
            return -1;
        int inode_id = memory_manager.CreateFile(block_id, (user.empty() ? GetCurrentUser() : user));
        if(inode_id == -1) {
            block_manager.DeleteFile(block_id);
            return -3;
        }
        int result = menu_manager.CreateFile(file_path, inode_id);
        if(result == -1) {
            block_manager.DeleteFile(block_id);
            return 0;
        }
        if(result == -2) {
            block_manager.DeleteFile(block_id);
            return -2;
        }

        return 1;
    }
    /**
     * @brief 打开文件到内存
     *
     * @param file_path 文件路径
     * @return int -5:内存inode不足 -4:内存不足 -3:已经打开 -2:路径错误 -1:无对应文件 0:成功
     */
    int OpenFile(string file_path) {
        int inode = menu_manager.GetInodeID(file_path);
        if(inode < 0)
            return inode;
        string content = block_manager.ReadFile(memory_manager.GetDiscAddress(inode));
        int result = memory_manager.OpenFile(inode, content);
        if(result < 0)
            return result - 2;
        return 0;
    }
    /**
     * @brief 关闭文件，移出内存
     *
     * @param file_path 文件路径
     * @return int -3:未打开 -2:路径错误 -1:无对应文件 0:成功
     */
    int CloseFile(string file_path) {
        int inode = menu_manager.GetInodeID(file_path);
        if(inode < 0)
            return inode;
        int result = memory_manager.CloseFile(inode);
        if(result < 0)
            return -3;
        return 0;
    }
    /**
     * @brief 读取文件
     *
     * @param file_path 文件路径
     * @return pair<int, string> -3:文件未打开 -2:路径错误 -1:无对应文件 0:成功
     */
    pair<int, string> ReadFile(string file_path) {
        int inode = menu_manager.GetInodeID(file_path);
        if(inode < 0)
            return make_pair(inode, "");
        if(!memory_manager.IsOpened(inode))
            return make_pair(-3, "");
        return make_pair(0, memory_manager.ReadFile(inode));
    }
    /**
     * @brief 写入文件
     *
     * @param file_path 文件路径
     * @param content 写入的内容
     * @param user 操作者姓名，默认为当前登录用户
     * @return int  -5:内存不足 -4:权限不足 -3:文件未打开 -2:路径错误 -1:无对应文件,0:磁盘空间不足,1:成功
     * @todo 判断文件是否在内存中才可读取
     * @todo 判断用户权限
     */
    int WriteFile(string file_path, string content, string user = "") {
        int inode = menu_manager.GetInodeID(file_path);
        if(inode < 0)
            return inode;
        if(!memory_manager.IsOpened(inode))
            return -3;
        // 写入到内存
        int result = memory_manager.WriteFile(inode, (user.empty() ? GetCurrentUser() : user), content);
        if(result < 0)
            return result - 2;
        // 写入到磁盘
        return block_manager.WriteFile(memory_manager.GetDiscAddress(inode), content);
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
            int disc_address = memory_manager.GetDiscAddress(files[1].first);
            // 如果有修改权限才能删除
            if(memory_manager.DeleteFile(files[1].first, GetCurrentUser()))
                block_manager.DeleteFile(disc_address);
            return 0;
        }
        // 删除文件夹
        if(files[0].first != -3) {
            // 尝试删除非空文件夹下的所有文件
            for(auto i: files) {
                menu_manager.DeleteFile(i.second, file_path);
                int disc_address = memory_manager.GetDiscAddress(i.first);
                // 如果有修改权限才能删除
                if(memory_manager.DeleteFile(i.first, GetCurrentUser()))
                    block_manager.DeleteFile(disc_address);
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
     * @brief 返回内存使用情况
     *
     * @return pair<int,int> (已使用，总)
     */
    pair<int, int> GetCapacity() {
        return memory_manager.GetCapacity();
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
        // 保存到磁盘文件
        WriteFile("/~menu", menu_manager.Save());
        WriteFile("/~user", user_manager.Save());
        WriteFile("/~inode", memory_manager.Save());
        block_manager.Save(f);
        fclose(f);
        // 保存为人眼可看的肉眼文件
        FILE* detail_f = fopen(DETAIL_FILE_PATH.c_str(), "w");
        menu_manager.Save(detail_f);
        user_manager.Save(detail_f);
        memory_manager.Save(detail_f);
        fclose(detail_f);
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
        // 从磁盘文件读入
        block_manager.Load(f);
        menu_manager.Load(ReadMenu());
        memory_manager.Load(ReadMemory());
        user_manager.Load(ReadUser());
        fclose(f);
        // 从肉眼文件读入
        // FILE* detail_f = fopen(DETAIL_FILE_PATH.c_str(), "r");
        // menu_manager.Load(detail_f);
        // user_manager.Load(detail_f);
        // fclose(detail_f);
        return true;
    }
    /**
     * @brief 输出所有的调试信息
     *
     */
    void Debug() {
        memory_manager.Debug();
        block_manager.Debug();
        user_manager.Debug();
        menu_manager.Debug();
    }

} FileSystem;