#ifndef ADTLIST_H
#define ADTLIST_H

typedef struct _record{
	char *tokens[28];
	int tokenmeta[28];
	struct _record* next;
}record; 

record* get_hol();

void reset_head(record* top);

void insert(record* new_record);

void free_list();

void print_list();

#endif
