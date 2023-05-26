/**
 * @ ��դ�����Ҋ�����Τ��҃��Ǥ���
 * @Author       : RagnaLP
 * @Date         : 2023-05-26 09:41:57
 * @LastEditTime : 2023-05-26 17:34:57
 * @Description  : ����������̿�ϵͳ
 */
#include "config.h"

/**
 * @brief ���ڼ�¼ÿ����п��ʹ�������������
 *
 */
typedef struct ListBlock {
    /// ���п�ջ,free_block[0]Ϊջ��Ԫ�ظ���,[1~n]Ϊ���п���
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
 * @brief ���ʵ���������õĿ���
 *
 */
typedef struct DataBlock {
    /// ָ���¸����ݿ��ָ��
    int next;
    /// ����ֵ
    char data[BLOCK_SIZE - sizeof(int)];
} DataBlock;

/**
 * @brief ʵ�ʵĴ��̿飬����Ϊ�鳤��������ݿ�,������ʹ��union��ʵ�ֹ����ڴ��Ч��
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
 * @brief ��ɿ��������ͷŵĿ������
 *
 */
typedef class BlockMananger {
private:
    /// ������
    ListBlock super_block;
    /// Ӳ�̿�
    Block block[BLOCK_NUM];

public:
    /**
     * @brief ��տ��п�ĳ�������
     *
     */
    void Clear() {
        super_block.clear();
        for(int i = 0; i < BLOCK_NUM; i++)
            block[i].clear();
    }

    /**
     * @brief ��ʼ�����п�ĳ�������
     *
     */
    void Initialize() {
        Clear();
        for(int i = 0; i < BLOCK_NUM; i++) {
            ReleaseBlock(i);
        }
    }

    /**
     * @brief �ͷ�һ����Ϊ���п�
     *
     * @param block_id ��Ҫ�ͷŵĿ��id
     */
    void ReleaseBlock(int block_id) {
        // �������п��п��ѷ�����
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
     * @brief �������һ����
     *
     * @return int ����Ŀ��,-1ʱ��ʾ����ʧ��
     */
    int AllocateBlock() {
        int block_id;
        // �������п��п鼴��������
        if(super_block.count() == 1) {
            // ���޿��п飬����ʧ��
            if(block[super_block.top()].list_block.count() == 0) {
                return -1;
            }
            // ��ǰ��������Ҫ�����µĿ����б�
            else {
                block_id = super_block.top();
                super_block.pop();
                // �����µĿ����б�
                super_block = block[block_id].list_block;
            }
        }
        // ������ʣ��Ŀ��п��ܷ���
        else {
            block_id = super_block.top();
            super_block.pop();
        }
        return block_id;
    }
    /**
     * @brief �����������ֽ�Ϊ��λ���
     *
     */
    void Save(FILE* file) {
        // ������
        char* ptr = reinterpret_cast<char*>(&super_block);
        for(int i = 0; i < BLOCK_SIZE; i++) {
            fprintf(file, "%d ", *(ptr + i));
        }
        fprintf(file, "\n");

        // ���̿�
        for(int i = 0; i < BLOCK_NUM; i++) {
            ptr = reinterpret_cast<char*>(&block[i]);
            for(int i = 0; i < BLOCK_SIZE; i++) {
                fprintf(file, "%d ", *(ptr + i));
            }
            fprintf(file, "\n");
        }
    }
    /**
     * @brief ���ֽ���ʽ�Ŀ����ݻָ�
     *
     */
    void Load(FILE* file) {
        // ������
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

        // ���̿�
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
        cout << "�������п��нڵ�ջ:" << endl;
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
            cout << "�鳤�� #" << setw(3) << next_list << "��" << endl;
            cout << " ---------" << endl;
            for(int i = 0; i <= block[next_list].list_block.free_blocks[0]; i++) {
                cout << " | " << setw(5) << block[next_list].list_block.free_blocks[i] << " | " << endl;
            }
            cout << " ---------" << endl;
            next_list = block[next_list].list_block.free_blocks[1];
        }
    }
} BlockMananger;