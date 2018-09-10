#include <stdio.h>
#include <stdlib.h>
#include "ADTList.h"

#define QUOTE 2

static record *head = NULL;

record* get_hol(){
	return head;
}

void reset_head(record* top){
	head = top;
}

void insert(record* newrow){
	newrow->next = head;
	head = newrow;
}

void print_list(){
    if ((head == NULL)) {
		printf("WARNING: print_list called with head==NULL");
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

void free_list() {
	if ((head == NULL)) {
		printf("WARNING: free_list called with head==NULL");
		return;
	}
	record* lead = head;
	record* p;
	int i = 0;
	while (lead != NULL) {
		p = lead;
		lead = lead->next;
		for (i = 0; i < 28; i++)
			free(p->tokens[i]);
		free(p);
	}
	head = NULL;
}
