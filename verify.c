#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "miracl.h"
#include"common_func.h"

void verify(char signFile[], char* publicKeyFile) //签名信息在  签名消息同名的.ecs文件中 所以此处只需提供签名文件和公钥即可。
{
	FILE* fp;
	int ep;
	epoint* g, * publc;
	char ifname[50], ofname[50];
	strcpy(ifname, signFile);
	big a, b, p, q, x, y, v, u1, u2, r, s, hash;
	miracl* mip;


	fp = fopen("parameter.ecs", "rt");
	if (fp == NULL)
	{
		printf("file parameter.ecs does not exist\n");
		return 0;
	}

	mip = mirsys(1000, 16);   /* Use Hex Internally */
	a = mirvar(0);
	b = mirvar(0);
	p = mirvar(0);
	q = mirvar(0);
	x = mirvar(0);
	y = mirvar(0);
	v = mirvar(0);
	u1 = mirvar(0);
	u2 = mirvar(0);
	s = mirvar(0);
	r = mirvar(0);
	hash = mirvar(0);

	innum(p, fp);
	innum(a, fp);
	innum(b, fp);
	innum(q, fp);
	innum(x, fp);
	innum(y, fp);

	fclose(fp);

	ecurve_init(a, b, p, MR_PROJECTIVE);  /* initialise curve */
	g = epoint_init();
	epoint_set(x, y, 0, g);
	if (!epoint_set(x, y, 0, g)) /* initialise point of order q */
	{
		printf("1. Problem - point (x,y) is not on the curve\n");
		exit(0);
	}

	/* get public key of signer */
	fp = fopen(publicKeyFile, "rt");
	if (fp == NULL)
	{
		printf("file public.ecs does not exist\n");
		return 0;
	}
	fscanf(fp, "%d", &ep);
	innum(x, fp);
	fclose(fp);

	publc = epoint_init();
	if (!epoint_set(x, x, ep, publc))  /* decompress */
	{
		printf("1. Not a point on the curve\n");
		return 0;
	}

	strcpy(ofname, ifname);
	strip(ofname);
	strcat(ofname, ".ecs");
	if ((fp = fopen(ifname, "rb")) == NULL)
	{ /* no message */
		printf("Unable to open file %s\n", ifname);
		return 0;
	}
	hashing(fp, hash);
	fclose(fp);
	fp = fopen(ofname, "rt");
	if (fp == NULL)
	{ /* no signature */
		printf("signature file %s does not exist\n", ofname);
		return 0;
	}
	innum(r, fp);
	innum(s, fp);
	fclose(fp);
	if (mr_compare(r, q) >= 0 || mr_compare(s, q) >= 0)
	{
		printf("Signature is NOT verified\n");
		return 0;
	}
	xgcd(s, q, s, s, s);
	mad(hash, s, s, q, q, u1);
	mad(r, s, s, q, q, u2);

	ecurve_mult2(u2, publc, u1, g, g);

	epoint_get(g, v, v);
	divide(v, q, q);
	if (mr_compare(v, r) == 0)
		printf("After checking.The message is form Alice\n");
	else                 
		printf("Signature is NOT verified.The message is not from Alice\n");
}
