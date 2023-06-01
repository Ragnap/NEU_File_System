/**
 * @ ��դ�����Ҋ�����Τ��҃��Ǥ���
 * @Author       : RagnaLP
 * @Date         : 2023-05-31 14:41:28
 * @LastEditTime : 2023-06-01 10:26:34
 * @Description  : �ڴ������Inodeϵͳ
 */
#include "config.h"
/// ����inode
struct DiscIndexNode {
    /// ���ڱ�ע�ļ�Ȩ�ޣ��������߿��޸�
    string creator;
    /// �ļ���С
    int file_size;
    /// ʵ�ʴ����׵�ַ
    int disc_block_address;
    // ָ���ڴ���inode���
    int mem_inode_id;
};
/// �ڴ�inode
struct MemIndexNode {
    // �ڴ���ַ
    int mem_block_address;
    // ָ���Ӧ�ļ��Ĵ���inode���
    int disc_inode_id;
};
/**
 * @brief inode������
 */
class MemoryManager {
private:
    /// ����inode
    map<int, DiscIndexNode> disc_inodes;
    /// ���д���inodeջ
    stack<int> free_disc_inode;
    /// �ڴ�inode
    map<int, MemIndexNode> mem_inodes;
    /// �����ڴ�inodeջ
    stack<int> free_mem_inode;

    /// ʵ���ڴ�
    char mem[MEM_BLOCK_NUM][MEM_BLOCK_SIZE];
    /// �ڴ����ӱ�,ָ���¸��ڴ���ָ�룬��ָ��ֵΪ-1ʱ��ʾΪ�ļ������һ����
    int mem_next[MEM_BLOCK_NUM];

    /// �����ڴ����ջ
    stack<int> free_mem_block;

