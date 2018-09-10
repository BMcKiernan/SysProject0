#include <stdio.h>
#include <stdlib.h>

static int error;

int geterror(){
	return error;
}


char* readline(FILE* input_file, char* p, int size) {
	int n = 0;
	int ch;
	char *p2, *q, *q2;
	while ((ch = getc(input_file)) != '\n' && ch != EOF) {
		p[n++] = ch;
		if (n == (size - 1)) {
			size = size * 2;
			p2 = malloc((size) * sizeof(char));
			if (p2 == NULL) {
				printf("Allocation of memory failed. File: %s, Line: %d\n",
				__FILE__, __LINE__);
				error = 1;
				return p;
			}
			p[n] = '\0';
			q = p;
			q2 = p2;
			while ((*q2++ = *q++));
			free(p);
			p = p2;
		}
	}
	p[n] = '\0';
	return p;
}
