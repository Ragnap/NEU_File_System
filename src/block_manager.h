/**
 * @ 青空だけが見たいのは我儘ですか
 * @Author       : RagnaLP
 * @Date         : 2023-05-26 09:41:57
 * @LastEditTime : 2023-05-29 17:33:27
 * @Description  : 超级块与磁盘块系统
 */
#include "config.h"

/**
 * @brief 用于记录每组空闲块的使用情况的数据类
 *
 */
typedef struct ListBlock {
    /// 空闲块栈,free_block[0]为栈中元素个数,[1~n]为空闲块编号
    int free_blocks[LIST_BLOCK_SIZE];
    int count() {
        return free_blocks[0];
    }
    void clear() {
        for(int i = 0; i < LIST_BLOCK_SIZE; i++)
            free_blocks[i] = 0;
    }
    int top() {
        return free_blocks[count()];
    }
    void push(int block_id) {
        free_blocks[++free_blocks[0]] = block_id;
    }
    void pop() {
        free_blocks[free_blocks[0]--] = 0;
    }
} ListBlock;

/**
 * @brief 存放实际数据所用的块类
 *
 */
typedef struct DataBlock {
    /// 指向下个数据块的指针,当指针值为-1时表示文件的最后一个块
    int next;
    /// 数据值
    char data[DATA_BLOCK_SIZE];
} DataBlock;

/**
 * @brief 实际的磁盘块，可能为组长块或者数据块,在这里使用union来实现共用内存的效果
 *
 */
typedef union Block {
    ListBlock list_block;
    DataBlock data_block;
    void clear() {
        memset(this, 0, BLOCK_SIZE);
    }
} Block;

/**
 * @brief 完成块分配与块释放的块管理类
 *
 */
