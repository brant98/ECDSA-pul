#include <stdio.h>
#include "miracl.h"
#include <stdlib.h>

void strip(char* name)//��ȡ�ļ���
{
    int i;
    for (i = 0; name[i] != '\0'; i++)
    {
        if (name[i] != '.') continue;
        name[i] = '\0';
        break;
    }
}

void hashing(FILE* fp, big hash)//������Ϣ�Ĺ�ϣժҪ
{ 
    char h[20];
    int ch;
    sha sh;
    shs_init(&sh);
    while ((ch = fgetc(fp)) != EOF) shs_process(&sh, ch);
    shs_hash(&sh, h);
    bytes_to_big(20, h, hash);
}