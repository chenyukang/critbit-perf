#define _POSIX_C_SOURCE 200112
#define uint8 uint8_t
#define uint32 uint32_t

#if 0
#ifndef TIMES
#define TIMES 2
#endif
#endif

#include <stdint.h> 
#include <string.h> 
#include <stdlib.h> 
#include <stdio.h>
#include <sys/types.h> 
#include <errno.h>
#include <assert.h>

typedef struct{
    void* child[2];
    uint32 byte;
    uint8 otherbits;
}critbit0_node;

typedef struct{
    void* root;
}critbit0_tree;

int critbit0_contains(critbit0_tree*t, const char* u) {
    const uint8* ubytes= (void*)u;
    const size_t ulen= strlen(u);
    uint8* p= t->root;
    if(!p) return 0;
    while( 1 & (intptr_t)p ){
        critbit0_node* q = (void*)(p-1);
        uint8 c = 0;
        if(q->byte < ulen) c = ubytes[q->byte];
        const int direction= (1+(q->otherbits|c))>>8;
        p = q->child[direction];
    }
    return 0 == strcmp(u, (const char*)p);
}

int critbit0_insert(critbit0_tree* t, const char*u) {
    const uint8* const ubytes= (void*)u;
    const size_t ulen = strlen(u);
    uint8* p = t->root;
    if(!p){
        char* x;
        //if(posix_memalign((void**)&x, sizeof(void*), ulen+1))
        //return 0;
        x = malloc(ulen+1);
        memcpy(x, u, ulen+1);
        t->root = x;
        return 2;
    }

    while(1&(intptr_t) p){
        critbit0_node* q = (void*)(p-1);
        uint8 c= 0;
        if(q->byte < ulen)
            c= ubytes[q->byte];
        const int direction = (1+(q->otherbits|c))>>8;
        p = q->child[direction];
    }

    uint32 newbyte;
    uint32 newotherbits;

    for(newbyte= 0; newbyte<ulen; ++newbyte){
        if(p[newbyte] != ubytes[newbyte]){
            newotherbits = p[newbyte]^ubytes[newbyte];
            goto different_byte_found;
        }
    }

    if(p[newbyte] != 0){
        newotherbits = p[newbyte];
        goto different_byte_found;
    }
    return 1;

different_byte_found:
    newotherbits |= newotherbits>>1;
    newotherbits |= newotherbits>>2;
    newotherbits |= newotherbits>>4;
    newotherbits = (newotherbits & ~(newotherbits>>1))^255;
    uint8 c = p[newbyte];
    int newdirection = (1+(newotherbits|c))>>8;
    critbit0_node* newnode;
    if(posix_memalign((void**)&newnode, sizeof(void*), sizeof(critbit0_node)))
        return 0;

    char*x;
    if(posix_memalign((void**)&x, sizeof(void*), ulen+1)){
        free(newnode);
        return 0;
    }
    memcpy(x,ubytes,ulen+1);

    newnode->byte = newbyte;
    newnode->otherbits = newotherbits;
    newnode->child[1-newdirection] = x;

    void** wherep = &t->root;
    for(;;){
        uint8* p = *wherep;
        if(!(1&(intptr_t)p)) break;
        critbit0_node*q = (void*)(p-1);
        if(q->byte > newbyte) break;
        if(q->byte == newbyte && q->otherbits > newotherbits) break;
        uint8 c = 0;
        if(q->byte < ulen) c = ubytes[q->byte];
        const int direction = (1+(q->otherbits|c))>>8;
        wherep = q->child+direction;
    }

    newnode->child[newdirection] = *wherep;
    *wherep = (void*)(1+(char*)newnode);
    return 2;
}

int critbit0_delete(critbit0_tree*t, const char*u){
    const uint8* ubytes = (void*)u;
    const size_t ulen = strlen(u);
    uint8* p = t->root;
    void** wherep = &t->root;
    void** whereq = 0;
    critbit0_node* q = 0;
    int direction = 0;
    if(!p) return 0;

    while(1&(intptr_t)p){
        whereq= wherep;
        q= (void*)(p-1);
        uint8 c= 0;
        if(q->byte<ulen)c= ubytes[q->byte];
        direction= (1+(q->otherbits|c))>>8;
        wherep= q->child+direction;
        p= *wherep;
    }

    if(0!=strcmp(u,(const char*)p))return 0;
    free(p);
    if(!whereq){
        t->root= 0;
        return 1;
    }

    *whereq= q->child[1-direction];
    free(q);
    return 1;
}


static void traverse(void*top){
    uint8*p= top;

    if(1&(intptr_t)p){
        critbit0_node*q= (void*)(p-1);
        traverse(q->child[0]);
        traverse(q->child[1]);
        free(q);
    }else{
        free(p);
    }
}

void critbit0_clear(critbit0_tree*t) {
    if(t->root)traverse(t->root);
    t->root= NULL;
}


static int allprefixed_traverse(uint8*top,
                     int(*handle)(const char*,void*),void*arg){
    int direction;
    if(1&(intptr_t)top){
        critbit0_node*q= (void*)(top-1);
        for(direction= 0;direction<2;++direction)
            switch(allprefixed_traverse(q->child[direction],handle,arg)){
            case 1:break;
            case 0:return 0;
            default:return-1;
            }
        return 1;
    }

    return handle((const char*)top, arg);
}

int
critbit0_allprefixed(critbit0_tree*t,const char*prefix,
                     int(*handle)(const char*,void*),void*arg){
    const uint8*ubytes= (void*)prefix;
    const size_t ulen= strlen(prefix);
    uint8* p= t->root;
    uint8* top= p;
    size_t i = 0;
    
    if(!p) return 1;
    while(1&(intptr_t)p){
        critbit0_node* q = (void*)(p-1);
        uint8 c = 0;
        if(q->byte < ulen) c = ubytes[q->byte];
        const int direction = (1+(q->otherbits|c))>>8;
        p = q->child[direction];
        if(q->byte < ulen)
            top = p;
    }

    for(i= 0; i<ulen; ++i){
        if(p[i] != ubytes[i])
            return 1;
    }

    return allprefixed_traverse(top, handle, arg);
}


static int valid(char c) {
    if((c>='a' && c<='z') ||
       (c>='A' && c<='Z'))
        return 1;
    return 0;
}

int critbit0_test() {
    critbit0_tree root = {0};
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
            critbit0_insert(&root, buf);
            if(critbit0_contains(&root, buf) == 0){
                printf("error: %s\n", buf);
            }
        }
        if(critbit0_contains(&root, "yukang") != 0) {
            printf("error: yukang\n");
        }
        fclose(fp);
    }
    critbit0_clear(&root);
    return 0;
}

int main() {
    return critbit0_test();
#if 0
    critbit0_tree root = { 0 };
    critbit0_insert(&root, "aa");
    critbit0_insert(&root, "bb");
    critbit0_insert(&root, "aaA");
    if(critbit0_contains(&root, "aaa")) {
        printf("yes\n");
    }
    else {
        printf("no\n");
    }
#endif
    return 0;
}
