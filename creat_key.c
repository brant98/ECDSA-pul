#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "miracl.h"
void creat_key(char* parameterFile,char* publicKeyFile,char* privateKeyFile)
{
	FILE* fp;
	int ep;
	epoint* g, * w;
	big a, b, p, q, x, y, d;

	miracl* mip = mirsys(1000, 16);
	time_t seed;
	time(&seed);
	irand(seed);  //���������

	a = mirvar(0);
	b = mirvar(0);
	p = mirvar(0);  //192bits
	q = mirvar(0);
	x = mirvar(0);
	y = mirvar(0);
	d = mirvar(0);

	fp = fopen(parameterFile, "rt"); //��Բ���ߵ���ز��������parameter.ecs��
	if (fp == 0)
	{
		printf("Failed to open this file��\n");
		exit(1);
	}
	mip->IOBASE = 16;
	cinnum(p, fp);
	cinnum(a, fp);
	cinnum(b, fp);
	cinnum(q, fp);
	cinnum(x, fp);
	cinnum(y, fp);
	fclose(fp);
	ecurve_init(a, b, p, MR_PROJECTIVE);  //��ʼ����Բ����

	g = epoint_init();
	w = epoint_init();

	if (!epoint_set(x, y, 0, g)) /* initialise point of order q */
	{
		printf("1. Problem - point (x,y) is not on the curve\n");
		exit(0);
	}

	ecurve_mult(q, g, w);
	if (!point_at_infinity(w))
	{
		printf("2. Problem - point (x,y) is not of order q\n");
		exit(0);
	}

	/* generate public and private keys */
	bigrand(q, d);    //0<=d<q  
	ecurve_mult(d, g, g);

	ep = epoint_get(g, x, x);    /* compress point */

	printf("public key = %d ", ep); //�����Կ
	cotnum(x, stdout);

	fp = fopen(publicKeyFile, "wt"); //����˽Կ�ֱ����������ļ��� �������ǩ����֤
	fprintf(fp, "%d ", ep);
	cotnum(x, fp);
	fclose(fp);

	fp = fopen(privateKeyFile, "wt");
	cotnum(d, fp);
	fclose(fp);
	printf("The public key and private key is created!\n");
}