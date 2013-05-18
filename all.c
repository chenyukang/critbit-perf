#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define uint8 uint8_t
#define uint32 uint32_t

static int valid(char c) {
    if((c>='a' && c<='z') ||
       (c>='A' && c<='Z'))
        return 1;
    return 0;
}

int read_all() {
    FILE* fp = 0;
    char* p;
    char buf[64];
    int len;
    int i, k;
    int time;
    memset(buf, 0, sizeof(buf));
    fp = fopen("/usr/share/dict/words", "r");
    while(fgets(buf, 64, fp) != 0) {
        len = strlen(buf);
        buf[len-1] = 0;
        len--;
        for(i=0, k=0; i<len; i++) {
            if(valid(buf[i])) {
                buf[k++] = buf[i];
            }
        }
        buf[k] = 0;
        p = strdup(buf);
    }
    printf("finished\n");
    fclose(fp);
    while(1) {
        ;
    }
    return 0;
}

int main() {
    read_all();
    return 0;
}
