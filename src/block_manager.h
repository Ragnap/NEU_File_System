/**
 * @ 青空だけが見たいのは我儘ですか
 * @Author       : RagnaLP
 * @Date         : 2023-05-26 09:41:57
 * @LastEditTime : 2023-05-26 17:34:57
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
        free_blocks[--free_blocks[0]] = 0;
    }
} ListBlock;

/**
 * @brief 存放实际数据所用的块类
 *
 */
typedef struct DataBlock {
    /// 指向下个数据块的指针
    int next;
    /// 数据值
    char data[BLOCK_SIZE - sizeof(int)];
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
        cout << sizeof(ListBlock) << " " << sizeof(DataBlock);
        cout << "超级块中空闲节点栈:" << endl;
        cout << " ---------" << endl;
        for(int i = 0; i <= LIST_BLOCK_SIZE; i++) {
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