#include<time.h>
#include"miracl.h"
#include"mirdef.h"
void strip(char* name);   //分离文件后缀
void hashing(FILE* fp, big hash);  //计算消息的摘要