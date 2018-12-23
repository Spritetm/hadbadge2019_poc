#include <stdio.h>

int main(int argc, char **argv) {
	printf("Main fn reached.\n");
	while(1) {
		char *s=calloc(1000, 1);
		printf("Malloc: %p\n", s);
	}
}