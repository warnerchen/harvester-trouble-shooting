#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define STEP 1<<20 // 1M
#define DEFAULT_MEM 100 // 100M

int hold_memory(unsigned int sz) {
	char* tmp=NULL;
	int i = 0;
	for (i=0; i < sz; i++) {
		tmp = (char*)malloc(STEP);
		if  (tmp == NULL) {
			printf("allocate memory failed at loop %i, return\n", i);
			return 1;
		}

		memset(tmp, '1', STEP);
	}
	printf("allocated %u M bytes, enter sleeping\n", sz);

	// no return, until application is killed
	while (1) {
		sleep(100);
	}

	return 0;
}

int main(int argc, char* argv[]) {

  unsigned int sz = DEFAULT_MEM*STEP;

  unsigned int i = DEFAULT_MEM;

  if (argc > 1) {
  	i = atoi(argv[1]);
  	// max 100G
  	if (i > 0 && i < 102400) {
  		sz = i;
  	}
  }

  printf("this is a simple test to hold memory %u M bytes\n", sz);

  return hold_memory(sz);
}

// usage:
// gcc -o memhold memhold.c
// chmod +x ./memhold
// ./memhold