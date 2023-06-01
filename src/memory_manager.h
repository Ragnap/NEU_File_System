/**
 * @ 青空だけが見たいのは我儘ですか
 * @Author       : RagnaLP
 * @Date         : 2023-05-31 14:41:28
 * @LastEditTime : 2023-06-01 10:26:34
 * @Description  : 内存管理与Inode系统
 */
#include "config.h"
/// 磁盘inode
struct DiscIndexNode {
    /// 用于标注文件权限，仅创建者可修改
    string creator;
    /// 文件大小
    int file_size;
    /// 实际磁盘首地址
    int disc_block_address;
    // 指向内存中inode编号
    int mem_inode_id;
};
/// 内存inode
struct MemIndexNode {
    // 内存块地址
    int mem_block_address;
    // 指向对应文件的磁盘inode编号
    int disc_inode_id;
};
/**
 * @brief inode管理类
 */
class MemoryManager {
private:
    /// 磁盘inode
    map<int, DiscIndexNode> disc_inodes;
    /// 空闲磁盘inode栈
    stack<int> free_disc_inode;
    /// 内存inode
    map<int, MemIndexNode> mem_inodes;
    /// 空闲内存inode栈
    stack<int> free_mem_inode;

    /// 实际内存
    char mem[MEM_BLOCK_NUM][MEM_BLOCK_SIZE];
    /// 内存链接表,指向下个内存块的指针，当指针值为-1时表示为文件的最后一个块
    int mem_next[MEM_BLOCK_NUM];

    /// 空闲内存块块号栈
    stack<int> free_mem_block;

    /**
     * @brief 获取某个文件在内存中的首地址
     *
     * @param disc_inode_id 磁盘inode编号
     * @return int 内存中的首地址 -1:文件未打开
     */
    int GetMemAddress(int disc_inode_id) {
        if(!IsOpened(disc_inode_id))
            return -1;
        return mem_inodes[disc_inodes[disc_inode_id].mem_inode_id].mem_block_address;
    }
    /**
     * @brief 向硬盘内直接写入内容，调用前需要做好条件判断
     *
     * @param head_mem_address 内存中的首地址
     * @param content 内容
     */
    void WriteMem(int head_mem_address, string content) {
        // 写入到内存
        int now_block = head_mem_address;
        int now_pos = 0;
        int wait = content.length();
        int finish = 0;
        while(wait != 0) {
            // 写入当前内存块
            while(wait != 0 && now_pos < MEM_BLOCK_SIZE) {
                mem[now_block][now_pos++] = content[finish++];
                wait--;
            }
            // 已经没有未写入内容，退出
            if(wait == 0)
                break;

            // 当前内存块满但还没写完，分配新块
            int new_block = free_mem_inode.top();
            free_mem_inode.pop();

            mem_next[now_block] = new_block;
            now_block = new_block;
            now_pos = 0;
        }
        mem_next[now_block] = -1;
    }
    /**
     * @brief 删除内存中的一个完整项
     *
     * @param start_mem_address 首内存块地址
     */
    void DeleteMem(int start_mem_address) {
        if(mem_next[start_mem_address] != -1)
            DeleteMem(mem_next[start_mem_address]);
        mem_next[start_mem_address] = -1;
        memset(mem[start_mem_address], 0, sizeof(mem[start_mem_address]));
        free_mem_block.push(start_mem_address);
    }

public:
    /**
     * @brief 检查某个磁盘inode对应的文件是否已打开
     *
     * @param disc_inode_id 磁盘inode编号
     */
    bool IsOpened(int disc_inode_id) {
        if(disc_inodes.find(disc_inode_id) == disc_inodes.end())
            return 0;
        return disc_inodes[disc_inode_id].mem_inode_id != -1;
    }
    /**
     * @brief 获取实际硬盘地址
     *
     * @param disc_inode_id 磁盘inode编号
     * @return int
     */
    int GetDiscAddress(int disc_inode_id) {
        return disc_inodes[disc_inode_id].disc_block_address;
    }
    /**
     * @brief 返回内存使用情况
     *
     * @return pair<int,int> (已使用，总)
     */
    pair<int, int> GetCapacity() {
        return make_pair((MEM_BLOCK_NUM - free_mem_block.size()) * MEM_BLOCK_SIZE, MEM_BLOCK_NUM * MEM_BLOCK_SIZE);
    }
    /**
     * @brief 清空内存与inode信息
     */
    void Clear() {
        disc_inodes.clear();
        while(!free_disc_inode.empty())
            free_disc_inode.pop();

        mem_inodes.clear();
        while(!free_mem_inode.empty())
            free_mem_inode.pop();

        memset(mem, 0, sizeof(mem));
        memset(mem_next, -1, sizeof(mem_next));
        while(!free_mem_block.empty())
            free_mem_block.pop();
    }

