#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "mergesort.h"
#include "getcolumns.h"
#include "readline.h"
#include "ADTList.h"

static int comparetype, col;

enum {
	NULL_TOK = -1, COMMA = 1, QUOTE = 2, INT = 3, DOUBLE = 4, STRING = 5,
};

int main(int argc, char *argv[]) {
	int len, error, col, i,  opt, h, strsize, size = 512;
	char *line, *token, *pholder, *quote, *tokensleft, *copy, *colheader;
	FILE *fd;

	int typearr[] = { STRING, STRING, INT, INT, INT, INT, STRING, INT, INT,
			STRING, STRING, STRING, INT, INT, STRING, INT, STRING, STRING, INT,
			STRING, STRING, STRING, INT, INT, INT, DOUBLE, DOUBLE, INT };

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
	error = geterror();
	if (error) {
		printf("Could not reallocate memory in readline\n");
		fclose(fd);
		free(line);
	        return -1;
	}

	record* headers = malloc(sizeof(record));
	col = getcolumns(line, headers, colheader);
	if(col == -1){
		printf("%s is not a valid column name.\n", colheader);
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
    error = geterror();
    if(error){
    	free(line);
    	for(i = 0; i < 28; i ++)
    		free(headers->tokens[i]);
    	free(headers);
    	fclose(fd);
    	printf("readline() failed\n");
    	return -1;
    }


    //set static variables in mergesort.c
    setcol(col);
    set_comparetype(comparetype);

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

			//get the single quotation delimited token in some of the lines.
			strsize = strlen(quote);
			new_row->tokens[i] = malloc(sizeof(char) * strsize + 1);
			strcpy(new_row->tokens[i], quote);
			new_row->tokenmeta[i] = QUOTE;
			i++;

			//Without explicitly stating-after 28 tokens stop tokenizing-
			//this block iterates one extra time getting garbage data
			//which must be present in movie_metadata.csv from when
			//it was downloaded.
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
		//reset line to the beginning of the allocated memory
		line = pholder;
		//Using length of last token -- Memory will already be allocated AT LEAST that much
		line = readline(fd, line, len);
                error = geterror();
                if(error){
                    free(line);
                    fclose(fd);
                    free_list();
                    return -1;
                }
		//get size of the new string from readline for check in outer while loop
		len = strlen(line);
		//reset pholder to line for later
		pholder = line;
	}
	reset_head(mergesort(get_hol()));
	insert(headers);
	print_list();
    free_list();
	free(line);
	fclose(fd);
	return 0;
}
