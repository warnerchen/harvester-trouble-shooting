#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STEP 1<<20
#define STEP_1K 1<<10
#define STEP_64K 1<<16

int exhaust_memory(unsigned int sz) {
	char* tmp=NULL;
	unsigned int all = 0;
	while(1) {
		tmp = (char*)malloc(sz);
		if  (tmp == NULL) {
			printf("allocate memory failed, return\n");
			return 1;
		}
		memset(tmp, '1', sz);
		all += sz >> 10;
		printf("allocated memory: %10u kB, %10u MB\n", all, all>>10);
	}

	return 0;
}

int main(int argc, char* argv[]) {

  unsigned int sz=STEP;

  if (argc > 1) {
  	if (*argv[1] == '1') {
  		sz = STEP_1K;
  	} else if (*argv[1] == '2') {
  		sz = STEP_64K;
  	}
  }

  printf("this is a simple test to exhaust memory, allocate unit=%u bytes\n", sz);

  return exhaust_memory(sz);
}

// usage:
// gcc -o memtest memtest.c
// chmod +x ./memtest
// ./memtest