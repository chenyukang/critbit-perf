#include "trie.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

struct Trie* TrieNew() {
    int i;
    struct Trie *root=(struct Trie*)malloc(sizeof(struct Trie));
    root->leaf = 0;
    for(i=0; i<CNT; i++)
        root->children[i] = 0;
    return root;
}

int TrieDel(struct Trie* root) {
    int i;
    for(i=0; i<CNT; i++) {
        if(root->children[i]) {
            TrieDel(root->children[i]);
            free(root->children[i]);
            root->children[i] = 0;
        }
    }
    free(root->children);
    free(root);
    return 0;
}

static int valid(char c) {
    if((c>='a' && c<='z') ||
       (c>='A' && c<='Z'))
        return 1;
    return 0;
}
        
static int idx(char c) {
    assert(valid(c));
    return c>='a' ? c-'a' : c-'A'+ 26;
}

int TrieFind(struct Trie* root, char* s, int len) {
    int i;
    struct Trie* p = root;
    for(i=0; p != 0 && i<len; i++) {
        int id = idx(s[i]);
        if(p->children[id] == 0) {
            return 0;
        } else {
            p = p->children[id];
        }
    }
    return p->leaf == 1;
}

int TrieInsert(struct Trie* root, char* s, int len) {
    int i;
    struct Trie* p = root;
    for(i=0; i<len; i++) {
        int id = idx(s[i]);
        if(id == -1) {
            printf("error: %s\n", s);
            assert(0);
        }
        if(p->children[id] == 0) {
            p->children[id] = (struct Trie*)malloc(sizeof(struct Trie));
            memset(p->children[id], 0, sizeof(struct Trie));
        }
        p = p->children[id];
    }
    if(p->leaf == 0){
        p->leaf = 1;
        return 1;
    }
    return 0;
}

int TrieTest() {
    struct Trie* root = TrieNew();
    FILE* fp = 0;
    char buf[64];
    int len;
    int i, k;
    int time;
    for(time=0; time<TIMES; time++) {
        memset(buf, 0, sizeof(buf));
        fp = fopen("/usr/share/dict/words", "r");
        while(fgets(buf, 64, fp) != 0) {
            len = strlen(buf);
            assert(buf[len-1] == '\n');
            buf[len-1] = 0;
            len--;
            for(i=0, k=0; i<len; i++) {
                if(valid(buf[i])) {
                    buf[k++] = buf[i];
                }
            }
            buf[k] = 0;
            TrieInsert(root, buf, k);
#if 1
            if(TrieFind(root, buf, k) != 1) {
                printf("error: %s\n", buf);
            }
#endif
        }
        if(TrieFind(root, "yukang", 6) != 0) {
            printf("error yukang");
        }
        fclose(fp);
    }
    return 0;
}

int main() {
    return TrieTest();
}