typedef class BlockMananger {
private:
    /// 超级块
    ListBlock super_block;
    /// 硬盘块
    Block block[BLOCK_NUM];

public:
    /**
     * @brief 清空空闲块的成组链接
     *
     */
    void Clear() {
        super_block.clear();
        for(int i = 0; i < BLOCK_NUM; i++)
            block[i].clear();
    }

    /**
     * @brief 初始化空闲块的成组链接
     *
     */
    void Initialize() {
        Clear();
        for(int i = 0; i < BLOCK_NUM; i++) {
            ReleaseBlock(i);
        }
    }

    /**
     * @brief 释放一个块为空闲块
     *
     * @param block_id 需要释放的块的id
     */
    void ReleaseBlock(int block_id) {
        // 超级块中空闲块已分配完
        if(super_block.count() == LIST_BLOCK_SIZE - 1) {
            block[block_id].list_block = super_block;
            super_block.clear();
            super_block.push(block_id);
        }
        else {
            super_block.push(block_id);
        }
    }

    /**
     * @brief 申请分配一个块
     *
     * @return int 分配的块号,-1时表示分配失败
     */
    int AllocateBlock() {
        int block_id;
        // 超级块中空闲块即将分配完
        if(super_block.count() == 1) {
            // 若无空闲块，分配失败
            if(block[super_block.top()].list_block.count() == 0) {
                return -1;
            }
            // 当前块分配后需要移入新的空闲列表
            else {
                block_id = super_block.top();
                super_block.pop();
                // 移入新的空闲列表
                super_block = block[block_id].list_block;
                // 原块进行清空
                block[block_id].clear();
            }
        }
        // 超级块剩余的空闲块能分配
        else {
            block_id = super_block.top();
            super_block.pop();
        }
        return block_id;
    }
    /**
     * @brief 删除完整文件，释放所有占用的块
     *
     * @param start_block_id 该文件的第一个数据块位置
     */
    void DeleteFile(int start_block_id) {
        int now_block_id = start_block_id;
        while(now_block_id != -1) {
            int next_block_id = block[now_block_id].data_block.next;
            // 清空数据
            block[now_block_id].clear();
            // 释放数据块
            ReleaseBlock(now_block_id);
            now_block_id = next_block_id;
        }
    }
    /**
     * @brief 为一个新的空文件创建块
     *
     * @return int 该文件的第一个数据块位置,-1时表示分配失败
     */
    int CreateFile() {
        int now_block = AllocateBlock();
        if(now_block == -1)
            return -1;
        block[now_block].data_block.next = -1;
        return now_block;
    }
    /**
     * @brief 获取磁盘中一个完整的文件内容
     *
     * @param start_block_id  该文件的第一个数据块位置
     * @return string 完整的文件内容
     */
    string ReadFile(int start_block_id) {
        string content = "";
        int now_block_id = start_block_id;
        while(now_block_id != -1) {
            for(int i = 0; i < DATA_BLOCK_SIZE && block[now_block_id].data_block.data[i] != 0; i++) {
                content += block[now_block_id].data_block.data[i];
            }
            now_block_id = block[now_block_id].data_block.next;
        }
        return content;
    }

    /**
     * @brief 向磁盘中写入一个完整的文件，默认为覆盖写入，可选附加写入
     *
     * @param start_block_id 该文件的第一个数据块位置
     * @param content 文件的内容
     * @param is_override 是否覆盖写入
     * @return bool 写入是否成功
     */
    bool WriteFile(int start_block_id, string content, bool is_override = true) {
        // 当前待写入块位置
        int now_block = start_block_id;
        // 当前待写入字符的位置
        int now_pos = 0;
        // 根据写入模式计算写入位置，定位最后一个块中的最后一个字符
        if(is_override) {
            // 清空源文件以及数据块
            DeleteFile(start_block_id);
            // 在Delete的时候会释放首数据块,需重新分配首数据块
            AllocateBlock();
        }
        else {
            // 找最后一个块
            while(block[now_block].data_block.next != -1) {
                now_block = block[now_block].data_block.next;
            }
            // 找块中最后一个可写位置
            while(now_pos < DATA_BLOCK_SIZE && block[now_block].data_block.data[now_pos] != 0) {
                now_pos++;
            }
        }
        // 待写入的字节数
        int wait = content.length();
        // 已完成写入的字节数
        int finish = 0;

        while(wait != 0) {
            // 写入当前块
            while(wait != 0 && now_pos < DATA_BLOCK_SIZE) {
                block[now_block].data_block.data[now_pos++] = content[finish++];
                wait--;
            }
            // 当前块写完已经没有未写入内容，退出
            if(wait == 0)
                break;

            // 当前块满但还没写完，分配新块
            int new_block = AllocateBlock();
            if(new_block == -1)
                return false;
            block[now_block].data_block.next = new_block;
            now_block = new_block;
            now_pos = 0;
        }

        // 给最后一个块打上终止符
        block[now_block].data_block.next = -1;
        return true;
    }

    /**
     * @brief 将块内容以字节为单位输出
     *
     */
    void Save(FILE* file) {
        // 超级块
        char* ptr = reinterpret_cast<char*>(&super_block);
        for(int i = 0; i < BLOCK_SIZE; i++) {
            fprintf(file, "%d ", *(ptr + i));
        }
        fprintf(file, "\n");

        // 磁盘块
        for(int i = 0; i < BLOCK_NUM; i++) {
            ptr = reinterpret_cast<char*>(&block[i]);
            for(int i = 0; i < BLOCK_SIZE; i++) {
                fprintf(file, "%d ", *(ptr + i));
            }
            fprintf(file, "\n");
        }
    }
    /**
     * @brief 将字节形式的块内容恢复
     *
     */
    void Load(FILE* file) {
        // 超级块
        char byte_data[BLOCK_SIZE];
        for(int i = 0; i < BLOCK_SIZE; i++) {
            int x;
            fscanf(file, "%d ", &x);
            *(byte_data + i) = static_cast<char>(x);
        }
        char* ptr = reinterpret_cast<char*>(&super_block);
        for(int i = 0; i < BLOCK_SIZE; i++) {
            *(ptr + i) = byte_data[i];
        }

        // 磁盘块
        for(int i = 0; i < BLOCK_NUM; i++) {
            for(int i = 0; i < BLOCK_SIZE; i++) {
                int x;
                fscanf(file, "%d ", &x);
                *(byte_data + i) = static_cast<char>(x);
            }
            ptr = reinterpret_cast<char*>(&block[i]);
            for(int i = 0; i < BLOCK_SIZE; i++) {
                *(ptr + i) = byte_data[i];
            }
        }
    }

    void Debug() {
        cout << "超级块中空闲节点栈:" << endl;
        cout << " ---------" << endl;
        for(int i = 0; i < LIST_BLOCK_SIZE; i++) {
            if(super_block.free_blocks[0] < i)
                cout << " |       | " << endl;
            else
                cout << " | " << setw(5) << super_block.free_blocks[i] << " | " << endl;
        }
        cout << " ---------" << endl;

        int next_list = super_block.free_blocks[1];

        while(next_list != 0) {
            cout << "组长块 #" << setw(3) << next_list << "：" << endl;
            cout << " ---------" << endl;
            for(int i = 0; i <= block[next_list].list_block.free_blocks[0]; i++) {
                cout << " | " << setw(5) << block[next_list].list_block.free_blocks[i] << " | " << endl;
            }
            cout << " ---------" << endl;
            next_list = block[next_list].list_block.free_blocks[1];
        }
    }
} BlockMananger;