    /**
     * @brief 初始化内存与inode信息
     */
    void Initialize() {
        Clear();
        for(int i = 0; i < INODE_NUM; i++) {
            free_disc_inode.push(i);
            free_mem_inode.push(i);
        }
        for(int i = 0; i < MEM_BLOCK_NUM; i++)
            free_mem_block.push(i);
    }

    /**
     * @brief 创建文件，获取对应的硬盘inode节点编号
     *
     * @param disc_block_address 文件实际的硬盘块地址
     * @param user_name 创建者用户名
     * @return int inode编号 -1:硬盘inode不足
     */
    int CreateFile(int disc_block_address, string user_name) {
        if(free_disc_inode.empty())
            return -1;
        int id = free_disc_inode.top();
        free_disc_inode.pop();
        DiscIndexNode inode;
        inode.creator = user_name;
        inode.file_size = 0;
        inode.disc_block_address = disc_block_address;
        inode.mem_inode_id = -1;
        disc_inodes[id] = inode;
        return id;
    }

    /**
     * @brief 读取硬盘内容到内存中
     *
     * @param disc_inode_id 硬盘inode编号
     * @param content 内容
     * @return int 0:正常 -1:已经打开 -2:内存不足 -3:内存inode不足
     */
    int OpenFile(int disc_inode_id, string content) {
        DiscIndexNode disc_inode = disc_inodes[disc_inode_id];
        // 检测是否已经打开
        if(IsOpened(disc_inode_id))
            return -1;
        // 判断内存是否够大
        if(content.length() > free_mem_block.size() * MEM_BLOCK_SIZE)
            return -2;
        // 检测内存inode数量是否足够
        if(free_mem_inode.empty())
            return -3;
        // 分配内存inode
        MemIndexNode mem_inode;
        int mem_inode_id = free_mem_inode.top();
        free_mem_inode.pop();
        mem_inode.disc_inode_id = disc_inode_id;
        mem_inode.mem_block_address = free_mem_block.top();
        free_mem_block.pop();
        mem_inodes[mem_inode_id] = mem_inode;
        // 写入到内存
        WriteMem(mem_inode.mem_block_address, content);
        // 记录状态
        disc_inodes[disc_inode_id].file_size = content.length();
        disc_inodes[disc_inode_id].mem_inode_id = mem_inode_id;
        return 0;
    }
    /**
     * @brief 获取内存中一个完整的文件内容
     *
     * @param disc_inode_id  硬盘inode编号
     * @return string 完整的文件内容
     */
    string ReadFile(int disc_inode_id) {
        if(!IsOpened(disc_inode_id))
            return "";
        string content = "";
        int now_mem_address = GetMemAddress(disc_inode_id);
        while(now_mem_address != -1) {
            for(int i = 0; i < MEM_BLOCK_SIZE && mem[now_mem_address][i] != 0; i++) {
                content += mem[now_mem_address][i];
            }
            now_mem_address = mem_next[now_mem_address];
        }
        return content;
    }
    /**
     * @brief 修改内存中一个文件的内容
     *
     * @param disc_inode_id 硬盘inode编号
     * @param user 当前用户
     * @param content 完整的文件内容
     * @return int 0:成功 -1:文件未打开 -2:权限不足 -3:内存不足
     */
    int WriteFile(int disc_inode_id, string user, string content) {
        if(!IsOpened(disc_inode_id))
            return -1;
        if(disc_inodes[disc_inode_id].creator != user)
            return -2;
        // 判断内存是否够大
        if(content.length() - disc_inodes[disc_inode_id].file_size > free_mem_block.size() * MEM_BLOCK_SIZE)
            return -3;
        int start_mem_address = GetMemAddress(disc_inode_id);
        // 清空内存中的源文件以及数据块
        DeleteMem(start_mem_address);
        // 重新占用首内存块
        free_mem_block.pop();
        WriteMem(start_mem_address, content);
        // 修改状态
        disc_inodes[disc_inode_id].file_size = content.size();
        return 0;
    }
    /**
     * @brief 关闭内存中一个文件的内容
     *
     * @param disc_inode_id 硬盘inode编号
     * @return int 0:成功 -1:文件未打开
     */
    int CloseFile(int disc_inode_id) {
        if(!IsOpened(disc_inode_id))
            return -1;
        int start_mem_address = GetMemAddress(disc_inode_id);
        // 清空内存中的源文件以及数据块
        DeleteMem(start_mem_address);
        // 释放内存inode
        mem_inodes.erase(disc_inodes[disc_inode_id].mem_inode_id);
        free_mem_inode.push(disc_inodes[disc_inode_id].mem_inode_id);
        // 修改状态
        disc_inodes[disc_inode_id].mem_inode_id = -1;
        return 0;
    }
    /**
     * @brief 删除文件对应操作
     *
     * @param disc_inode_id 文件的磁盘inode编号
     */
    bool DeleteFile(int disc_inode_id, string user) {
        if(IsOpened(disc_inode_id))
            CloseFile(disc_inode_id);
        // 权限检查
        if(disc_inodes[disc_inode_id].creator != user)
            return 0;
        // 释放硬盘inode
        disc_inodes.erase(disc_inode_id);
        free_disc_inode.push(disc_inode_id);
        return 1;
    }
    /**
     * @brief 将索引文件的数据转换成单个字符串
     *
     */
    void Save(FILE* file) {
        // 空闲磁盘inode表
        fprintf(file, "%d\n", free_disc_inode.size());
        stack<int> temp = free_disc_inode;
        while(!temp.empty()) {
            fprintf(file, "%d ", temp.top());
            temp.pop();
        }
        fprintf(file, "\n");
        // 文件夹表
        fprintf(file, "%d\n", disc_inodes.size());
        for(auto i: disc_inodes) {
            fprintf(file, "%d %s %d %d\n", i.first, i.second.creator.c_str(), i.second.file_size, i.second.disc_block_address);
        }
    }
    /**
     * @brief 将索引文件的数据转换成单个字符串
     *
     */
    string Save() {
        ostringstream data;
        // 空闲磁盘inode表
        data << free_disc_inode.size() << "\n";
        stack<int> temp = free_disc_inode;
        while(!temp.empty()) {
            data << temp.top() << " ";
            temp.pop();
        }
        data << "\n";
        // 已有磁盘inode表
        data << disc_inodes.size() << "\n";
        for(auto i: disc_inodes) {
            data << i.first << " " << i.second.creator << " " << i.second.file_size << " " << i.second.disc_block_address << "\n";
        }
        return data.str();
    }
    /**
     * @brief 从字符串形式的数据恢复数据
     *
     */
    void Load(string data) {
        istringstream in(data);
        int id, siz;
        // 空闲磁盘inode表
        in >> siz;
        for(int i = 0; i < siz; i++) {
            in >> id;
            free_disc_inode.push(id);
        }
        // 已有磁盘inode表
        in >> siz;
        for(int i = 0; i < siz; i++) {
            DiscIndexNode inode;
            in >> id >> inode.creator >> inode.file_size >> inode.disc_block_address;
            inode.mem_inode_id = -1;
            disc_inodes[id] = inode;
        }
        // 初始化内存有关
        for(int i = 0; i < INODE_NUM; i++)
            free_mem_inode.push(i);
        for(int i = 0; i < MEM_BLOCK_NUM; i++)
            free_mem_block.push(i);
    }

