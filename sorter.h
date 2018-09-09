#ifndef SORTER_H
#define SORTER_H
#include "sorter.h"

typedef struct _record{
  char *tokens[28];
  int tokenmeta[28];
  struct _record* next;
}record;

void insert(record* newrow);

char* readline(FILE* fd, char* line, int size);

void free_list();

void print_list();

void insert(record* new_row);


#endif
