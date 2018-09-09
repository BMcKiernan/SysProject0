#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sorter.h"
#include "mergesort.h"
#include "getcolumns.h"


static record* head = NULL;
static int comparetype, col, error = 0;

enum {
	NULL_TOK = -1, COMMA = 1, QUOTE = 2, INT = 3, DOUBLE = 4, STRING = 5,
};

int main(int argc, char *argv[]) {
	int len, col, i,  opt, h, strsize, size = 512;
	char *line, *token, *pholder, *quote, *tokensleft, *copy, *colheader;
	FILE *fd;
	h = 0;

	while ((opt = getopt(argc, argv, "c:")) != -1) {
		switch (opt) {
		case 'c': colheader = optarg; 	break;
		default:
			fprintf(stderr, "Usage: %s [-c colname]\n", argv[0]);
			return -1;
		}
	}
	if (optind == 1) {
		fprintf(stderr, "Usage: %s [-c colname]\n", argv[0]);
		return -1;
	}

	int typearr[] = { STRING, STRING, INT, INT, INT, INT, STRING, INT, INT,
			STRING, STRING, STRING, INT, INT, STRING, INT, STRING, STRING, INT,
			STRING, STRING, STRING, INT, INT, INT, DOUBLE, DOUBLE, INT };

	/*
	 * To Do: Need to add error checking in tokenizing loop for readline (if(error)) and mallocs
	 */

	fd = fopen("movie_metadata.csv", "r");
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

	record* headers = malloc(sizeof(record));
	col = getcolumns(line, headers, optarg);
	if(col == -1){
		printf("%s is not a valid column name.\n", optarg);
		free(line);
		for (i = 0; i < 28; i++)
			free(headers->tokens[i]);
		free(headers);
		fclose(fd);
		return -1;
	} else
		comparetype = typearr[col];

	len = strlen(line) + 1;
    line = readline(fd, line, len);
    if(error){
    	free(line);
    	for(i = 0; i < 28; i ++)
    		free(headers->tokens[i]);
    	free(headers);
    	fclose(fd);
    	printf("readline() failed\n");
    	return -1;
    }

/*
	//DEBUGGING CODE
	copy = malloc(sizeof(char)*strlen(line)+1);
	strcpy(copy, line);
	//DEBUGGING CODE
*/

	pholder = line;
	//get size of string from readline
	len = strlen(pholder) + 1;
	//Loop while the length of the line read is greater than 1 (FILE IS NOT EOF)
	while (len > 1) {
		record* new_row = malloc(sizeof(record));
		i = 0;
		token = strsep(&line, "\"");
		//Line is NULL if there is no quoted token containing an inner comma
		if (line == NULL) {
			line = token; //reset line back to itself after strsep made it null

			while (( token = strsep(&line, ",")) != NULL ) {


				strsize = strlen(token) ;
				if (strsize == 0) { //NULL TOKEN
					new_row->tokens[i] = malloc(sizeof(char) * 2); //to simplify freeing later
					strcpy(new_row->tokens[i], "!\0");
					new_row->tokenmeta[i] = NULL_TOK;
				} else {
					new_row->tokens[i] = malloc(sizeof(char) * strsize + 1);
					strcpy(new_row->tokens[i], token);
					new_row->tokenmeta[i] = COMMA;
				}
				i++;
			}
		} else {
			//Line has quotation delimited token containing comma
			quote = strsep(&line, "\"");
			tokensleft = token;
			line++; //CHECK
			while ((token = strsep(&tokensleft, ",")) != NULL ) {
                //Potential Solution to problem may be that strsep returns token for "\0" or something like that
				strsize = strlen(token);
				if (strsize == 0) { //NULL TOKEN
					new_row->tokens[i] = malloc(sizeof(char) * 2); //to simplify freeing later
					strcpy(new_row->tokens[i], "!\0");
					new_row->tokenmeta[i] = NULL_TOK;
				} else {
					new_row->tokens[i] = malloc(sizeof(char) * strsize + 1);
					strcpy(new_row->tokens[i], token);
					new_row->tokenmeta[i] = COMMA;
				}
				i++;
			}

			strsize = strlen(quote);
			new_row->tokens[i] = malloc(sizeof(char) * strsize + 1);
			strcpy(new_row->tokens[i], quote);
			new_row->tokenmeta[i] = QUOTE;
			i++;

			//NEED TO DO ERROR CHECKING HERE  -- setting i < 28 fixed the problem.
			//There must be garbage data at the end of line beyond the 28 tokens
			while ( i< 28 && (token = strsep(&line, ",")) != NULL ) {

				strsize = strlen(token);
				if (strsize == 0) { //NULL TOKEN
					new_row->tokens[i] = malloc(sizeof(char) * 2); //to simplify freeing later
					strcpy(new_row->tokens[i], "!\0");
					new_row->tokenmeta[i] = NULL_TOK;
				} else {
					new_row->tokens[i] = malloc(sizeof(char) * strsize + 1);
					strcpy(new_row->tokens[i], token);
					new_row->tokenmeta[i] = COMMA;
				}
				i++;
			}
		}
		insert(new_row);

/*
        //DEBUGGING CODE
		printf("\n[Original Line:] %s\n",copy);
		printf("[Row number: %d  Number of tokens: %d] ",h++,i);
		for(i = 0; i < 28; i++){
			printf("-%s-", new_row->tokens[i]);
		}
		printf("\n");
		//DEBUGGING CODE
*/


		//reset line to the beginning of the allocated memory
		line = pholder;
		//Using length of last token -- Memory will already be allocated AT LEAST that much
		line = readline(fd, line, len);

/*
		//DEBUGGING CODE
		free(copy);
		copy = malloc(sizeof(char)*strlen(line)+1);
		strcpy(copy, line);
		//DEBUGGING CODE
*/

		//get size of the new string from readline for check in outer while loop
		len = strlen(line);
		//reset pholder to line for later
		pholder = line;
	}
	print_list();
	//free(copy);
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

void print_list(){
    if ((head == NULL)) {
		printf("WARNING: free_list called with head==NULL");
		return;
	}
	record* lead = head;
	record* p;
	int i = 0;
	while(lead != NULL){
		p = lead;
		lead = lead->next;
		for(i = 0;  i < 28; i++){
			if(p->tokenmeta[i] == QUOTE)
				printf("\"%s\",",p->tokens[i]);
			else if( i < 27)
				printf("%s,",p->tokens[i]);
			else
				printf("%s",p->tokens[i]);
		}
		printf("\n");
	}
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