    void Debug() {
        cout << endl << "内存情况如下：" << endl;
        cout << "      ";
        for(int i = 0; i < MEM_BLOCK_SIZE; i++)
            cout << "-";
        cout << endl;
        for(int i = 0; i < MEM_BLOCK_NUM; i++) {
            cout << setw(3) << i << " | ";
            for(int j = 0; j < MEM_BLOCK_SIZE; j++) {
                if(mem[i][j] == '\n')
                    cout << setw(3) << "\\n";
                else if(mem[i][j] == '\t')
                    cout << setw(3) << "\\t";
                else if(mem[i][j] == '\0')
                    cout << setw(3) << "\\0";
                else
                    cout << setw(3) << (char)mem[i][j];
            }
            cout << " | - " << setw(4) << mem_next[i] << endl;
        }
        cout << "      ";
        for(int i = 0; i < MEM_BLOCK_SIZE; i++)
            cout << "-";
        cout << endl;

        cout << "磁盘inode如下：" << endl;
        cout << " --------------------------------------------------" << endl;
        cout << " | id |   user   |   size   |  mem_id  | disc_adr | " << endl;

        for(auto i: disc_inodes) {
            cout << " | ";
            cout << setw(2) << left << i.first << " | ";
            cout << setw(8) << left << i.second.creator << " | ";
            cout << setw(8) << left << i.second.file_size << " | ";
            cout << setw(8) << left << i.second.mem_inode_id << " | ";
            cout << setw(8) << left << i.second.disc_block_address;
            cout << " | " << endl;
        }
        cout << " --------------------------------------------------" << endl;

        cout << "内存inode如下：" << endl;
        cout << " ----------------------------------------" << endl;
        cout << " | id |   disc_inode   |    mem_addr    | " << endl;

        for(auto i: mem_inodes) {
            cout << " | ";
            cout << setw(2) << left << i.first << " | ";
            cout << setw(14) << left << i.second.disc_inode_id << " | ";
            cout << setw(14) << left << i.second.mem_block_address;
            cout << " | " << endl;
        }
        cout << " ----------------------------------------" << endl;
    }
};