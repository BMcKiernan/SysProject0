#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sorter.h"

static int error = 0;
static record* head = NULL;

enum {
	NULL_TOK = -1, COMMA = 0, QUOTE = 1, INT = 3, DOUBLE = 4, STRING = 5,
};

int main(int argc, char *argv[]) {
	int len, i, strsize, size = 512;
	char *line, *token, *pholder, *quote, *tokensleft, *tholder, *copy;
	FILE *fd;

	int typearr[] = { STRING, STRING, INT, INT, INT, INT, STRING, INT, INT,
			STRING, STRING, STRING, INT, INT, STRING, INT, STRING, STRING, INT,
			STRING, STRING, STRING, INT, INT, INT, DOUBLE, DOUBLE, INT };
	/*
	 * To Do: Need to add error checking in tokenizing loop for readline (if(error)) and mallocs
	 */

	fd = fopen("movie.csv", "r");
	if (fd == NULL) {
		printf("Couldn't open file\n");
		return 1;
	}

	line = malloc(sizeof(char) * size);
	if (line == NULL) {
		printf("Couldn't allocate memory for 'line'\n");
		fclose(fd);
	}
	line = readline(fd, line, size);
	if (error) {
		printf("Could not reallocate memory in readline\n");
		fclose(fd);
		free(line);
	}
	pholder = line;
	//get size of string from readline
	while (*pholder)
		pholder++;
	len = pholder - line;
	//reset pholder to line for later
	pholder = line;
	//Loop while the length of the line read is greater than 1 (FILE IS NOT EOF)
	while (len > 1) {
		record* new_row = malloc(sizeof(record));
		i = 0;
		token = strsep(&line, "\"");
		//Line is NULL if there is no quoted token containing an inner comma
		if (line == NULL) {
			line = token; //reset line back to itself after strsep made it null

			while ((token = strsep(&line, ",")) != NULL) {
				tholder = token;
				while (*tholder) //get size of token
					tholder++;
				strsize = tholder - token;

				if (strsize == 0) { //NULL TOKEN
					new_row->tokens[i] = malloc(sizeof(char) * 2); //to simplify freeing later
					strcpy(new_row->tokens[i], "!\0");
					new_row->tokenmeta[i] = NULL_TOK;
				} else {
					new_row->tokens[i] = malloc(sizeof(char) * strsize + 1);
					copy = new_row->tokens[i];
					tholder = token;
					while ((*copy++ = *tholder++));
					//*copy = '\0';
					new_row->tokenmeta[i] = COMMA;
				}
				i++;
			}
		} else {
			//Line has quotation delimited token containing comma
			quote = strsep(&line, "\"");
			tokensleft = token;
			line++; //CHECK
			while ((token = strsep(&tokensleft, ",")) != NULL) {
				tholder = token;
				while (*tholder) //get size of token
					tholder++;
				strsize = tholder - token;

				if (strsize == 0) { //NULL TOKEN
					new_row->tokens[i] = malloc(sizeof(char) * 2); //to simplify freeing later
					strcpy(new_row->tokens[i], "!\0");
					new_row->tokenmeta[i] = NULL_TOK;
				} else {
					new_row->tokens[i] = malloc(sizeof(char) * strsize + 1);
					copy = new_row->tokens[i];
					tholder = token;
					while ((*copy++ = *tholder++));
					//copy = '\0';
					new_row->tokenmeta[i] = COMMA;
				}
				i++;
			}
			tholder = quote;
			while (*tholder)
				tholder++;
			strsize = tholder - quote;
			new_row->tokens[i] = malloc(sizeof(char) * strsize + 1);
			copy = new_row->tokens[i];
			tholder = quote;
			while ((*copy++ = *tholder++));
			//copy = '\0';
			new_row->tokenmeta[i] = QUOTE;
			i++;
			while ((token = strsep(&line, ",")) != NULL) {
				tholder = token;
				while (*tholder) //get size of token
					tholder++;
				strsize = tholder - token;

				if (strsize == 0) { //NULL TOKEN
					new_row->tokens[i] = malloc(sizeof(char) * 2); //to simplify freeing later
					strcpy(new_row->tokens[i], "!\0");
					new_row->tokenmeta[i] = NULL_TOK;
				} else {
					new_row->tokens[i] = malloc(sizeof(char) * (strsize + 1));
					copy = new_row->tokens[i];
					tholder = token;
					while ((*copy++ = *tholder++));
					//copy = '\0';
					new_row->tokenmeta[i] = COMMA;
				}
				i++;
			}
		}
		insert(new_row);
		//reset line to the begnning of the allocated memory
		line = pholder;


		//Using length of last token -- Memory will already be allocated AT LEAST that much
		line = readline(fd, line, len);
		//reset pholder to get  new line length
		pholder = line;
		//get size of the new string from readline for check in outer while loop
		while (*pholder)
			pholder++;
		len = pholder - line;
		//reset pholder to line for later
		pholder = line;
	}
    free_list();
	free(line);
	fclose(fd);
	return 0;
}

void insert(record* newrow){
	newrow->next = head;
	head = newrow;
}

void free_list() {
	if ((head == NULL)) {
		printf("WARNING: free_list called with head==NULL");
		return;
	}
	record* lead = head;
	record* willy;
	int i = 0;
	while (lead != NULL) {
		willy = lead;
		lead = lead->next;
		for (i = 0; i < 28; i++)
			free(willy->tokens[i]);
		free(willy);
	}
	head = NULL;
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

