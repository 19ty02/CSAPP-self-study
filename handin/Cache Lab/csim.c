#include "cachelab.h"

#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

typedef unsigned long long ull;
int v = 0, s = -1, E = -1, b = -1;
ull S, B;
const char* t;

void getParseArgs(int argc, char *argv[])
{
    int opt;
    while ((opt = getopt(argc, argv, "s:E:b:t:v")) != -1) {
        switch (opt) {
            case 's':
                s = atoi(optarg);
            case 'E':
                E = atoi(optarg);
            case 'b':
                b = atoi(optarg);
            case 't':
                t = optarg;
            case 'v':
                v = 1;
        }
    }

    assert(s >= 0 && E >= 0 && b >=0 && t != NULL);
    return;
}

/*
cache结构:
2^s 组, 每组E行,每行1个有效位,t个标记位
使用1个byte表示有效位, 8个byte表示标记位
再用8个byte表示时间戳
*/
ull count;
ull* label;
char* eff;
ull* tim;
void initCache()
{
    S = 1 << s;
    B = 1 << b;

    label = (ull *)malloc(S * (ull)E * sizeof(ull));
    eff = (char *)malloc(S * (ull)E * sizeof(char));
    tim = (ull *)malloc(S * (ull)E * sizeof(ull));
}

int nHit, nMiss, nEviction;
void handleCache(ull mem)
{
    count++;

    ull sIndex = (mem >> b) & ((1ull << s) - 1ull);
    ull tTag = mem >> (s + b);
    int start = sIndex * E;

    ull min;
    int minPos = -1;
    int emptyPos = -1;
    for (int i = 0; i < E; i++)
    {
        if (eff[start + i])
        {
            if (minPos == -1 || tim[start + i] < min)
            {
                min = tim[start + i];
                minPos = i;
            }
            if (label[start + i] == tTag)
            {
                nHit++;
                if (v)
                    printf("hit ");
                tim[start + i] = count;     // bug 注意命中后需要更新时间戳,而不是啥都不做了
                return;
            }
        }
        else
        {
            emptyPos = i;
        }
    }

    nMiss++;
    if (v)
        printf("miss ");

    int pos;
    if (emptyPos != -1)
    {
        pos = emptyPos;
    }
    else
    {
        nEviction++;
        if (v)
            printf("eviction ");
        pos = minPos;
    }

    label[start + pos] = tTag;
    eff[start + pos] = 1;
    tim[start + pos] = count;

}

int main(int argc, char *argv[])
{
    getParseArgs(argc, argv);
    initCache();

    FILE* file;
    file = fopen(t, "r");
    if (file == NULL)
    {
        perror("Error : can't open file");
        return 0;
    }

    char s1[10], s2[100];
    while (fscanf(file, "%s%s", s1, s2) == 2)
    {
        char *ptrComma = strchr(s2, ',');
        ull mem = strtoll(s2, &ptrComma, 16);
        ull sz = strtoll(ptrComma + 1, NULL, 10);
        
        if (s1[0] != 'I' && v)
            printf("%s %llx,%llx ", s1, mem, sz);

        if (s1[0] == 'L')
        {
            handleCache(mem);
        }
        if (s1[0] == 'S')
        {
            handleCache(mem);
        }
        if (s1[0] == 'M')
        {
            handleCache(mem);
            handleCache(mem);
        }

        if (s1[0] != 'I' && v)
            printf("\n");
    }

    free(label);
    free(eff);
    free(tim);
    printSummary(nHit, nMiss, nEviction);
    return 0;
}
