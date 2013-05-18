#ifndef _TRIE_H__
#define _TRIE_H__

#include <stdio.h>

#define CNT 52

struct Trie {
    int leaf;
    struct Trie* children[CNT];//the following point
};

struct Trie* TrieNew();
int TrieDel(struct Trie*);
int TrieInsert(struct Trie*, char* , int);
int TrieFind(struct Trie*, char*, int);

#endif

