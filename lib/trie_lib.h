/**
 * @ «‡ø’§¿§±§¨“ä§ø§§§Œ§œŒ“Éç§«§π§´
 * @Author       : RagnaLP
 * @Date         : 2023-06-01 22:17:57
 * @LastEditTime : 2023-06-01 23:13:20
 * @Description  :
 */
#include <string>
using namespace std;
class Trie {
private:
    struct Node {
        bool end;
        int son[26];
        Node() {
            end = 0;
            for(int i = 0; i < 26; i++)
                son[i] = 0;
        }
    } tr[5010];
    int tr_cnt = 0;

public:
    void insert(string& x) {
        int u = 0, v, lenx = x.length();
        for(int i = 0; i < lenx; i++) {
            v = x[i] - 'a';
            if(!tr[u].son[v])
                tr[u].son[v] = ++tr_cnt;
            u = tr[u].son[v];
        }
        tr[u].end = 1;
    }
    bool check(string& x) {
        int u = 0, v;
        for(int i = 0; i < x.length(); i++) {
            v = x[i] - 'a';
            if(!tr[u].son[v])
                return 0;
            u = tr[u].son[v];
        }
        return tr[u].end;
    }
    string predict(string& x) {
        if(x.empty())
            return "";
        if(!tr[0].son[x[0] - 'a'])
            return "";
        string may = "";
        int u = 0, v;
        for(int i = 0; i < x.length(); i++) {
            v = x[i] - 'a';
            if(!tr[u].son[v]) {
                break;
            }
            may += x[i];
            u = tr[u].son[v];
        }
        if(tr[u].end == 0) {
            while(1) {
                v = -1;
                for(int i = 0; i < 26; i++) {
                    if(tr[u].son[i]) {
                        v = i;
                        break;
                    }
                }
                if(v == -1)
                    break;
                may += v + 'a';
                u = tr[u].son[v];
            }
        }
        return may;
    }
};