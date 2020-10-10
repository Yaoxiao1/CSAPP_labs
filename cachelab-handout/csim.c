#include "cachelab.h"
#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

typedef struct {
    int valid, tag, LRU_cnt;
} cache_line;

int num_hit, num_miss, num_evict;
int opt, opt_help, opt_verbose, s, E, b;
int S;
cache_line** cache;

void update(unsigned address){
    unsigned s_addr, t_addr;
    int max_cnt = 0, replace_e = -1;
    s_addr = (address>>b)&((-1U)>>(32-s));
    t_addr = address>>(b+s);
    for(int i=0;i<E; ++i)
    {
        if(cache[s_addr][i].tag==t_addr)
        {
            ++num_hit;
            cache[s_addr][i].LRU_cnt=0;
            return;
        }
    }
    for (int i=0;i<E; ++i)
    {
        if (cache[s_addr][i].valid==0)
        {
            ++num_miss;
            cache[s_addr][i].valid = 1;
            cache[s_addr][i].tag = t_addr;
            cache[s_addr][i].LRU_cnt = 0;
            return;
        }
    }

    for (int j=0;j<E;++j)
    {
        if (cache[s_addr][j].LRU_cnt > max_cnt)
        {
            max_cnt = cache[s_addr][j].LRU_cnt;
            replace_e = j;
        }
    }
    
    ++num_miss;
    ++num_evict;
    cache[s_addr][replace_e].tag = t_addr;
    cache[s_addr][replace_e].LRU_cnt = 0;
    return;

}


void update_time(){
    for(int i=0;i<S;++i)
    {
        for(int j=0;j<E;++j)
        {
            if(cache[i][j].valid==1) cache[i][j].LRU_cnt += 1;       
        }
    }
}


int main(int argc, char** argv)
{
    char filename[100];
    char buffer[1000];
    num_hit = num_miss = num_evict = 0;
    while(-1 != (opt = getopt(argc, argv, "hvs:E:b:t:")))
    {
        switch(opt){
            case 'h':
                opt_help = 1;
                break;
            case 'v':
                opt_verbose = 1;
                break;            
            case 's':
                s = atoi(optarg);
                break;
            case 'E':
                E = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
                break;
            case 't':
                strcpy(filename, optarg);
                break;
            default:
                printf("wrong argument\n");
                break;
        }
    }
    S = 1<<s;  //2^s
    cache = (cache_line**)malloc(sizeof(cache_line*) * S);
    for(int i=0;i<S;++i)
    {
        cache[i] = (cache_line*)malloc(sizeof(cache_line) * E);
    }
    for(int i=0;i<S;++i)
    {
        for(int j=0;j<E;++j)
        {
            cache[i][j].valid = 0;
            cache[i][j].tag = -1;
            cache[i][j].LRU_cnt = -1;
        }
    }
    FILE* pfile;
    pfile = fopen(filename, "r");
    if (pfile==NULL) {
        fprintf(stderr, "wrong file!\n");
        exit(-1);
    }
    char identifier;
    unsigned address;
    int size;
    while (fgets(buffer, 1000, pfile)) //读数据的方式很关键, fscanf不好用
    {
        sscanf(buffer, " %c %xu,%d", &identifier, &address, &size);
        switch(identifier){
            case 'M':
                update(address);
                update(address);
                break;
            case 'L':
                update(address);
                break;
            case 'S':
                update(address);
                break;
        }
        update_time();
    }
    for(int i=0;i<S;++i) free(cache[i]);
    free(cache);
    fclose(pfile);
    printSummary(num_hit, num_miss, num_evict);
    return 0;
}
