#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mergesort.h"
#include "sorter.h"


static int col;
static int comparetype;

#define STRING 5
#define INT 3

void set_comparetype(int j){
    comparetype = j;
}

void setcol(int i){
	col = i;
}

int less(char* a, char* b) {
	int ai, bi;
	double ad, bd;
	if (strcmp(a, "!\0") == 0) {
		return 1;
	} else {

		if (comparetype == STRING) {
			if (strcmp(a, b) < 0) {
				return 1;
			} else
				return 0;
		} else if (comparetype == INT) {
			ai = 0;
			bi = 0;
			sscanf(a, "%d", &ai);
			sscanf(b, "%d", &bi);
			if (a < b) {
				return 1;
			} else
				return 0;
		} else {
			ad = 0;
			bd = 0;
			sscanf(a, "%lf", &ad);
			sscanf(b, "%lf", &bd);
			if (a < b) {
				return 1;
			} else
				return 0;
		}
	}
}

record* merge(record* a, record* b){
	record lead;
	record* c = &lead;
	while((a != NULL) && (b != NULL))
		if(less(a->tokens[col], b->tokens[col])){
			c->next = a;
			c = a;
			a = a->next;
		}
		else{
			c->next = b;
			c = b;
			b = b->next;
		}
	c->next = (a == NULL) ? b : a;
	return lead.next;
}

record* mergesort(record* c){
	record *a, *b;
	if(c == NULL || c->next == NULL)
		return c;
	a = c;
	b = c ->next;
	while((b != NULL) && (b->next != NULL)){
		c = c->next;
		b = b->next->next;
	}
	b = c->next;
	c->next = NULL;
	return merge(mergesort(a), mergesort(b));
}