    /**
     * @brief ��ȡĳ���ļ����ڴ��е��׵�ַ
     *
     * @param disc_inode_id ����inode���
     * @return int �ڴ��е��׵�ַ -1:�ļ�δ��
     */
    int GetMemAddress(int disc_inode_id) {
        if(!IsOpened(disc_inode_id))
            return -1;
        return mem_inodes[disc_inodes[disc_inode_id].mem_inode_id].mem_block_address;
    }
    /**
     * @brief ��Ӳ����ֱ��д�����ݣ�����ǰ��Ҫ���������ж�
     *
     * @param head_mem_address �ڴ��е��׵�ַ
     * @param content ����
     */
    void WriteMem(int head_mem_address, string content) {
        // д�뵽�ڴ�
        int now_block = head_mem_address;
        int now_pos = 0;
        int wait = content.length();
        int finish = 0;
        while(wait != 0) {
            // д�뵱ǰ�ڴ��
            while(wait != 0 && now_pos < MEM_BLOCK_SIZE) {
                mem[now_block][now_pos++] = content[finish++];
                wait--;
            }
            // �Ѿ�û��δд�����ݣ��˳�
            if(wait == 0)
                break;

            // ��ǰ�ڴ��������ûд�꣬�����¿�
            int new_block = free_mem_inode.top();
            free_mem_inode.pop();

            mem_next[now_block] = new_block;
            now_block = new_block;
            now_pos = 0;
        }
        mem_next[now_block] = -1;
    }
    /**
     * @brief ɾ���ڴ��е�һ��������
     *
     * @param start_mem_address ���ڴ���ַ
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
     * @brief ���ĳ������inode��Ӧ���ļ��Ƿ��Ѵ�
     *
     * @param disc_inode_id ����inode���
     */
    bool IsOpened(int disc_inode_id) {
        if(disc_inodes.find(disc_inode_id) == disc_inodes.end())
            return 0;
        return disc_inodes[disc_inode_id].mem_inode_id != -1;
    }
    /**
     * @brief ��ȡʵ��Ӳ�̵�ַ
     *
     * @param disc_inode_id ����inode���
     * @return int
     */
    int GetDiscAddress(int disc_inode_id) {
        return disc_inodes[disc_inode_id].disc_block_address;
    }
    /**
     * @brief �����ڴ�ʹ�����
     *
     * @return pair<int,int> (��ʹ�ã���)
     */
    pair<int, int> GetCapacity() {
        return make_pair((MEM_BLOCK_NUM - free_mem_block.size()) * MEM_BLOCK_SIZE, MEM_BLOCK_NUM * MEM_BLOCK_SIZE);
    }
    /**
     * @brief ����ڴ���inode��Ϣ
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
     * @brief ��ʼ���ڴ���inode��Ϣ
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
     * @brief �����ļ�����ȡ��Ӧ��Ӳ��inode�ڵ���
     *
     * @param disc_block_address �ļ�ʵ�ʵ�Ӳ�̿��ַ
     * @param user_name �������û���
     * @return int inode��� -1:Ӳ��inode����
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
     * @brief ��ȡӲ�����ݵ��ڴ���
     *
     * @param disc_inode_id Ӳ��inode���
     * @param content ����
     * @return int 0:���� -1:�Ѿ��� -2:�ڴ治�� -3:�ڴ�inode����
     */
    int OpenFile(int disc_inode_id, string content) {
        DiscIndexNode disc_inode = disc_inodes[disc_inode_id];
        // ����Ƿ��Ѿ���
        if(IsOpened(disc_inode_id))
            return -1;
        // �ж��ڴ��Ƿ񹻴�
        if(content.length() > free_mem_block.size() * MEM_BLOCK_SIZE)
            return -2;
        // ����ڴ�inode�����Ƿ��㹻
        if(free_mem_inode.empty())
            return -3;
        // �����ڴ�inode
        MemIndexNode mem_inode;
        int mem_inode_id = free_mem_inode.top();
        free_mem_inode.pop();
        mem_inode.disc_inode_id = disc_inode_id;
        mem_inode.mem_block_address = free_mem_block.top();
        free_mem_block.pop();
        mem_inodes[mem_inode_id] = mem_inode;
        // д�뵽�ڴ�
        WriteMem(mem_inode.mem_block_address, content);
        // ��¼״̬
        disc_inodes[disc_inode_id].file_size = content.length();
        disc_inodes[disc_inode_id].mem_inode_id = mem_inode_id;
        return 0;
    }
    /**
     * @brief ��ȡ�ڴ���һ���������ļ�����
     *
     * @param disc_inode_id  Ӳ��inode���
     * @return string �������ļ�����
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
     * @brief �޸��ڴ���һ���ļ�������
     *
     * @param disc_inode_id Ӳ��inode���
     * @param user ��ǰ�û�
     * @param content �������ļ�����
     * @return int 0:�ɹ� -1:�ļ�δ�� -2:Ȩ�޲��� -3:�ڴ治��
     */
    int WriteFile(int disc_inode_id, string user, string content) {
        if(!IsOpened(disc_inode_id))
            return -1;
        if(disc_inodes[disc_inode_id].creator != user)
            return -2;
        // �ж��ڴ��Ƿ񹻴�
        if(content.length() - disc_inodes[disc_inode_id].file_size > free_mem_block.size() * MEM_BLOCK_SIZE)
            return -3;
        int start_mem_address = GetMemAddress(disc_inode_id);
        // ����ڴ��е�Դ�ļ��Լ����ݿ�
        DeleteMem(start_mem_address);
        // ����ռ�����ڴ��
        free_mem_block.pop();
        WriteMem(start_mem_address, content);
        // �޸�״̬
        disc_inodes[disc_inode_id].file_size = content.size();
        return 0;
    }
    /**
     * @brief �ر��ڴ���һ���ļ�������
     *
     * @param disc_inode_id Ӳ��inode���
     * @return int 0:�ɹ� -1:�ļ�δ��
     */
    int CloseFile(int disc_inode_id) {
        if(!IsOpened(disc_inode_id))
            return -1;
        int start_mem_address = GetMemAddress(disc_inode_id);
        // ����ڴ��е�Դ�ļ��Լ����ݿ�
        DeleteMem(start_mem_address);
        // �ͷ��ڴ�inode
        mem_inodes.erase(disc_inodes[disc_inode_id].mem_inode_id);
        free_mem_inode.push(disc_inodes[disc_inode_id].mem_inode_id);
        // �޸�״̬
        disc_inodes[disc_inode_id].mem_inode_id = -1;
        return 0;
    }
    /**
     * @brief ɾ���ļ���Ӧ����
     *
     * @param disc_inode_id �ļ��Ĵ���inode���
     */
    bool DeleteFile(int disc_inode_id, string user) {
        if(IsOpened(disc_inode_id))
            CloseFile(disc_inode_id);
        // Ȩ�޼��
        if(disc_inodes[disc_inode_id].creator != user)
            return 0;
        // �ͷ�Ӳ��inode
        disc_inodes.erase(disc_inode_id);
        free_disc_inode.push(disc_inode_id);
        return 1;
    }
    /**
     * @brief �������ļ�������ת���ɵ����ַ���
     *
     */
    void Save(FILE* file) {
        // ���д���inode��
        fprintf(file, "%d\n", free_disc_inode.size());
        stack<int> temp = free_disc_inode;
        while(!temp.empty()) {
            fprintf(file, "%d ", temp.top());
            temp.pop();
        }
        fprintf(file, "\n");
        // �ļ��б�
        fprintf(file, "%d\n", disc_inodes.size());
        for(auto i: disc_inodes) {
            fprintf(file, "%d %s %d %d\n", i.first, i.second.creator.c_str(), i.second.file_size, i.second.disc_block_address);
        }
    }
    /**
     * @brief �������ļ�������ת���ɵ����ַ���
     *
     */
    string Save() {
        ostringstream data;
        // ���д���inode��
        data << free_disc_inode.size() << "\n";
        stack<int> temp = free_disc_inode;
        while(!temp.empty()) {
            data << temp.top() << " ";
            temp.pop();
        }
        data << "\n";
        // ���д���inode��
        data << disc_inodes.size() << "\n";
        for(auto i: disc_inodes) {
            data << i.first << " " << i.second.creator << " " << i.second.file_size << " " << i.second.disc_block_address << "\n";
        }
        return data.str();
    }
    /**
     * @brief ���ַ�����ʽ�����ݻָ�����
     *
     */
    void Load(string data) {
        istringstream in(data);
        int id, siz;
        // ���д���inode��
        in >> siz;
        for(int i = 0; i < siz; i++) {
            in >> id;
            free_disc_inode.push(id);
        }
        // ���д���inode��
        in >> siz;
        for(int i = 0; i < siz; i++) {
            DiscIndexNode inode;
            in >> id >> inode.creator >> inode.file_size >> inode.disc_block_address;
            inode.mem_inode_id = -1;
            disc_inodes[id] = inode;
        }
        // ��ʼ���ڴ��й�
        for(int i = 0; i < INODE_NUM; i++)
            free_mem_inode.push(i);
        for(int i = 0; i < MEM_BLOCK_NUM; i++)
            free_mem_block.push(i);
    }

    void Debug() {
        cout << endl << "�ڴ�������£�" << endl;
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

        cout << "����inode���£�" << endl;
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

        cout << "�ڴ�inode���£�" << endl;
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