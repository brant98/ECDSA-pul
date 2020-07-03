#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "miracl.h"
#include"common_func.h"

void sign(char signFile[],char* privateKeyFile)  //签名消息存入 签名消息文件名.ecs文件中
{
    FILE* fp;
    char ifname[50], ofname[50];
    strcpy(ifname, signFile);
    big a, b, p, q, x, y, d, r, s, k, hash;
    epoint* g;
    miracl* mip = mirsys(1000, 16);   /* Use Hex internally */
    time_t seed;
    time(&seed);
    irand(seed);  //随机数种子
    int bits;
    /* get public data */
    fp = fopen("parameter.ecs", "rt");
    if (fp == NULL)
    {
        printf("file common.ecs does not exist\n");
        return 0;
    }
    a = mirvar(0);
    b = mirvar(0);
    p = mirvar(0);
    q = mirvar(0);
    x = mirvar(0);
    y = mirvar(0);
    d = mirvar(0);
    r = mirvar(0);
    s = mirvar(0);
    k = mirvar(0);
    hash = mirvar(0);

    innum(p, fp);     /* modulus        */
    innum(a, fp);     /* curve parameters */
    innum(b, fp);
    innum(q, fp);     /* order of (x,y) */
    innum(x, fp);     /* (x,y) point on curve of order q */
    innum(y, fp);
    fclose(fp);

    ecurve_init(a, b, p, MR_PROJECTIVE);  /* initialise curve */
    g = epoint_init();

    if (!epoint_set(x, y, 0, g)) /* initialise point of order q */
    {
        printf("1. Problem - point (x,y) is not on the curve\n");
        exit(0);
    }

    /* calculate r - this can be done offline,
       and hence amortized to almost nothing   */
    bigrand(q, k);

    ecurve_mult(k, g, g);      /* see ebrick.c for method to speed this up */

    epoint_get(g, r, r);
    divide(r, q, q);

    /* get private key of signer */
    fp = fopen(privateKeyFile, "rt");
    if (fp == NULL)
    {
        printf("file private.ecs does not exist\n");
        return 0;
    }
    innum(d, fp);
    fclose(fp);

    strcpy(ofname, ifname);
    strip(ofname);
    strcat(ofname, ".ecs");
    if ((fp = fopen(ifname, "rb")) == NULL)
    {
        printf("Unable to open file %s\n", ifname);
        return 0;
    }
    hashing(fp, hash);
    fclose(fp);
  
    /*计算s */
    xgcd(k, q, k, k, k);
    mad(d, r, hash, q, q, s);
    mad(s, k, k, q, q, s);
    fp = fopen(ofname, "wt");
    otnum(r, fp);
    otnum(s, fp);
    fclose(fp);
    printf("The message is signed by Alice.\n");
}