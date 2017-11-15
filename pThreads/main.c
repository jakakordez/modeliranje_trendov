#define HAVE_STRUCT_TIMESPEC

#include <stdio.h>
#include <pthread.h>

#include "matrix.h"

void *hi(void *arg)
{
	printf("Hello world!");
	return NULL;
}

int main(void)
{
	pthread_t nit;

	pthread_create(&nit, NULL, hi, NULL);
	pthread_join(nit, NULL);

	return 0;
}

