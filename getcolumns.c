#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ADTList.h"

#define COMMA 1

int getcolumns(char* line, record* headers, char* colheader){
    int col, strsize, i = 0;
    char *header, *p;
    col = -1;
    p = line;
    while((header = strsep(&line, ",")) != NULL){
        
        strsize = strlen(header);
        headers->tokens[i] = malloc(sizeof(char)*strsize +1);
        strcpy(headers->tokens[i], header);
        headers->tokenmeta[i] = COMMA;
        if(strcmp(colheader, header) == 0)
            col = i;
        i++;
    }

    line = p;
    return col;
}
