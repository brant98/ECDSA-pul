#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "miracl.h"
#include "creat_key.h"
#include "sign.h"
#include "verify.h"

int main()
{

	clock_t start, finish;
	start = clock();

	for (int i = 0; i < 1000; i++) {
	creat_key("parameter.ecs", "publickey.ecs", "privatekey.ecs");
	sign("pulsecret.txt", "privatekey.ecs");
	verify("pulsecret.txt", "publickey.ecs");

	}


	printf("Test of this algorithm finished\n");
	finish = clock();

	printf("Start at  %f s\n", (double)start / CLOCKS_PER_SEC);
	printf("End at %f s\n", (double)finish / CLOCKS_PER_SEC);

	printf("1000 times tests  used %f seconds in total.\n", (double)difftime(finish, start) / CLOCKS_PER_SEC);
	printf("The algorithm runs once used %f seconds on average.\n", (double)difftime(finish, start) / CLOCKS_PER_SEC / 1000);
	return 0;
}