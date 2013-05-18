#include <set>
#include <string>
#include <iostream>
using namespace std;

#ifndef TIMES
#define TIMES 20
#endif

std::set<string> dict;

static int valid(char c) {
    if((c>='a' && c<='z') ||
       (c>='A' && c<='Z'))
        return 1;
    return 0;
}

int test_set() {
    FILE* fp = 0;
    char* p;
    char buf[64];
    int len;
    int i, k;
    int time;
    for(time=0; time<TIMES; time++) {
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
            dict.insert(buf);
            if(dict.find(buf) == dict.end()) {
                printf("error: %s\n", buf);
            }
        }
        if(dict.find("yukang") != dict.end()) {
            printf("error: yukang");
        }
        fclose(fp);
    }
    return 0;
}

int main() {
    return test_set();
}



